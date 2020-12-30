#pragma once

#pragma once

#include "Mesh.h"
#include "Perlin.h"

namespace avt {

	class Cloud : public Mesh {
	public:
		Cloud() {
			//Vertex field with squares in order
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					Mesh m = Mesh("./Resources/Objects/cube_vtn_flat.obj");
					m.colorAll({ 1.0f, 1.0f, 1.0f });
					m.applyTransform(Mat4::translation({(float)i, 0.0f, (float)j}));
					float perlin = Perlin::perlin(i, j);
					if (perlin-0.4f> 0) {
						m.applyTransform(Mat4::scale({perlin - 0.4f, perlin-0.4f, perlin - 0.4f}));

						for (auto v : m._vertices) {
							_vertices.push_back(v);
						}
						for (auto c : m._colors) {
							_colors.push_back(c);
						}
					}
				}
			}
			
			computeNormals();
			applyTransform(Mat4::scale({ 0.25f, 0.25f, 0.25f }));

		}

		void update(float t) {
			for (auto v : _vertices) {
				v.setY(Perlin::perlin(v.x() + t, v.z() + t)-0.5f);
			}
			computeNormals();
		}
	};

}
