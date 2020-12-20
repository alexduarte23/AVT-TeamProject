#pragma once

#include "Camera.h"
#include "avt_math.h"

namespace avt {

	class OrthographicCamera : public Camera {
	private:
		float _l;
		float _r;
		float _b;
		float _t;
		float _n;
		float _f;

		void updateProj() {
			_projM = Mat4{
				2 / (_r - _l), 0, 0, 0,
				0, 2 / (_t - _b), 0, 0,
				0, 0, 2 / (_n - _f), 0,
				(_l + _r) / (_l - _r), (_b + _t) / (_b - _t), (_n + _f) / (_n - _f), 1 }; //column major
		}

	public:
		OrthographicCamera(float left, float right, float bottom, float top, float near, float far, Vector3 pos = Vector3{}, Vector3 up = Vector3(0, 1.f, 0))
			: Camera(pos, up), _l(left), _r(right), _b(bottom), _t(top), _n(near), _f(far) {

			updateProj();
		}

		~OrthographicCamera() {}

		void setOrtho(float left, float right, float bottom, float top, float near, float far) {
			_l = left;
			_r = right;
			_b = bottom;
			_t = top;
			_n = near;
			_f = far;

			updateProj();
		}

		void resize(int w, int h) override {
			float aspect = w / (float)h;
			_b = _l / aspect;
			_t = _r / aspect;
			updateProj();
		}

		float left() const {
			return _l;
		}

		float right() const {
			return _r;
		}

		float bottom() const {
			return _b;
		}

		float top() const {
			return _t;
		}

		float near() const {
			return _n;
		}

		float far() const {
			return _f;
		}

	};

}