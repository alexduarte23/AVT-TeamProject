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

		std::vector<Vertex> _vertices;
		std::vector<GLubyte> _indices;

		VertexArray _va;
		VertexBuffer _vb;
		IndexBuffer _ib;

		Mesh(){}

		Mesh(const std::string& filename) {
			loadMeshData(filename);
		}

		void loadOBJ(const std::string& filename) {
			loadMeshData(filename);
		}

		void addVertex(const Vector3& v, Vector4& color) {
			_vertices.push_back({ v.to4D(), color });
		}

		void addVertex(const Vertex& v) {
			_vertices.push_back(v);
		}

		void addFace(GLubyte i1, GLubyte i2, GLubyte i3) {
			_indices.push_back(i1);
			_indices.push_back(i2);
			_indices.push_back(i3);
		}

		void colorAll(Vector4 color);

		void applyTransform(Mat4 mat);

		void setup();

		std::vector<Vertex>& getVertices() {
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

	private:

		void parseVertex(std::stringstream& sin);

		void parseFace(std::stringstream& sin);

		void parseLine(const std::string& line);

		void loadMeshData(const std::string& filename);

	};
}