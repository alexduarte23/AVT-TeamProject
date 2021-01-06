#include "../HeaderFiles/Camera.h"

#include "../HeaderFiles/UniformBuffer.h"

namespace avt {

	void Camera::updateView() {
		_right = (_front.cross(_worldUp)).normalized();
		_up = _right.cross(_front);

		_viewM = Mat4{
			_right.x(), _up.x(),-_front.x(), 0.0f,
			_right.y(), _up.y(),-_front.y(), 0.0f,
			_right.z(), _up.z(),-_front.z(), 0.0f,
			-_pos.dot(_right), -_pos.dot(_up), _pos.dot(_front), 1.0f }; //column major
	}

	const Mat4& Camera::viewMatrix() const {
		return _viewM;
	}

	const Mat4& Camera::projMatrix() const {
		return _projM;
	}

	void Camera::processMove(const Vector3& movement, double deltaTime) {
		auto offsets = movement.normalized() * _moveSpeed * (float)deltaTime;

		_pos += _right * offsets.x() + _up * offsets.y() + _front * offsets.z();
		updateView();
	}

	void Camera::processMouse(const Vector2& offset , double deltaTime, bool invert) {
		float yaw = (float)offset.x() * _mouseSens * (float)deltaTime;
		float pitch = (float)offset.y() * _mouseSens * (float)deltaTime;
		int direction = invert ? -1 : 1;

		_front = _front.rotateOnAxis(_up, direction * toRad(yaw));
		_front = _front.rotateOnAxis(_right, direction * toRad(pitch));

		updateView();
	}

}