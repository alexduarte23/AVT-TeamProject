#pragma once

#pragma once

#include "SceneNode.h"
#include "Perlin.h"
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
		};

		Mesh _cubeMesh;
		std::vector<Vertex> _cubeData;
		//std::vector<CloudInfo> _cloudCubes;
		VertexBuffer _cube_vb, _instance_vb;
		VertexArray _va;

		GridCell** grid = nullptr;

		float spacing = 0.7f;
		float perlinSpacing = 0.2;
		float threshold = 0.4;
		float threshold2 = 0.07;
		int rowN = 15;
		int _maxCubes = rowN * rowN;

		float movePeriod = 1.0;
		float growth = 0.1;

		float time = 0;

	public:
		CloudSystem() : _cubeData(Mesh::loadOBJ("./Resources/Objects/cube_vtn_flat.obj")) {
			//_cubeMesh("./Resources/Objects/cube_vtn_flat.obj") {

			//_cubeMesh.setup();
			
			_va.create();

			VertexBufferLayout cube_layout;
			cube_layout.add<GLfloat>(3); // POSITION
			cube_layout.add<GLfloat>(2); // TEXTURE COORD
			cube_layout.add<GLfloat>(3); // NORMAL
			cube_layout.add<GLfloat>(3); // COLOR

			_cube_vb.create(_cubeData.data(), _cubeData.size() * sizeof(Vertex));
			_va.addBuffer(_cube_vb, cube_layout);
			_cube_vb.unbind();

			VertexBufferLayout instance_layout;
			instance_layout.setDivisor(1);
			instance_layout.add<GLfloat>(3); // POSITIONS
			instance_layout.add<GLfloat>(1); // SIZE

			_instance_vb.create(nullptr, (long long int)_maxCubes * sizeof(CloudInfo));
			_va.addBuffer(_instance_vb, instance_layout);
			_instance_vb.unbind();

			_va.unbind();
			

			grid = new GridCell*[rowN];
			for (int i = 0; i < rowN; i++) {
				grid[i] = new GridCell[rowN];
			}

			regen();




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

		~CloudSystem() {
			for (int i = 0; i < rowN; i++) {
				delete[] grid[i];
			}
			delete[] grid;
		}

		void regen() {
			//deleteAll();

			Vector2 gridPoint(randrange(0, 100), randrange(0, 100));
			gridPoint = Vector2(randrange(0, 100), randrange(0, 100));

			float max = 0;
			Vector2 maxPos;

			for (int i = 0; i < rowN; i++) {
				for (int j = 0; j < rowN; j++) {
					float p = Perlin::perlin(perlinSpacing * rowN / 2 - gridPoint.x() + i * perlinSpacing, perlinSpacing * rowN / 2 - gridPoint.x() + j * perlinSpacing);
					p = p / 2.f + .5f;
					p = p < threshold ? 0 : (p - threshold) / (1 - threshold);
					p = p > 0.6 ? 0.6 : p;
					p *= 3;
					p = p * p;
					p /= 4;
					p = p < threshold2 ? 0 : p;
					//if (p > 0) std::cout << p << std::endl;
					if (p > max) {
						max = p;
						maxPos = { 1.f*i, 1.f*j };
					}
					grid[i][j] = {p, p};
					/*if (p) {
						//_cloudCubes.push_back({ {spacing * i, 0, spacing * j}, p });
						auto cube = createNode(&_cubeMesh);
						cube->scale(Vector3() + p);
						cube->translate({ spacing * i, 0, spacing * j });
					}*/

				}
			}

			/*for (int i = 0; i < rowN; i++) {
				for (int j = 0; j < rowN; j++) {
					if (grid[i][j].size == 0) continue;

					auto cube = createNode(&_cubeMesh);
					cube->scale(Vector3() + grid[i][j].size);
					cube->translate({ spacing * i, 0, spacing * j });
				}
			}*/
		}

		void move() {
			for (int i = 0; i < rowN; i++) {
				for (int j = 0; j < rowN-1; j++) {
					grid[i][j].perlin = grid[i][j+1].perlin;
				}
				grid[i][rowN - 1].perlin = 0;
			}
		}

		void updateSizes(float dt) {
			for (int i = 0; i < rowN; i++) {
				for (int j = 0; j < rowN; j++) {
					if (grid[i][j].size == grid[i][j].perlin) continue;
					
					float v = grid[i][j].perlin - grid[i][j].size;
					float sign = v / abs(v);
					grid[i][j].size += v / abs(v) * dt * growth;
					grid[i][j].size = sign > 0 ? min(grid[i][j].size, grid[i][j].perlin) : max(grid[i][j].size, grid[i][j].perlin);
				}
			}
		}

		void update(float dt) {
			time += dt;
			if (time >= movePeriod) {
				time = 0;
				move();
			}
			updateSizes(dt);


			/*deleteAll();
			for (int i = 0; i < rowN; i++) {
				for (int j = 0; j < rowN; j++) {
					if (grid[i][j].size == 0) continue;

					auto cube = createNode(&_cubeMesh);
					cube->scale(Vector3() + grid[i][j].size);
					cube->translate({ spacing * i, 0, spacing * j });
				}
			}*/
		}

		void draw(Shader* shader, const Mat4& worldMatrix, Light* light) override {
			auto newWorldMat = worldMatrix * getTransform();
			Shader* curr_shader = getShader() ? getShader() : shader;
			if (getShader()) {
				getShader()->bind();
				glUniform3f(curr_shader->getUniform("LightPosition"), light->getPosition().x(), light->getPosition().y(), light->getPosition().z());
				glUniform3f(curr_shader->getUniform("LightColor"), light->getColor().x(), light->getColor().y(), light->getColor().z());
			}

			_va.bind();

			StencilPicker::prepareStencil(getStencilIndex());

			std::vector<CloudInfo> data;
			for (int i = 0; i < rowN; i++) {
				for (int j = 0; j < rowN; j++) {
					if (grid[i][j].size == 0) continue;
					data.push_back({ { spacing * i, 0, spacing * j }, grid[i][j].size });
				}
			}
			_instance_vb.fill(data.data(), (GLsizei)data.size() * sizeof(CloudInfo));
			_instance_vb.unbind();


			beforeDraw();
			glUniformMatrix4fv(curr_shader->getUniform(MODEL_MATRIX), 1, GL_FALSE, newWorldMat.data());
			glDrawArraysInstanced(GL_TRIANGLES, 0, _cubeData.size(), (GLsizei)data.size());
			afterDraw();

			_va.unbind();

			if (getShader()) {
				getShader()->unbind();
				shader->bind();
			}

		}

	};

}
