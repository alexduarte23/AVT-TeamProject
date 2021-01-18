#pragma once

#include <iostream>
#include <math.h>


namespace avt {

	class Vector2;
	class Vector4;

	class Vector3 {
	private:
		float _x, _y, _z;

	public:
		Vector3(float x = 0, float y = 0, float z = 0)
			: _x(x), _y(y), _z(z) {}

		Vector3(const Vector3& vec)
			: _x(vec.x()), _y(vec.y()), _z(vec.z()) {}

		Vector3(const Vector2& vec, float z = 0);

		~Vector3() {}

		float x() const {
			return _x;
		}

		float y() const {
			return _y;
		}

		float z() const {
			return _z;
		}

		void setX(float x) {
			_x = x;
		}

		void setY(float y) {
			_y = y;
		}

		void setZ(float z) {
			_z = z;
		}

		Vector3 clone() const {
			return Vector3(_x, _y, _z);
		}

		Vector3& operator=(const Vector3& vec);

		Vector2 to2D() const;

		Vector4 to4D(float w = 1.f) const;

		float dot(const Vector3& vec) const;

		Vector3 cross(const Vector3& vec) const;

		float length() const;

		// length squared
		float quadrance() const;

		Vector3 normalized() const;

		Vector3& normalize();

		Vector3 pow(float exp) const;

		float distanceTo(const Vector3& vec) const;

		float angleTo(const Vector3& vec) const;

		Vector3 rotateOnAxis(const Vector3& axis, float rad) const;

		friend std::ostream& operator<<(std::ostream& os, const Vector3& vec);

		friend std::istream& operator>>(std::istream& is, Vector3& vec);

		Vector3 operator+(const Vector3& vec) const;

		Vector3 operator+(float num) const;

		Vector3 operator-(const Vector3& vec) const;

		Vector3 operator-(float num) const;

		Vector3 operator+() const;

		Vector3 operator-() const;

		bool operator==(const Vector3& vec) const;

		bool operator!=(const Vector3& vec) const;

		float operator*(const Vector3& vec) const;

		Vector3 operator*(float num) const;

		Vector3& operator+=(const Vector3& vec);

		Vector3& operator+=(float num);

		Vector3& operator-=(const Vector3& vec);

		Vector3& operator-=(float num);

		Vector3& operator*=(const Vector3& vec);

		Vector3& operator*=(float num);

		Vector3& operator/=(float num);

		Vector3 operator/(float num) const;

		friend Vector3 operator*(float num, const Vector3& vec) {
			return vec * num;
		}

		friend Vector3 operator+(float num, const Vector3& vec) {
			return vec + num;
		}

		static float quadProd(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d);
	};

}