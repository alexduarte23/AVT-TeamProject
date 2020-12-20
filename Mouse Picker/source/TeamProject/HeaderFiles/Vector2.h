#pragma once

#include <iostream>
#include <math.h>


namespace avt {

	class Vector3;
	class Vector4;

	class Vector2 {
	private:
		float _x, _y;

	public:
		Vector2(float x = 0, float y = 0)
			: _x(x), _y(y) {}

		Vector2(const Vector2& vec)
			: _x(vec.x()), _y(vec.y()) {}

		~Vector2() {}

		float x() const {
			return _x;
		}

		float y() const {
			return _y;
		}

		void setX(float x) {
			_x = x;
		}

		void setY(float y) {
			_y = y;
		}

		Vector2 clone() const {
			return Vector2(_x, _y);
		}

		Vector2& operator=(const Vector2& vec);

		Vector3 to3D(float z = 0) const;

		Vector4 to4D(float z = 0, float w = 1.f) const;

		float dot(const Vector2& vec) const;

		float length() const;

		// length squared
		float quadrance() const;

		Vector2 normalized() const;

		Vector2& normalize();

		Vector2 pow(float exp) const;

		float distanceTo(const Vector2& vec) const;

		float angleTo(const Vector2& vec) const;

		friend std::ostream& operator<<(std::ostream& os, const Vector2& vec);

		friend std::istream& operator>>(std::istream& is, Vector2& vec);

		Vector2 operator+(const Vector2& vec) const;

		Vector2 operator+(float num) const;

		Vector2 operator-(const Vector2& vec) const;

		Vector2 operator-(float num) const;

		Vector2 operator+() const;

		Vector2 operator-() const;

		bool operator==(const Vector2& vec) const;

		bool operator!=(const Vector2& vec) const;

		float operator*(const Vector2& vec) const;

		Vector2 operator*(float num) const;

		Vector2& operator+=(const Vector2& vec);

		Vector2& operator+=(float num);

		Vector2& operator-=(const Vector2& vec);

		Vector2& operator-=(float num);

		Vector2& operator*=(const Vector2& vec);

		Vector2& operator*=(float num);

		Vector2& operator/=(float num);

		Vector2 operator/(float num) const;

		friend Vector2 operator*(float num, Vector2& vec) {
			return vec * num;
		}

		friend Vector2 operator+(float num, Vector2& vec) {
			return vec + num;
		}
	};

}