#pragma once

#pragma once

#include "SceneNode.h"
#include "Mesh.h"
#include <vector>

namespace avt {

	class CloudSystem : public SceneNode {
	private:
		
		struct CloudInfo {
			Vector3 pos;
			float size = 0;
		};

		struct GridCell {
			float perlin = 0;
			float size = 0;
			float time = 0;
			float pulseOffset = 0;
			float wobbleOffset = 0;
		};

		std::vector<Vertex> _cubeData;
		VertexBuffer _cube_vb, _instance_vb;
		VertexArray _va;

		GridCell** grid = nullptr;

		float spacing = 0.5f;
		float perlinSpacing = 0.15f;
		float threshold = 0.35f;
		float threshold2 = 0.05f;
		//int gridN = 50;
		int rowN = 80;
		int _maxCubes = rowN * rowN;

		float movePeriod = .9f;
		float spawnPeriod = 2.0f;
		float growth = 0.12f;

		float pulseSpeed = 4.f;
		float pulseStrength = 0.07f;

		float wobbleSpeed = 3.f;
		float wobbleStrength = 0.12f;

		float moveTimer = 0;
		float spawnTimer = 0;


		void move();

		void updateSizes(float dt);

		void setupBuffers();

	public:
		CloudSystem()
			: _cubeData(Mesh::loadOBJ("./Resources/Objects/cube_vtn_flat.obj")) {
			
			setupBuffers();

			grid = new GridCell*[rowN];
			for (int i = 0; i < rowN; i++) {
				grid[i] = new GridCell[rowN];
				for (int j = 0; j < rowN; j++) grid[i][j] = {0, 0};
			}

			for (int i = 0; i < 15; i++) createCloud();
		}

		~CloudSystem() {
			for (int i = 0; i < rowN; i++) {
				delete[] grid[i];
			}
			delete[] grid;
		}

		void createCloud();

		void update(double dt);

		void draw(Shader* shader, const Mat4& worldMatrix, Light* light) override;

	};

}
