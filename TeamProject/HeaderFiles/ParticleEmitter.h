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
		float initialSize = 1;
		float rot = 0;
		float age = 0;
		float lifetime = 0;
		int type = 0;
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
		float _r = 0, _h = 0;

		void initParticles() {
			for (int i = 0; i < 200; i++)
				spawn(0);
		}

	protected:

		void kill(float dt) override;
		void updateParticles(float dt) override;
		void spawn(float dt) override;

	public:

		DustEmitter(const Vector3& spawnBox)
			: ParticleEmitter("Resources/textures/dustParticle2.png", 10000), _box(spawnBox) {
			_spawnPeriod = .003f;
			initParticles();
		}

		DustEmitter(float radius, float height)
			: ParticleEmitter("Resources/textures/dustParticle2.png", 10000), _h(height), _r(radius) {
			_spawnPeriod = .003f;
			initParticles();
		}

		~DustEmitter() {}


	};

	class FireflyEmitter : public ParticleEmitter {
	private:
		Vector3 _box;
		float _r = 0, _h = 0;

		void initParticles() {
			for (int i = 0; i < 20; i++)
				spawn(0);
		}

	protected:

		void kill(float dt) override;
		void updateParticles(float dt) override;
		void spawn(float dt) override;

	public:

		FireflyEmitter(const Vector3& spawnBox)
			: ParticleEmitter("Resources/textures/dustParticle.png", 200), _box(spawnBox) {
			_spawnPeriod = .5f;
			initParticles();
		}

		FireflyEmitter(float radius, float height)
			: ParticleEmitter("Resources/textures/dustParticle.png", 200), _h(height), _r(radius) {
			_spawnPeriod = .5f;
			initParticles();
		}

		~FireflyEmitter() {}


	};

	class FireEmitter : public ParticleEmitter {
	private:

		Texture _dissolveMap;
		static constexpr float K1 = .4f; // time to start dissolve
		static constexpr float K2 = .5f; // time to stop size grow

	protected:

		void kill(float dt) override;
		void updateParticles(float dt) override;
		void spawn(float dt) override;

	public:

		FireEmitter()
			: ParticleEmitter("Resources/textures/particleSmoke7.png", 50) {
			_spawnPeriod = .02f;
		
			_dissolveMap.setWrap(GL_REPEAT, GL_REPEAT);
			_dissolveMap.setFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
			_dissolveMap.useMipmap();
			_dissolveMap.create("Resources/textures/dissolve01.png");

			spawn(0);
		
		}

		~FireEmitter() {}

		void draw(Shader* shader, const Mat4& worldMatrix, Light* light) override;


	};

}