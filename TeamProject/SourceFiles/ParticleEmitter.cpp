#include "../HeaderFiles/ParticleEmitter.h"


namespace avt {

	void ParticleEmitter::create() {

		_smokeTexture.setWrap(GL_REPEAT, GL_REPEAT);
		_smokeTexture.setFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
		_smokeTexture.useMipmap();
		_smokeTexture.create("Resources/textures/particleSmoke.png");

		_va.create();

		VertexBufferLayout quad_layout;
		quad_layout.add<GLfloat>(3); // VERTICES
		quad_layout.add<GLfloat>(2); // TEXTURE COORDS

		_quad_vb.create(vertices, 4 * 5 * sizeof(float));
		_va.addBuffer(_quad_vb, quad_layout);
		_quad_vb.unbind();

		VertexBufferLayout instance_layout;
		instance_layout.setDivisor(1);
		instance_layout.add<GLfloat>(3); // POSITIONS
		instance_layout.add<GLfloat>(4); // COLOR
		instance_layout.add<GLfloat>(1); // SIZE
		instance_layout.add<GLfloat>(1); // ROT

		_instance_vb.create(nullptr, (long long int)_maxParticles * 9 * sizeof(float));
		_va.addBuffer(_instance_vb, instance_layout);
		_instance_vb.unbind();

		_va.unbind();
	}

	void ParticleEmitter::spawn(float dt) {
		if (_time <= 0.01) return;

		_time = 0;

		for (int i = 0; i < 1; i++) {
			Particle* unused = getUnused();

			if (!unused && _particles.size() >= _maxParticles) return;

			if (unused) {
				unused->s = {};
				unused->v = Vector3(randrange(-.3f, .3f), 1.f, randrange(-.3f, .3f)).normalized() * 3;
				unused->size = randrange(25.f, 60.f);
				unused->rot = randrange(0, PI / 2);
				unused->color = { randrange(.4f,.6f),1,1,0 };
				unused->age = 0;
				unused->lifetime = randrange(6.f, 12.f);;
			}
			else {
				Particle* p = new Particle;// { randrange(.2f, .9f), randrange(0, .1f), randrange(0, .1f), .5f });
				p->s = {};
				p->color = { randrange(.4f,.6f),1,1,0 };
				p->v = Vector3(randrange(-.3f, .3f), 1.f, randrange(-.3f, .3f)).normalized() * 3;
				p->lifetime = randrange(7.f, 15.f);
				p->rot = randrange(0, PI / 2);
				p->size = randrange(25.f, 60.f);
				_particles.push_back(p);

			}
		}

	}

	void ParticleEmitter::updateParticles(float dt) {
		for (auto p : _particles) {
			if (p->age < 0) continue;
			p->s += p->v * dt;
			p->age += dt;
			if (p->age < p->lifetime / 4.0) p->color.setW(p->age / p->lifetime / .25f);
			if (p->age > p->lifetime / 2) p->color.setW(2 - 2 * p->age / p->lifetime);
		}
	}

	void ParticleEmitter::kill(double dt) {
		for (auto p : _particles) {
			if (p->age > p->lifetime) p->age = -1;
		}
	}


	void ParticleEmitter::draw(Shader* shader, const Mat4& worldMatrix, Light* light) {
		auto newWorldMat = worldMatrix * getTransform();
		Shader* curr_shader = getShader() ? getShader() : shader;
		curr_shader->bind();

		_va.bind();

		std::vector<float> data;
		for (auto p : _particles) {
			if (p->age < 0) continue;
			data.push_back(p->s.x());
			data.push_back(p->s.y());
			data.push_back(p->s.z());
			data.push_back(p->color.x());
			data.push_back(p->color.y());
			data.push_back(p->color.z());
			data.push_back(p->color.w());
			data.push_back(p->size);
			data.push_back(p->rot);
		}
		_instance_vb.fill(data.data(), (GLsizei)data.size() * sizeof(float));
		_instance_vb.unbind();

		_smokeTexture.bind();

		beforeDraw();
		glDepthMask(GL_FALSE);
		glUniformMatrix4fv(curr_shader->getUniform(MODEL_MATRIX), 1, GL_FALSE, newWorldMat.data());
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, (GLsizei)data.size() / 9);
		glDepthMask(GL_TRUE);
		afterDraw();

		_smokeTexture.unbind();

		_va.unbind();
		curr_shader->unbind();

	}

}
