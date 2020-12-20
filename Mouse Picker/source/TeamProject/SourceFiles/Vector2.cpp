#include "../HeaderFiles/Vector2.h"

#include "../HeaderFiles/Vector3.h"
#include "../HeaderFiles/Vector4.h"


namespace avt {

	float Vector2::dot(const Vector2& vec) const {
		return _x * vec.x() + _y * vec.y();
	}

	float Vector2::length() const {
		return std::sqrt(_x * _x + _y * _y);
	}

	float Vector2::quadrance() const {
		return _x * _x + _y * _y;
	}

	Vector2 Vector2::normalized() const {
		if (length() == 0) {
			return *this;
		}
		return *this / length();
	}

	Vector2& Vector2::normalize() {
		if (length() == 0) {
			return *this;
		}
		return *this /= length();
	}

	Vector2 Vector2::pow(float exp) const {
		return Vector2(std::pow(_x, exp), std::pow(_y, exp));
	}

	float Vector2::distanceTo(const Vector2& vec) const {
		Vector2 newV(vec.x() - _x, vec.y() - _y);
		return newV.length();
	}

	float Vector2::angleTo(const Vector2& vec) const {
		return std::acos(dot(vec) / (length() * vec.length()));
	}

	Vector2 Vector2::operator+() const {
		return Vector2(_x, _y);
	}

	Vector2 Vector2::operator-() const {
		return Vector2(-_x, -_y);
	}

	Vector2 Vector2::operator+(const Vector2& vec) const {
		return Vector2(_x + vec.x(), _y + vec.y());
	}

	Vector2 Vector2::operator+(float num) const {
		return Vector2(_x + num, _y + num);
	}

	bool Vector2::operator==(const Vector2& vec) const {
		return _x == vec.x() && _y == vec.y();
	}

	bool Vector2::operator!=(const Vector2& vec) const {
		return _x != vec.x() || _y != vec.y();
	}

	float Vector2::operator*(const Vector2& vec) const {
		return dot(vec);
	}

	Vector2 Vector2::operator*(float num) const {
		return Vector2(_x * num, _y * num);
	}


	Vector2 Vector2::operator-(const Vector2& vec) const {
		return Vector2(_x - vec.x(), _y - vec.y());
	}

	Vector2 Vector2::operator-(float num) const {
		return Vector2(_x - num, _y - num);
	}

	Vector2& Vector2::operator+=(const Vector2& vec) {
		_x += vec.x();
		_y += vec.y();
		return *this;
	}

	Vector2& Vector2::operator+=(float num) {
		_x += num;
		_y += num;
		return *this;
	}

	Vector2& Vector2::operator-=(const Vector2& vec) {
		_x -= vec.x();
		_y -= vec.y();
		return *this;
	}

	Vector2& Vector2::operator-=(float num) {
		_x -= num;
		_y -= num;
		return *this;
	}

	Vector2& Vector2::operator*=(const Vector2& vec) {
		_x *= vec.x();
		_y *= vec.y();
		return *this;
	}

	Vector2& Vector2::operator*=(float num) {
		_x *= num;
		_y *= num;
		return *this;
	}

	Vector2& Vector2::operator/=(float num) {
		_x /= num;
		_y /= num;
		return *this;
	}

	Vector2 Vector2::operator/(float num) const {
		return Vector2(_x / num, _y / num);
	}

	Vector2& Vector2::operator=(const Vector2& vec) {
		_x = vec.x();
		_y = vec.y();
		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const Vector2& vec) {
		os << "(" << vec.x() << ", " << vec.y() << ")";
		return os;
	}

	std::istream& operator>>(std::istream& is, Vector2& vec) {
		is >> vec._x >> vec._y;
		return is;
	}

	Vector3 Vector2::to3D(float z) const {
		return Vector3(_x, _y, z);
	}

	Vector4 Vector2::to4D(float z, float w) const {
		return Vector4(_x, _y, z, w);
	}

}