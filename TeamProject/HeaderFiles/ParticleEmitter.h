#pragma once

#include <vector>
#include "SceneNode.h"

namespace avt {

	struct Particle {
		Vector3 s, v, a;
		Vector4 color;
		float size;
		double age = 0;
		double lifetime;

		Particle(Vector3 pos, Vector4 col)
			: s(pos), color(col) {}
	};


	class ParticleEmitter : public SceneNode {
	private:
		std::vector<Particle*> _particles;

		VertexBuffer _quad_vb, _instance_vb;
		VertexArray _va;
	
		int _maxParticles = 10000;

		Vector3 vertices[6] = {
				{-.1f,-.1f,0},
				{.1f,-.1f,0},
				{-.1f,.1f,0},

				{.1f,-.1f,0},
				{.1f,.1f,0},
				{-.1f,.1f,0}
		};

		Particle* getUnused() {
			for (auto p : _particles) {
				if (p->age < 0) return p;
			}
			return nullptr;
		}

	

	protected:
		double _time;

		void init() {}

		void addParticle(Particle* p) {
			_particles.push_back(p);
		}

		void spawn(double dt) {
			if (_time <= 0.01) return;

			_time = 0;

			for (int i = 0; i < 10; i++) {
				Particle* unused = getUnused();

				if (!unused && _particles.size() >= _maxParticles) return;

				if (unused) {
					unused->s = {};
					unused->v = Vector3(randrange(-.5f, .5f), 1.f, randrange(-.5f, .5f)).normalized() * 2;
					unused->size = randrange(.9f,  1.6f);
					unused->age = 0;
					unused->lifetime = randrange(4.f, 10.f);;
				}
				else {
					Particle* p = new Particle({}, { randrange(.2f, .9f),randrange(0, .1f),randrange(0, .1f),.5f });
					p->v = Vector3(randrange(-.5f, .5f), 1.f, randrange(-.5f, .5f)).normalized() * 2;
					p->lifetime = randrange(4.f, 10.f);
					p->size = randrange(.9f,  1.6f);
					_particles.push_back(p);

				}
			}

		}

		void updateParticles(double dt) {
			for (auto p : _particles) {
				p->s += p->v * dt;
				p->age += dt;
			}
		}

		void kill(double dt) {
			for (auto p : _particles) {
				if (p->age > p->lifetime) p->age = -1;
			}
		}

	public:
		ParticleEmitter ()
			: SceneNode(), _time(0) {
			
			_va.create();

			VertexBufferLayout quad_layout;
			quad_layout.add<GLfloat>(3); // VERTICES

			_quad_vb.create(vertices, 6 * sizeof(Vector3));
			_va.addBuffer(_quad_vb, quad_layout);
			_quad_vb.unbind();

			VertexBufferLayout instance_layout;
			instance_layout.setDivisor(1);
			instance_layout.add<GLfloat>(3); // POSITIONS
			instance_layout.add<GLfloat>(4); // COLOR
			instance_layout.add<GLfloat>(1); // SIZE

			_instance_vb.create(nullptr, (long long int)_maxParticles * 8 * sizeof(float));
			_va.addBuffer(_instance_vb, instance_layout);
			_instance_vb.unbind();

			_va.unbind();

			init();
		}

		void create() {

		}

		virtual ~ParticleEmitter () {
			for (auto p : _particles)
				delete p;
		}

		void update(double dt) {
			_time += dt;

			updateParticles(dt);
			spawn(dt);
			kill(dt);
		}

		void draw(Shader* shader, const Mat4& worldMatrix) override {
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
			}
			_instance_vb.fill(data.data(), data.size() * sizeof(float));
			_instance_vb.unbind();

			beforeDraw();
			glUniformMatrix4fv(curr_shader->getUniform(MODEL_MATRIX), 1, GL_FALSE, newWorldMat.data());
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, data.size()/8);
			afterDraw();

			_va.unbind();
			curr_shader->unbind();

		}

	};

}