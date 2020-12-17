#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "avt_math.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "UniformBuffer.h"
#include "Shader.h"
#include "Renderer.h"
#include "Camera.h"

namespace avt {

	struct Vertex{
		Vector4 position;
		Vector4 color;
	};

	class Mesh {
	public:

		std::vector<Vertex> _vertexes;
		
		std::vector<GLubyte> _indices;
		std::vector<Vector3> _vertices, _verticesData;
		std::vector<Vector3> _colors;
		std::vector<Vector2> _textures, _texturesData;
		std::vector<Vector3> _normals, _normalsData;

		std::vector <unsigned int> _verticesIdx, _texturesIdx, _normalsIdx;

		VertexArray _va;
		VertexBuffer _vb, _tb, _nb, _cb;
		IndexBuffer _ib;

		bool _texturesLoaded = false, _normalsLoaded = false;

		Mesh(){}

		Mesh(const std::string& filename) {
			loadMeshData(filename);
			processMeshData();
			freeMeshData();

			for (int i = 0; i < _vertices.size(); i++) {
				_colors.push_back({ 1.f, 1.f, 1.f });
			}
		}

		void loadOBJ(const std::string& filename) {
			loadMeshData(filename);
			processMeshData();
			freeMeshData();
		}

		void addVertex(const Vector3& v, const Vector4& color) {
			_vertexes.push_back({ v.to4D(), color });
		}

		void addVertex(const Vertex& v) {
			_vertexes.push_back(v);
		}
		
		void addFace(GLubyte i1, GLubyte i2, GLubyte i3) {
			_indices.push_back(i1);
			_indices.push_back(i2);
			_indices.push_back(i3);
		}

		void colorAll(Vector3 color);

		void applyTransform(Mat4 mat);
		void setup();
		void setold();

		std::vector<Vector3>& getVertices() {
			return _vertices;
		}

		VertexArray& va() {
			return _va;
		}

		VertexBuffer& vb() {
			return _vb;
		}

		IndexBuffer& ib() {
			return _ib;
		}

		void computeNormals();

		void update(float time);

	private:

		void parseVertex(std::stringstream& sin);

		void parseTexture(std::stringstream& sin);

		void parseNormal(std::stringstream& sin);

		void parseFace(std::stringstream& sin);

		void parseLine(const std::string& line);

		void loadMeshData(const std::string& filename);

		void processMeshData();
		
		void freeMeshData();
		
		void setTexturesLoaded(bool b);
		
		void setNormalsLoaded(bool b);

	};
}