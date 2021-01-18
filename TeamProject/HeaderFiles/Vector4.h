#pragma once

#include <iostream>
#include <math.h>
#include <GL/glew.h>


namespace avt {

	class Vector2;
	class Vector3;

	class Vector4 {
	private:
		float _x, _y, _z, _w;

	public:
		Vector4(float x = 0, float y = 0, float z = 0, float w = 0)
			: _x(x), _y(y), _z(z), _w(w) {}

		Vector4(const Vector4& vec)
			: _x(vec.x()), _y(vec.y()), _z(vec.z()), _w(vec.w()) {}

		Vector4(const Vector2& vec, float z = 0, float w = 0);

		Vector4(const Vector3& vec, float w = 0);

		~Vector4() {}

		float x() const {
			return _x;
		}

		float y() const {
			return _y;
		}

		float z() const {
			return _z;
		}

		float w() const {
			return _w;
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

		void setW(float w) {
			_w = w;
		}

		Vector4 clone() const {
			return Vector4(_x, _y, _z, _w);
		}

		Vector4& operator=(const Vector4& vec);

		Vector2 to2D() const;

		Vector3 to3D() const;

		float dot(const Vector4& vec) const;

		float length() const;

		// length squared
		float quadrance() const;

		Vector4 normalized() const;

		Vector4& normalize();

		Vector4 pow(float exp) const;

		float distanceTo(const Vector4& vec) const;

		float angleTo(const Vector4& vec) const;

		friend std::ostream& operator<<(std::ostream& os, const Vector4& vec);

		friend std::istream& operator>>(std::istream& is, Vector4& vec);

		Vector4 operator+(const Vector4& vec) const;

		Vector4 operator+(float num) const;

		Vector4 operator-(const Vector4& vec) const;

		Vector4 operator-(float num) const;

		Vector4 operator+() const;

		Vector4 operator-() const;

		bool operator==(const Vector4& vec) const;

		bool operator!=(const Vector4& vec) const;

		float operator*(const Vector4& vec) const;

		Vector4 operator*(float num) const;

		Vector4& operator+=(const Vector4& vec);

		Vector4& operator+=(float num);

		Vector4& operator-=(const Vector4& vec);

		Vector4& operator-=(float num);

		Vector4& operator*=(const Vector4& vec);

		Vector4& operator*=(float num);

		Vector4& operator/=(float num);

		Vector4 operator/(float num) const;

		friend Vector4 operator*(float num, const Vector4& vec) {
			return vec * num;
		}

		friend Vector4 operator+(float num, const Vector4& vec) {
			return vec + num;
		}
	};

}


