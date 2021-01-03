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
			Vector3 white(1.f, 1.f, 1.f);
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 9; j++) {
					_meshData.push_back({ vertices[i][j + 1], {}, {}, white });
					_meshData.push_back({ vertices[i + 1][j], {}, {}, white });
					_meshData.push_back({ vertices[i][j], {}, {}, white });

					_meshData.push_back({ vertices[i][j + 1], {}, {}, white });
					_meshData.push_back({ vertices[i + 1][j + 1], {}, {}, white });
					_meshData.push_back({ vertices[i + 1][j], {}, {}, white });
					/*
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
					_colors.push_back({ 1.0f, 1.0f, 1.0f });*/
				}
			}

			//Compute Normals
			computeFaceNormals();
			
		}

		void update(float t) {
			for (auto& v : _meshData) {
				v.position.setY(Perlin::perlin(v.position.x()+t, v.position.z()+t) / 10);
			}
			computeFaceNormals();
			setup();
		}
	};

}
