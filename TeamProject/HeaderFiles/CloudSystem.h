#pragma once

#pragma once

#include "SceneNode.h"
#include "Perlin.h"

namespace avt {

	class CloudSystem : public SceneNode {
	private:
		
		Mesh _cubeMesh;

	public:
		CloudSystem() : _cubeMesh("./Resources/Objects/cube_vtn_flat.obj") {
			_cubeMesh.setup();
			createNode(&_cubeMesh);
			//loadOBJ("./Resources/Objects/cube_vtn_flat.obj");
			//Vertex field with squares in order
			/*
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					Mesh m = Mesh("./Resources/Objects/cube_vtn_flat.obj");
					m.colorAll({ 1.0f, 1.0f, 1.0f });
					m.applyTransform(Mat4::translation({(float)i, 0.0f, (float)j}));
					float perlin = Perlin::perlin(i, j);
					if (perlin-0.4f> 0) {
						m.applyTransform(Mat4::scale({perlin - 0.4f, perlin-0.4f, perlin - 0.4f}));

						for (auto v : m._meshData) {
							_meshData.push_back(v);
						}
					}
				}
			}
			
			computeFaceNormals();
			applyTransform(Mat4::scale({ 0.25f, 0.25f, 0.25f }));
			*/
		}

	};

}
