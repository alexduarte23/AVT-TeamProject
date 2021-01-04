#include "../HeaderFiles/Mesh.h"
#include "../HeaderFiles//Globals.h"

#include <set>

namespace avt {
	void Mesh::colorAll(Vector3 color) {
		for (auto& v : _meshData) {
			v.color = color;
		}
	}

	void Mesh::applyTransform(Mat4 mat) {
		for (auto& v : _meshData) {
			v.position = (mat * v.position.to4D()).to3D();
		}
	}

	void Mesh::setup() {
		_vb.create(_meshData.data(), _meshData.size() * sizeof(Vertex));

		VertexBufferLayout layout;
		layout.add<GLfloat>(3); // POSITION
		layout.add<GLfloat>(2); // TEXTURE COORD
		layout.add<GLfloat>(3); // NORMAL
		layout.add<GLfloat>(3); // COLOR

		_va.create();
		_va.addBuffer(_vb, layout);

		_va.unbind();
		_vb.unbind();
	}

	void Mesh::updateBufferData() {
		_vb.fill(_meshData.data(), _meshData.size() * sizeof(Vertex));
		_vb.unbind();
	}

	void Mesh::addFace(const Vertex& v1, const Vertex& v2, const Vertex& v3, bool computeFaceNormal) {
		_meshData.push_back(v1);
		_meshData.push_back(v2);
		_meshData.push_back(v3);

		if (computeFaceNormal) {
			Vector3 normal = (v1.position - v2.position).cross(v2.position - v3.position).normalized();
			size_t size = _meshData.size();
			_meshData[size - 1].normal = normal;
			_meshData[size - 2].normal = normal;
			_meshData[size - 3].normal = normal;
		}
	}

	void Mesh::computeFaceNormals() {
		for (size_t i = 0; i < _meshData.size(); i += 3) {
			Vertex& v1 = _meshData[i];
			Vertex& v2 = _meshData[i + 1];
			Vertex& v3 = _meshData[i + 2];

			Vector3 normal = (v1.position-v2.position).cross(v2.position - v3.position).normalized();

			v1.normal = normal;
			v2.normal = normal;
			v3.normal = normal;
		}
	}

	void Mesh::computeVertexNormals(bool weighted) {
		std::vector<Vertex*> current;
		const Vector3 zero;

		for (auto& v : _meshData) {
			v.normal = Vector3();
		}

		for (size_t i = 0; i < _meshData.size(); i++) {
			if (_meshData[i].normal != zero) continue;

			Vector3 normal;
			for (size_t j = i; j < _meshData.size(); j++) {
				if (_meshData[i].position != _meshData[j].position) continue;

				Vertex& v1 = _meshData[j - j%3];
				Vertex& v2 = _meshData[j - j%3 + 1];
				Vertex& v3 = _meshData[j - j%3 + 2];

				Vector3 fn = (v1.position - v2.position).cross(v2.position - v3.position);
				if (!weighted) fn.normalize();
				normal += fn;

				current.push_back(&_meshData[j]);
			}
			normal.normalize();
			for (auto v : current) {
				v->normal = normal;
			}

			current.clear();
		}
	}

	void Mesh::computeMixedNormals(float threshold, bool weighted) {
		std::vector<Vertex*> current;
		const Vector3 zero;

		for (auto& v : _meshData) {
			v.normal = Vector3();
		}

		for (size_t i = 0; i < _meshData.size(); i++) {
			if (_meshData[i].normal != zero) continue;

			Vertex& v1 = _meshData[i - i % 3];
			Vertex& v2 = _meshData[i - i % 3 + 1];
			Vertex& v3 = _meshData[i - i % 3 + 2];

			Vector3 baseNormal = (v1.position - v2.position).cross(v2.position - v3.position);
			Vector3 normal = baseNormal;
			if (!weighted) normal.normalize();
			current.push_back(&_meshData[i]);

			for (size_t j = i - i%3 + 3; j < _meshData.size(); j++) {
				if (_meshData[i].position != _meshData[j].position || _meshData[j].normal != zero) continue;

				Vertex& v1 = _meshData[j - j % 3];
				Vertex& v2 = _meshData[j - j % 3 + 1];
				Vertex& v3 = _meshData[j - j % 3 + 2];

				Vector3 fn = (v1.position - v2.position).cross(v2.position - v3.position);

				if (fn.angleTo(baseNormal) > threshold) continue;

				if (!weighted) fn.normalize();
				normal += fn;

				current.push_back(&_meshData[j]);
			}
			normal.normalize();
			for (auto v : current) {
				v->normal = normal;
			}

			current.clear();
		}
	}

	// OBJ LOADING

	void Mesh::loadOBJ(const std::string& filename, const Vector3& color) {
		std::vector<Vector3> vertices;
		std::vector<Vector3> normals;
		std::vector<Vector2> textures;

		std::ifstream ifile(filename);
		std::string line;
		while (std::getline(ifile, line)) {
			parseLine(line, vertices, textures, normals, color);
		}
	}

	void Mesh::parseLine(const std::string& line, std::vector<Vector3>& vertices, std::vector<Vector2>& textures, std::vector<Vector3>& normals, const Vector3& color) {
		std::stringstream sline(line);
		std::string s;

		sline >> s;
		if (s.compare("v") == 0) parseVertex(sline, vertices);
		else if (s.compare("vt") == 0) parseTexture(sline, textures);
		else if (s.compare("vn") == 0) parseNormal(sline, normals);
		else if (s.compare("f") == 0) parseFace(sline, vertices, textures, normals, color);
	}

	void Mesh::parseVertex(std::stringstream& sin, std::vector<Vector3>& vertices) {
		Vector3 v;
		sin >> v;
		vertices.push_back(v);
	}

	void Mesh::parseTexture(std::stringstream& sin, std::vector<Vector2>& textures) {
		Vector2 t;
		sin >> t;
		textures.push_back(t);
	}

	void Mesh::parseNormal(std::stringstream& sin, std::vector<Vector3>& normals) {
		Vector3 n;
		sin >> n;
		normals.push_back(n);
	}

	void Mesh::parseFace(std::stringstream& sin, const std::vector<Vector3>& vertices, const std::vector<Vector2>& textures, const std::vector<Vector3>& normals, const Vector3& color) {
		std::string vertexString, token;
		
		for (size_t i = 0; i < 3; i++) {
			Vertex v;
			sin >> vertexString;
			std::stringstream sinV(vertexString);

			std::getline(sinV, token, '/');
			v.position = vertices[std::stoll(token) - 1];

			std::getline(sinV, token, '/');
			v.tex = token.size() ? textures[std::stoll(token) - 1] : Vector2(0,0);

			std::getline(sinV, token, '/');
			v.normal = token.size() ? normals[std::stoll(token) - 1] : Vector3(0, 0, 0);

			v.color = color;

			_meshData.push_back(v);
			
		}
	}

}