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
		if (getShader()) {
			getShader()->bind();
		}

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

		if (getShader()) {
			getShader()->unbind();
			shader->bind();
		}

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
		Particle p;
		if (!_r && !_h) { // box
			p.s = Vector3(
				randrange(-_box.x() / 2, _box.x() / 2),
				randrange(-_box.y() / 2, _box.y() / 2),
				randrange(-_box.z() / 2, _box.z() / 2));
		}
		else { // cylinder
			float radius = randrange(0, _r), angle = randrange(0, 2*PI);
			p.s = Vector3(
				radius * cos(angle),
				randrange(-_h / 2, _h / 2),
				radius * sin(angle));
		}

		p.v = randVector(-1, 1).normalize() * randrange(.01f, .07f);//((Quaternion({0,0,1.f}, randrange(-PI/2,PI/2)) * Quaternion({0,1.f,0}, randrange(0,2*PI)) ).toMat() * Vector4(randrange(.2f,1.f),0,0,1.f)).to3D();

		float rand = random();
		if (rand < .03f) p.size = randrange(.025f, .03f); // bigger
		else if (rand < .5f) p.size = randrange(.01f, .02f); // medium
		else p.size = randrange(.005f, .009f); // smaller

		p.initialColor = { .5f,.5f,.5f,randrange(.1f, .7f) };
		p.color = p.initialColor;
		p.color.setW(0);
		p.lifetime = randrange(4.f, 8.f);

		addParticle(p);
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


	// FIREFLY EMITTER

	void FireflyEmitter::spawn(float dt) {
		Particle p;
		if (!_r && !_h) { // box
			p.s = Vector3(
				randrange(-_box.x() / 2, _box.x() / 2),
				randrange(-_box.y() / 2, _box.y() / 2),
				randrange(-_box.z() / 2, _box.z() / 2));
		}
		else { // cylinder
			float radius = randrange(0, _r), angle = randrange(0, 2 * PI);
			p.s = Vector3(
				radius * cos(angle),
				randrange(-_h / 2, _h / 2),
				radius * sin(angle));
		}

		p.v = Vector3(randrange(-1.f, 1.f), randrange(-.1f, .1f), randrange(-1.f, 1.f)).normalize() * randrange(.1f, .3f);

		p.initialSize = randrange(.01f, .03f);
		p.size = 0;
		p.initialColor = { 1.f,1.f,1.f,randrange(.7f, 1.f) };
		//Vector4 cmyk();
		Vector3 c1 = Vector3(1.6f, 1.4f, 2.f), c2 = Vector3(2.f, 1.4f, 1.6f);
		p.initialColor = ((c2 - c1) * random() + c1).to4D();
		p.initialColor = { randrange(1.f, 1.7f),randrange(1.f, 1.7f),randrange(1.f, 1.7f),1.f };
		p.color = p.initialColor;
		//p.color.setW(0);
		p.lifetime = randrange(20.f, 30.f);

		addParticle(p);
	}

	void FireflyEmitter::updateParticles(float dt) {
		for (auto p : _particles) {
			if (p->age < 0) continue;
			float rand = random();
			if (rand < 0.1f) {
				p->v = (Quaternion({ 0,1.f,0 }, randrange(.05f, .3f)).toMat() * p->v.to4D()).to3D();
			}
			else if (rand > .95f) {
				p->v = (Quaternion({ 0,1.f,0 }, -randrange(.05f, .3f)).toMat() * p->v.to4D()).to3D();

			}
			//p->v += p->a * dt;
			p->s += p->v * dt;
			p->age += dt;
			if (p->age < 2.f) p->size = (p->initialSize - 0) * p->age / 2.f + 0;
			else if (p->age > p->lifetime - 2.f) p->size = (0 - p->initialSize) * (p->age-(p->lifetime-2.f))/2.f + p->initialSize;
			else p->size = p->initialSize;
		}
	}

	void FireflyEmitter::kill(float dt) {
		for (auto p : _particles) {
			if (p->age > p->lifetime) p->age = -1;
		}
	}


	// FIRE EMITTER

	void FireEmitter::spawn(float dt) {
		Particle p;
		p.s = {};
		p.v = Vector3(randrange(-1.f, 1.f), 0, randrange(-1.f, 1.f)).normalize() * .3f;
		p.a = Vector3(0, randrange(1.f, 5.f), 0);
		p.initialSize = randrange(.5f, .8f);
		p.size = 0;
		p.lifetime = randrange(.8f, 1.5f);
		p.rot = randrange(0, 2*PI);

		addParticle(p);
	}

	void FireEmitter::updateParticles(float dt) {
		
		for (auto p : _particles) {
			if (p->age < 0) continue;
			p->age += dt;
			if (p->age > p->lifetime) p->age = p->lifetime;

			if (p->age > K1) p->color.setW(1 - (p->age - K1) / (p->lifetime - K1));

			if (p->age < K2) p->size = p->initialSize / K2 * p->age;
			else p->size = p->initialSize;

			p->v += p->a * dt;
			p->s += p->v * dt;

			/*if (p->age < 0) continue;
			float rand = random();
			if (rand < 0.1f) {
				p->v = (Quaternion({ 0,1,0 }, randrange(.05, .3f)).toMat() * p->v.to4D()).to3D();
			}
			else if (rand > .95f) {
				p->v = (Quaternion({ 0,1,0 }, -randrange(.05, .3f)).toMat() * p->v.to4D()).to3D();

			}
			//p->v += p->a * dt;
			p->s += p->v * dt;
			p->age += dt;
			if (p->age < 2.f) p->color.setW(p->initialColor.w() / 2.f * p->age);
			else if (p->age > p->lifetime - 2.f) p->color.setW(-p->initialColor.w() / 2.f * p->age + p->initialColor.w() + p->initialColor.w() / 2.f * (p->lifetime - 2.f));
			else p->color.setW(p->initialColor.w());
			*/
		}
	}

	void FireEmitter::kill(float dt) {
		for (auto p : _particles) {
			if (p->age >= p->lifetime) p->age = -1;
		}
	}

	void FireEmitter::draw(Shader* shader, const Mat4& worldMatrix, Light* light) {
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

		_texture.bind(0);
		_dissolveMap.bind(1);

		beforeDraw();
		glDepthMask(GL_FALSE);
		glUniformMatrix4fv(curr_shader->getUniform(MODEL_MATRIX), 1, GL_FALSE, newWorldMat.data());
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, (GLsizei)data.size());
		glDepthMask(GL_TRUE);
		afterDraw();

		_texture.unbind(0);
		_dissolveMap.unbind(1);

		_va.unbind();
		curr_shader->unbind();

	}

}
