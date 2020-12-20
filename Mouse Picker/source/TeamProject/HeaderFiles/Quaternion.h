#pragma once

#include <iostream>


namespace avt {

	class Vector3;
	class Vector4;
	class Mat4;

	class Quaternion {
	private:
		float _t, _x, _y, _z;

		static constexpr float THRESHOLD = (float)1.0e-5;

	public:
		Quaternion(float t=0, float x=0, float y=0, float z=0) 
			: _t(t), _x(x), _y(y), _z(z) {}

		Quaternion(Vector3 axis, float rad);

		~Quaternion() {}

		const float t() const {
			return _t;
		}

		const float x() const {
			return _x;
		}

		const float y() const {
			return _y;
		}

		const float z() const {
			return _z;
		}

		float& t() {
			return _t;
		}

		float& x() {
			return _x;
		}

		float& y() {
			return _y;
		}

		float& z() {
			return _z;
		}


		Vector3 getAxis() const;

		float getAngle() const;

		void clean();

		float quadrance() const;

		float length() const;

		void normalize();

		Quaternion normalized() const;

		Quaternion conj() const;

		Quaternion inv() const;

		Quaternion operator+(const Quaternion& q) const;

		Quaternion& operator+=(const Quaternion& q);

		Quaternion operator-(const Quaternion& q) const;

		Quaternion& operator-=(const Quaternion& q);

		Quaternion operator*(const float s) const;

		Quaternion& operator*=(const float s);

		Quaternion operator/(const float s) const;

		Quaternion& operator/=(const float s);

		Quaternion operator*(const Quaternion& q) const;

		Quaternion& operator*=(const Quaternion& q);

		Mat4 toMat() const;

		Quaternion lerp(const Quaternion& qEnd, float k) const;

		Quaternion slerp(const Quaternion& qEnd, float k) const;

		bool operator==(const Quaternion& q1) const;

		bool operator!=(const Quaternion& q1) const;

		friend std::ostream& operator<<(std::ostream& os, const Quaternion& q) {
			os << "(" << q.t() << ", " << q.x() << ", " << q.y() << ", " << q.z() << ")" << std::endl;
			return os;
		}

		friend Quaternion operator*(const float s, Quaternion& q) {
			return q * s;
		}

		friend Quaternion operator+(const float s, Quaternion& q) {
			return q + s;
		}
	};

}