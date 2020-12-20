#pragma once

#include "Camera.h"
#include "avt_math.h"

namespace avt {

	class PerspectiveCamera : public Camera {
	private:
		float _fovy;
		float _aspect;
		float _n;
		float _f;

		void updateProj() {
			float d = 1 / tan(toRad(_fovy / 2));
			_projM = Mat4{
				d / _aspect, 0, 0, 0,
				0,	d, 0, 0,
				0, 0, (_n + _f) / (_n - _f), -1,
				0, 0, (2 * _n * _f) / (_n - _f), 0 }; //column major
		}

	public:
		PerspectiveCamera(float fovy, float aspect, float near, float far, Vector3 pos = Vector3{}, Vector3 up = Vector3(0,1.f,0))
			: Camera(pos, up), _fovy(fovy), _aspect(aspect), _n(near), _f(far) {

			updateProj();
		}

		~PerspectiveCamera() {}

		void setPerspective(float fovy, float aspect, float near, float far) {
			_fovy = fovy;
			_aspect = aspect;
			_n = near;
			_f = far;

			updateProj();
		}

		void resize(int w, int h) override {
			_aspect = w / (float)h;
			updateProj();
		}

		float fovy() const {
			return _fovy;
		}

		float aspect() const {
			return _aspect;
		}

		float near() const {
			return _n;
		}

		float far() const {
			return _f;
		}
	};

}