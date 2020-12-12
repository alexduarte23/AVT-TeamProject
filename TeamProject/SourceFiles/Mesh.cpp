#include "../HeaderFiles/Mesh.h"
#include "../HeaderFiles//Globals.h"

namespace avt {
	/**/
	void Mesh::colorAll(Vector3 color) {
		for (int i = 0; i < _vertices.size(); i++) {
			_colors[i] = color;
		}
	}

	void Mesh::applyTransform(Mat4 mat) {
		for (int i = 0; i < _vertices.size(); i++) {
			avt::Vector4 result = mat * _vertices[i];
			_vertices[i].setX(result.x());
			_vertices[i].setY(result.y());
			_vertices[i].setZ(result.z());
		}
	}
	/**/
	void Mesh::setup() {
		_va.create();

		VertexBufferLayout layout;

		_vb.create(_vertices.data(), _vertices.size() * sizeof(Vector3));
		layout.add<GLfloat>(3); // VERTICES
		_va.addBuffer(_vb, layout, VERTICES);

		if (_texturesLoaded) {
			_tb.create(_textures.data(), _textures.size() * sizeof(Vector2));
			layout.clear();
			layout.add<GLfloat>(2); // TEXTURES
			_va.addBuffer(_tb, layout, TEXTURES);
		}
		if (_normalsLoaded) {
			_nb.create(_normals.data(), _normals.size() * sizeof(Vector3));
			layout.clear();
			layout.add<GLfloat>(3); // NORMALS
			_va.addBuffer(_nb, layout, NORMALS);
		}

		_cb.create(_colors.data(), _colors.size() * sizeof(Vector3));
		layout.clear();
		layout.add<GLfloat>(3); // COLORS
		_va.addBuffer(_cb, layout, COLORS);

		layout.clear();
		_va.unbind();
		_vb.unbind();
	}

	void Mesh::setold() {
		_vb.create(_vertexes.data(), _vertexes.size() * sizeof(Vertex));
		_ib.create(_indices.data(), (GLuint)_indices.size());

		VertexBufferLayout layout;
		layout.add<GLfloat>(4); // VERTICES
		layout.add<GLfloat>(4); // COLORS
		_va.create();
		_va.addBuffer(_vb, layout, 0);

		_va.unbind();
		_vb.unbind();
		_ib.unbind();
	}

	void Mesh::parseVertex(std::stringstream& sin) {
		Vector3 v;
		sin >> v;
		_verticesData.push_back(v);
		_vertexes.push_back({ v.to4D(), {0,0,0,0} });
	}

	void Mesh::parseTexture(std::stringstream& sin) {
		Vector2 t;
		sin >> t;
		_texturesData.push_back(t);
	}

	void Mesh::parseNormal(std::stringstream& sin) {
		Vector3 n;
		sin >> n;
		_normalsData.push_back(n);
	}

	void Mesh::parseFace(std::stringstream& sin) {
		std::string token;
		if (_normalsData.empty() && _texturesData.empty())
		{
			for (int i = 0; i < 3; i++)
			{
				sin >> token;
				_indices.push_back(std::stoi(token) - 1);
			}
		}
		else
		{
			for (int i = 0; i < 3; i++)
			{
				std::getline(sin, token, '/');
				if (token.size() > 0) _verticesIdx.push_back(std::stoi(token));
				std::getline(sin, token, '/');
				if (token.size() > 0) _texturesIdx.push_back(std::stoi(token)); // 1 minus see video thinmatrix
				std::getline(sin, token, ' ');
				if (token.size() > 0) _normalsIdx.push_back(std::stoi(token));
			}
		}
	}

	void Mesh::parseLine(const std::string& line) {
		std::stringstream sline(line);
		std::string s;

		sline >> s;
		if (s.compare("v") == 0) parseVertex(sline);
		else if (s.compare("vt") == 0) parseTexture(sline);
		else if (s.compare("vn") == 0) parseNormal(sline);
		else if (s.compare("f") == 0) parseFace(sline);
	}

	void Mesh::loadMeshData(const std::string& filename) {
		std::ifstream ifile(filename);
		std::string line;
		while (std::getline(ifile, line)) {
			parseLine(line);
		}
		setTexturesLoaded(_texturesData.size() > 0);
		setNormalsLoaded(_normalsData.size() > 0);
	}

	void Mesh::processMeshData()
	{
		for (unsigned int i = 0; i < _verticesIdx.size(); i++) {
			unsigned int vi = _verticesIdx[i];
			Vector3 v = _verticesData[vi - 1];
			_vertices.push_back(v);
			if (_texturesLoaded)
			{
				unsigned int ti = _texturesIdx[i];
				Vector2 t = _texturesData[ti - 1];
				_textures.push_back(t);
			}
			if (_normalsLoaded)
			{
				unsigned int ni = _normalsIdx[i];
				Vector3 n = _normalsData[ni - 1];
				_normals.push_back(n);
			}
		}
	}

	void Mesh::freeMeshData()
	{
		_verticesData.clear();
		_texturesData.clear();
		_normalsData.clear();
		_verticesIdx.clear();
		_texturesIdx.clear();
		_normalsIdx.clear();
	}


	void Mesh::setTexturesLoaded(bool b) {
		_texturesLoaded = b;
	}

	void Mesh::setNormalsLoaded(bool b) {
		_normalsLoaded = b;
	}

}