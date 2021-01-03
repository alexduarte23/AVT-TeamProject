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
		Vector3 position;
		Vector2 tex;
		Vector3 normal;
		Vector3 color;
	};

	class Mesh {
	public:

		std::vector<Vertex> _meshData;

		VertexBuffer _vb;
		VertexArray _va;


		Mesh() {}

		Mesh(const std::string& filename) {
			loadOBJ(filename);
		}

		void loadOBJ(const std::string& filename);

		void addFace(const Vertex& v1, const Vertex& v2, const Vertex& v3, bool computeFaceNormal = false);

		void updateBufferData();


		void colorAll(Vector3 color);
		void applyTransform(Mat4 mat);

		void setup();


		std::vector<Vertex>& getMeshData() {
			return _meshData;
		}

		VertexArray& va() {
			return _va;
		}

		VertexBuffer& vb() {
			return _vb;
		}

		void computeFaceNormals();

		void clearLocalData() {
			_meshData.clear();
		}


	private:

		void parseLine(const std::string& line, std::vector<Vector3>& vertices, std::vector<Vector2>& textures, std::vector<Vector3>& normals);
		void parseVertex(std::stringstream& sin, std::vector<Vector3>& vertices);
		void parseTexture(std::stringstream& sin, std::vector<Vector2>& textures);
		void parseNormal(std::stringstream& sin, std::vector<Vector3>& normals);
		void parseFace(std::stringstream& sin, const std::vector<Vector3>& vertices, const std::vector<Vector2>& textures, const std::vector<Vector3>& normals);

	};
}