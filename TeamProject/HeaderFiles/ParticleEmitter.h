#pragma once

#include <vector>
#include "avt_math.h"
#include "SceneNode.h"
#include "Texture.h"


namespace avt {
	class Light;

	struct Particle {
		Vector3 s, v, a;
		Vector4 color = { 1.f,1.f,1.f,1.f };
		Vector4 initialColor = { 1.f,1.f,1.f,1.f };
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
		void create(const std::string& texture_filename);

	protected:
		std::vector<Particle*> _particles;

		VertexBuffer _quad_vb, _instance_vb;
		VertexArray _va;
		Texture _texture;

		static const float QUAD_STRIP[5 * 4];

		double _time;
		bool _on = true;
		unsigned int _maxParticles = 700;

		float _spawnPeriod = 0; // 0 => control disabled, spawn called everytime update is called
		float _spawnRemainder = 0;


		ParticleEmitter(const std::string& texture_filename, unsigned int maxParticles) : SceneNode(), _time(0), _maxParticles(maxParticles) {
			create(texture_filename);
		}

		Particle* getUnused() {
			for (auto p : _particles) {
				if (p->age < 0) return p;
			}
			return nullptr;
		}

		virtual void kill(float dt) {}
		virtual void updateParticles(float dt) {}
		virtual void spawn(float dt) {}

		bool addParticle(const Particle& particle);

	public:

		void toggle() {
			_on = !_on;
		}

		bool on() const {
			return _on;
		}

		virtual ~ParticleEmitter () {
			for (auto p : _particles)
				delete p;
		}

		void update(double dt) {
			_time += dt;

			updateParticles((float)dt);
			if (_on) {
				if (_spawnPeriod) {
					float totalDt = _spawnRemainder + (float)dt;
					for (int i=0; i < std::floor(totalDt / _spawnPeriod); i++)
						spawn(_spawnPeriod);
					_spawnRemainder = std::fmod(totalDt, _spawnPeriod);
				}
				else {
					spawn((float)dt);
				}
			}
			kill((float)dt);
		}


		void draw(Shader* shader, const Mat4& worldMatrix, Light* light) override;

	};


	class SmokeEmitter : public ParticleEmitter {
	private:

	public:

		SmokeEmitter()
			: ParticleEmitter("Resources/textures/particleSmoke.png", 700) {
			_spawnPeriod = 0.05f;
		}

		~SmokeEmitter() {}

		
		void kill(float dt) override;
		void updateParticles(float dt) override;
		void spawn(float dt) override;

	};


	class DustEmitter : public ParticleEmitter {
	private:
		Vector3 _box;

		void createParticle() {
			Particle p;
			p.s = Vector3(
				randrange(-_box.x() / 2, _box.x() / 2), 
				randrange(-_box.y() / 2, _box.y() / 2), 
				randrange(-_box.z() / 2, _box.z() / 2));

			p.v = randVector(-1, 1).normalize() * randrange(.01f, .07f);//((Quaternion({0,0,1.f}, randrange(-PI/2,PI/2)) * Quaternion({0,1.f,0}, randrange(0,2*PI)) ).toMat() * Vector4(randrange(.2f,1.f),0,0,1.f)).to3D();

			float rand = random();
			if (rand < .01f) p.size = randrange(.025f, .03f); // bigger
			else if (rand < .07f) p.size = randrange(.01f, .02f); // medium
			else p.size = randrange(.005f, .009f); // smaller

			p.initialColor = { 1.f,1.f,1.f,randrange(.05f, .4f) };
			p.color = p.initialColor;
			p.color.setW(0);
			p.lifetime = randrange(4.f,8.f);

			addParticle(p);
		}

		void initParticles() {
			for (int i = 0; i < 200; i++)
				createParticle();

		}

	public:

		DustEmitter(const Vector3& spawnBox)
			: ParticleEmitter("Resources/textures/dustParticle.png", 10000), _box(spawnBox) {
			_spawnPeriod = .003f;
			initParticles();
		}

		~DustEmitter() {}

	protected:

		void kill(float dt) override;
		void updateParticles(float dt) override;
		void spawn(float dt) override;

	};

}