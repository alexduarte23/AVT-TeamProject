#include "../HeaderFiles/ParticleEmitter.h"
#include "../HeaderFiles/StencilPicker.h"


namespace avt {

	const float ParticleEmitter::QUAD_STRIP[5 * 4] = {
			-1.f, -1.f, 0,      0,   0, // bottom left
			 1.f, -1.f, 0,    1.f,   0, // bottomm right
			-1.f,  1.f, 0,      0, 1.f, // top left
			 1.f,  1.f, 0,    1.f, 1.f  // top right
	};

	void ParticleEmitter::create(const std::string& texture_filename) {
		_texture.setWrap(GL_REPEAT, GL_REPEAT);
		_texture.setFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
		_texture.useMipmap();
		_texture.create(texture_filename);

		_va.create();

		VertexBufferLayout quad_layout;
		quad_layout.add<GLfloat>(3); // VERTICES
		quad_layout.add<GLfloat>(2); // TEXTURE COORDS

		_quad_vb.create(QUAD_STRIP, sizeof(QUAD_STRIP));
		_va.addBuffer(_quad_vb, quad_layout);
		_quad_vb.unbind();

		VertexBufferLayout instance_layout;
		instance_layout.setDivisor(1);
		instance_layout.add<GLfloat>(3); // POSITIONS
		instance_layout.add<GLfloat>(4); // COLOR
		instance_layout.add<GLfloat>(1); // SIZE
		instance_layout.add<GLfloat>(1); // ROT

		_instance_vb.create(nullptr, (long long int)_maxParticles * sizeof(ParticleBody));
		_va.addBuffer(_instance_vb, instance_layout);
		_instance_vb.unbind();

		_va.unbind();
	}

	bool ParticleEmitter::addParticle(const Particle& particle) {
		Particle* unused = getUnused();

		if (!unused && _particles.size() >= _maxParticles) return false;

		if (unused) {
			unused->s = particle.s;
			unused->v = particle.v;
			unused->a = particle.a;
			unused->color = particle.color;
			unused->size = particle.size;
			unused->rot = particle.rot;
			unused->age = particle.age;
			unused->lifetime = particle.lifetime;
		}
		else {
			Particle* p = new Particle(particle);
			_particles.push_back(p);

		}

		return true;
	}

	void ParticleEmitter::draw(Shader* shader, const Mat4& worldMatrix, Light* light) {
		auto newWorldMat = worldMatrix * getTransform();
		Shader* curr_shader = getShader() ? getShader() : shader;
		curr_shader->bind();

		_va.bind();

		StencilPicker::prepareStencil(getStencilIndex());

		std::vector<ParticleBody> data;
		for (auto p : _particles) {
			if (p->age < 0) continue;
			data.push_back({ p->s, p->color, p->size, p->rot });
		}
		_instance_vb.fill(data.data(), (GLsizei)data.size() * sizeof(ParticleBody));
		_instance_vb.unbind();

		_texture.bind();

		beforeDraw();
		glDepthMask(GL_FALSE);
		glUniformMatrix4fv(curr_shader->getUniform(MODEL_MATRIX), 1, GL_FALSE, newWorldMat.data());
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, (GLsizei)data.size());
		glDepthMask(GL_TRUE);
		afterDraw();

		_texture.unbind();

		_va.unbind();
		curr_shader->unbind();

	}


	// SMOKE EMITTER

	void SmokeEmitter::spawn(float dt) {
		//if (_time <= 0.01) return;

		//_time = 0;

		//for (int i = 0; i < 1; i++) {
			Particle p;
			p.s = {};
			p.v = Vector3(randrange(-.3f, .3f), 1.f, randrange(-.3f, .3f)).normalized() * 3;
			p.size = randrange(2.f, 6.f);
			p.rot = randrange(0, PI / 2);
			p.color = { randrange(.4f,.6f),1,1,0 };
			p.age = 0;
			p.lifetime = randrange(6.f, 12.f);

			addParticle(p);
		//}

	}

	void SmokeEmitter::updateParticles(float dt) {
		for (auto p : _particles) {
			if (p->age < 0) continue;
			p->s += p->v * dt;
			p->age += dt;
			if (p->age < p->lifetime / 4.0) p->color.setW(p->age / p->lifetime / .25f);
			if (p->age > p->lifetime / 2) p->color.setW(2 - 2 * p->age / p->lifetime);
		}
	}

	void SmokeEmitter::kill(float dt) {
		for (auto p : _particles) {
			if (p->age > p->lifetime) p->age = -1;
		}
	}


	// DUST EMITTER

	void DustEmitter::spawn(float dt) {
		createParticle();
		/*Particle p;
		p.s = {};
		p.v = Vector3(randrange(-.3f, .3f), 1.f, randrange(-.3f, .3f)).normalized() * 3;
		p.size = randrange(25.f, 60.f);
		p.rot = randrange(0, PI / 2);
		p.color = { randrange(.4f,.6f),1,1,0 };
		p.age = 0;
		p.lifetime = randrange(6.f, 12.f);

		addParticle(p);*/
	}

	void DustEmitter::updateParticles(float dt) {
		for (auto p : _particles) {
			if (p->age < 0) continue;
			p->s += p->v * dt;
			p->age += dt;
			if (p->age < 2.f) p->color.setW(p->initialColor.w() / 2.f * p->age);
			else if (p->age > p->lifetime - 2.f) p->color.setW(-p->initialColor.w()/2.f*p->age + p->initialColor.w() + p->initialColor.w()/2.f*(p->lifetime-2.f));
			else p->color.setW(p->initialColor.w());
		}
	}

	void DustEmitter::kill(float dt) {
		for (auto p : _particles) {
			if (p->age > p->lifetime) p->age = -1;
		}
	}

}
