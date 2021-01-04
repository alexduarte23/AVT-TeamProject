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
					addFace(
						{ vertices[i][j + 1], {}, {}, white },
						{ vertices[i + 1][j], {}, {}, white },
						{ vertices[i][j], {}, {}, white },
						true
					);

					addFace(
						{ vertices[i][j + 1], {}, {}, white },
						{ vertices[i + 1][j + 1], {}, {}, white },
						{ vertices[i + 1][j], {}, {}, white },
						true
					);
				}
			}

		}

		void update(float t) {
			for (auto& v : _meshData) {
				v.position.setY(Perlin::perlin(v.position.x()+t, v.position.z()+t) / 10);
			}
			computeFaceNormals();
			updateBufferData();
		}
	};

}
