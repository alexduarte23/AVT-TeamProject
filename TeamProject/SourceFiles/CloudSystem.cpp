#include "../HeaderFiles/CloudSystem.h"


#include "../HeaderFiles/Perlin.h"
#include "../HeaderFiles/StencilPicker.h"


namespace avt {

	void CloudSystem::move() {
		for (int i = 0; i < rowN; i++) {
			for (int j = 0; j < rowN - 1; j++) {
				grid[i][j].perlin = grid[i][j + 1].perlin;
				grid[i][j].time = grid[i][j + 1].time;

				if (grid[i][j].perlin != 0 && grid[i][j].size <= 0) {
					grid[i][j].pulseOffset = randrange(0, 2.f * PI);
					grid[i][j].wobbleOffset = randrange(0, 2.f * PI);
				}
			}
			grid[i][rowN - 1].perlin = 0;
			grid[i][rowN - 1].time = 0;
		}
	}

	void CloudSystem::updateSizes(float dt) {
		for (int i = 0; i < rowN; i++) {
			for (int j = 0; j < rowN; j++) {
				auto& cell = grid[i][j];

				if (cell.perlin != 0 || cell.size != 0) {
					cell.pulseOffset = fmod(cell.pulseOffset + dt*pulseSpeed, 2 * PI);
					cell.wobbleOffset = fmod(cell.wobbleOffset + dt*wobbleSpeed, 2 * PI);
				}

				if (cell.time > 0) {
					cell.time -= dt;
					if (cell.time <= 0) {
						cell.time = 0;
						cell.perlin = 0;
					}
				}

				if (cell.size == cell.perlin) continue;

				float v = cell.perlin - cell.size;
				float sign = v / abs(v);
				cell.size += v / abs(v) * dt * growth;
				//cell.size = sign > 0 ? min(cell.size, cell.perlin) : max(cell.size, cell.perlin);
				cell.size = sign > 0 ? min(cell.size, cell.perlin) : max(cell.size, cell.perlin);
			}
		}
	}

	void CloudSystem::setupBuffers() {
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
	}


	void CloudSystem::createCloud() {
		Vector2 gridPoint(randrange(0, 100.f), randrange(0, 100.f));

		int n = (int)randrange(3.f, 15.f);
		int m = (int)randrange(3.f, 15.f);
		Vector2 start(round(randrange(0, (float)rowN - n)), round(randrange(0, (float)rowN - m)));
		float lifetime = randrange(20.f, 30.f);

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				float p = Perlin::perlin(perlinSpacing * n / 2 - gridPoint.x() + i * perlinSpacing, perlinSpacing * m / 2 - gridPoint.y() + j * perlinSpacing);
				p = p / 2.f + .5f;
				p = p < threshold ? 0 : (p - threshold) / (1 - threshold);
				p = p > 0.6f ? 0.6f : p;
				p *= 3.f;
				p = p * p;
				p /= 4.f;
				p = p < threshold2 ? 0 : p;
				//if (p > 0) std::cout << p << std::endl;
				if (p == 0) continue;

				auto& cell = grid[(int)start.x() + i][(int)start.y() + j];
				float pulseOffset = randrange(0, 2 * PI);
				float wobbleOffset = randrange(0, 2 * PI);
				if (cell.size == 0) cell = { p, -p, lifetime, pulseOffset, wobbleOffset };
				else cell = { p, cell.size, lifetime, cell.pulseOffset, cell.wobbleOffset };

			}
		}
	}

	void CloudSystem::update(double dt) {
		moveTimer += (float)dt;
		spawnTimer += (float)dt;
		if (moveTimer >= movePeriod) {
			moveTimer = 0;
			move();
		}
		if (spawnTimer >= spawnPeriod) {
			spawnTimer = 0;
			createCloud();
		}
		updateSizes((float)dt);
	}

	void CloudSystem::draw(Shader* shader, const Mat4& worldMatrix, Light* light) {
		auto newWorldMat = worldMatrix * getTransform();
		Shader* curr_shader = getShader() ? getShader() : shader;
		if (getShader()) {
			getShader()->bind();
			/*
			glUniform3f(curr_shader->getUniform("LightPosition"), light->getPosition().x(), light->getPosition().y(), light->getPosition().z());
			glUniform3f(curr_shader->getUniform("LightColor"), light->getColor().x(), light->getColor().y(), light->getColor().z());
			*/
		}

		_va.bind();

		StencilPicker::prepareStencil(getStencilIndex());

		std::vector<CloudInfo> data;
		for (int i = 0; i < rowN; i++) {
			for (int j = 0; j < rowN; j++) {
				auto& cell = grid[i][j];
				if (cell.size <= 0) continue;

				float pulse = pulseStrength * sin(cell.pulseOffset);
				float wobble = wobbleStrength * sin(cell.wobbleOffset);
				data.push_back({ { spacing * i - rowN * spacing / 2 + wobble, 0, spacing * j - rowN * spacing / 2 }, cell.size + pulse });
			}
		}
		_instance_vb.fill(data.data(), (GLsizei)data.size() * sizeof(CloudInfo));
		_instance_vb.unbind();


		beforeDraw();
		glUniformMatrix4fv(curr_shader->getUniform(MODEL_MATRIX), 1, GL_FALSE, newWorldMat.data());
		glDrawArraysInstanced(GL_TRIANGLES, 0, (GLsizei)_cubeData.size(), (GLsizei)data.size());
		afterDraw();

		_va.unbind();

		if (getShader()) {
			getShader()->unbind();
			shader->bind();
		}

	}

}