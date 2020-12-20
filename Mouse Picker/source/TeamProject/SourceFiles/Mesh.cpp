#include "../HeaderFiles/Mesh.h"

namespace avt {

	void Mesh::colorAll(Vector4 color) {
		for (int i = 0; i < _vertices.size(); i++) {
			_vertices[i].color = color;
		}
	}

	void Mesh::applyTransform(Mat4 mat) {
		for (int i = 0; i < _vertices.size(); i++) {
			_vertices[i].position = mat * _vertices[i].position;
		}
	}

	void Mesh::setup() {
		_vb.create(_vertices.data(), _vertices.size() * sizeof(Vertex));
		_ib.create(_indices.data(), (GLuint)_indices.size());

		VertexBufferLayout layout;
		layout.add<GLfloat>(4); // VERTICES
		layout.add<GLfloat>(4); // COLORS
		_va.create();
		_va.addBuffer(_vb, layout);

		_va.unbind();
		_vb.unbind();
		_ib.unbind();
	}

	void Mesh::parseVertex(std::stringstream& sin) {
		Vector3 v;
		sin >> v;
		_vertices.push_back({ v.to4D(),{0,0,0,0} });
	}

	void Mesh::parseFace(std::stringstream& sin) {
		std::string token;
		for (int i = 0; i < 3; i++) {
			sin >> token;
			_indices.push_back((GLubyte)(std::stoi(token) - 1));
		}
	}

	void Mesh::parseLine(const std::string& line) {
		std::stringstream sline(line);
		std::string s;

		sline >> s;
		if (s.compare("v") == 0) parseVertex(sline);
		else if (s.compare("f") == 0) parseFace(sline);
	}

	void Mesh::loadMeshData(const std::string& filename) {
		std::ifstream ifile(filename);
		std::string line;
		while (std::getline(ifile, line)) {
			parseLine(line);
		}
	}

}