#pragma once

#include <GL/glew.h>

#include "avt_math.h"
#include "UniformBuffer.h"


namespace avt {

	class Camera {
	protected:

		static constexpr float SPEED = 3.f;
		static constexpr float SENSITIVITY = 2.5f;

		Vector3 _pos;
		Vector3 _front;
		Vector3 _up;
		Vector3 _right;
		Vector3 _worldUp;

		float _moveSpeed;
		float _mouseSens;

		Mat4 _viewM;
		Mat4 _projM;

		void updateView();

	protected:
		Camera(Vector3 pos = Vector3{}, Vector3 up = Vector3(0,1.0f,0))
			: _pos(pos), _worldUp(up), _front(0,0,-1.f), _moveSpeed(SPEED),
				_mouseSens(SENSITIVITY) {

			updateView();
		}

	public:
		virtual ~Camera() {}

		const Mat4& viewMatrix() const;

		const Mat4& projMatrix() const;

		virtual void resize(int w, int h) {
			return;
		};

		void setSensitivity(float sens) {
			_mouseSens = sens;
		}

		void setSpeed(float speed) {
			_moveSpeed = speed;
		}

		void setPosition(const Vector3& pos) {
			_pos = pos;
			updateView();
		}

		const Vector3& position() {
			return _pos;
		}

		void setUp(const Vector3& up) {
			_worldUp = up;
			updateView();
		}

		const Vector3& up() {
			return _worldUp;
		}

		void lookAt(const Vector3& target) {
			_front = (target - _pos).normalized();
			updateView();
		}

		void processMove(const Vector3& movement, double deltaTime);

		void processMouse(const Vector2& offset, double deltaTime, bool invert=false);

	};

}