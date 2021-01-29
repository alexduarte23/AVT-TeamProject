#pragma once

#include "Mesh.h"
#include "Perlin.h"

namespace avt {

	class TerrainPlane : public Mesh {
	private:
		int _divs;
		int _size;

	public:
		TerrainPlane(int size=10, int divisions=25) : _divs(divisions), _size(size) {
			//Create horizontal vertex grid
			Vector3 **vertices = new Vector3*[_divs];
			for (int i = 0; i < _divs; i++) {
				vertices[i] = new Vector3[_divs];
				for (int j = 0; j < _divs; j++) {
					float x = _size * (float)i / _divs;
					float y = _size * (float)j / _divs;
					vertices[i][j] = Vector3(x, 2 * Perlin::perlin(x/2, y/2), y);
				}
			}

			//Connect Vertices
			Vector3 white(1.f, 1.f, 1.f);
			for (int i = 0; i < _divs-1; i++) {
				for (int j = 0; j < _divs-1; j++) {
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

			for (int i = 0; i < _divs; i++) {
				delete[] vertices[i];
			}
			delete[] vertices;

		}

	};

}
