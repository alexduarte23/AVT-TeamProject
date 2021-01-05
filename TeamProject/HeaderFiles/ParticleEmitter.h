#pragma once

#include <vector>
#include "avt_math.h"
#include "SceneNode.h"
#include "Texture.h"


namespace avt {
	class Light;

	struct Particle {
		Vector3 s, v, a;
		Vector4 color;
		float size = 1;
		float rot = 0;
		float age = 0;
		float lifetime = 0;
	};

	struct ParticleBody {
		Vector3 s;
		Vector4 color;
		float size = 0;
		float rot = 0;
	};


	class ParticleEmitter : public SceneNode {
	private:
		std::vector<Particle*> _particles;

		VertexBuffer _quad_vb, _instance_vb;
		VertexArray _va;
		Texture _smokeTexture;

		double _time;
		bool _running = true;
		int _maxParticles = 700;

		float vertices[5*4] = {
			-.1f, -.1f, 0,      0,   0, // bottom left
			 .1f, -.1f, 0,    1.f,   0, // bottomm right
			-.1f,  .1f, 0,      0, 1.f, // top left
			 .1f,  .1f, 0,    1.f, 1.f  // top right
		};


		Particle* getUnused() {
			for (auto p : _particles) {
				if (p->age < 0) return p;
			}
			return nullptr;
		}

		void create();

		void updateParticles(float dt);
		void spawn(float dt);
		void kill(double dt);

	public:
		ParticleEmitter () : SceneNode(), _time(0) {
			create();
		}

		void toggle() {
			_running = !_running;
		}

		virtual ~ParticleEmitter () {
			for (auto p : _particles)
				delete p;
		}

		void update(double dt) {
			_time += dt;

			updateParticles((float)dt);
			if (_running) spawn((float)dt);
			kill((float)dt);
		}

		void draw(Shader* shader, const Mat4& worldMatrix, Light* light) override;

	};

}