#pragma once

#include "Mesh.h"
#include "Perlin.h"

namespace avt {

	class TerrainPlane : public Mesh {
	public:
		TerrainPlane() {
			//Create horizontal vertex grid
			Vector3 vertices[10][10];
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					vertices[i][j] = Vector3((float)i, 0.0f, (float)j);
					vertices[i][j].setY(Perlin::perlin((float)i, (float)j)/10);
				}
			}

			//Connect Vertices
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 9; j++) {
					_vertices.push_back(vertices[i][j + 1]);
					_vertices.push_back(vertices[i + 1][j]);
					_vertices.push_back(vertices[i][j]);

					_vertices.push_back(vertices[i][j + 1]);
					_vertices.push_back(vertices[i + 1][j+1]);
					_vertices.push_back(vertices[i + 1][j]);

					_colors.push_back({ 1.0f, 1.0f, 1.0f });
					_colors.push_back({ 1.0f, 1.0f, 1.0f });
					_colors.push_back({ 1.0f, 1.0f, 1.0f });
					_colors.push_back({ 1.0f, 1.0f, 1.0f });
					_colors.push_back({ 1.0f, 1.0f, 1.0f });
					_colors.push_back({ 1.0f, 1.0f, 1.0f });
				}
			}

			//Compute Normals
			computeNormals();
			
		}

		void update(float t) {
			for (auto v : _vertices) {
				v.setY(Perlin::perlin(v.x()+t, v.z()+t) / 10);
			}
			computeNormals();
			setup();
		}
	};

}
