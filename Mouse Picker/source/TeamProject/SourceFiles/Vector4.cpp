#include "../HeaderFiles/Vector4.h"

#include "../HeaderFiles/Vector3.h"
#include "../HeaderFiles/Vector2.h"


namespace avt {

	Vector4::Vector4(const Vector2& vec, float z, float w)
		: _x(vec.x()), _y(vec.y()), _z(z), _w(w) {}

	Vector4::Vector4(const Vector3& vec, float w)
		: _x(vec.x()), _y(vec.y()), _z(vec.z()), _w(w) {}

	float Vector4::dot(const Vector4& vec) const {
		return _x * vec.x() + _y * vec.y() + _z * vec.z() + _w * vec.w();
	}

	float Vector4::length() const {
		return std::sqrt(_x * _x + _y * _y + _z * _z + _w * _w);
	}

	float Vector4::quadrance() const {
		return _x * _x + _y * _y + _z * _z + _w * _w;
	}

	Vector4 Vector4::normalized() const {
		if (length() == 0) {
			return *this;
		}
		return *this / length();
	}

	Vector4& Vector4::normalize() {
		if (length() == 0) {
			return *this;
		}
		return *this /= length();
	}

	Vector4 Vector4::pow(float exp) const {
		return Vector4(std::pow(_x, exp), std::pow(_y, exp), std::pow(_z, exp), std::pow(_w, exp));
	}

	float Vector4::distanceTo(const Vector4& vec) const {
		Vector4 newV(vec.x() - _x, vec.y() - _y, vec.z() - _z, vec.w() - _w);
		return newV.length();
	}

	float Vector4::angleTo(const Vector4& vec) const {
		return std::acos(dot(vec) / (length() * vec.length()));
	}

	Vector4 Vector4::operator+() const {
		return Vector4(_x, _y, _z, _w);
	}

	Vector4 Vector4::operator-() const {
		return Vector4(-_x, -_y, -_z, -_w);
	}

	Vector4 Vector4::operator+(const Vector4& vec) const {
		return Vector4(_x + vec.x(), _y + vec.y(), _z + vec.z(), _w + vec.w());
	}

	Vector4 Vector4::operator+(float num) const {
		return Vector4(_x + num, _y + num, _z + num);
	}

	bool Vector4::operator==(const Vector4& vec) const {
		return _x == vec.x() && _y == vec.y() && _z == vec.z() && _w == vec.w();
	}

	bool Vector4::operator!=(const Vector4& vec) const {
		return _x != vec.x() || _y != vec.y() || _z != vec.z() || _w != vec.w();
	}

	float Vector4::operator*(const Vector4& vec) const {
		return dot(vec);
	}

	Vector4 Vector4::operator*(float num) const {
		return Vector4(_x * num, _y * num, _z * num, _w * num);
	}


	Vector4 Vector4::operator-(const Vector4& vec) const {
		return Vector4(_x - vec.x(), _y - vec.y(), _z - vec.z(), _w - vec.w());
	}

	Vector4 Vector4::operator-(float num) const {
		return Vector4(_x - num, _y - num, _z - num, _w - num);
	}

	Vector4& Vector4::operator+=(const Vector4& vec) {
		_x += vec.x();
		_y += vec.y();
		_z += vec.z();
		_w += vec.w();
		return *this;
	}

	Vector4& Vector4::operator+=(float num) {
		_x += num;
		_y += num;
		_z += num;
		_w += num;
		return *this;
	}

	Vector4& Vector4::operator-=(const Vector4& vec) {
		_x -= vec.x();
		_y -= vec.y();
		_z -= vec.z();
		_w -= vec.w();
		return *this;
	}

	Vector4& Vector4::operator-=(float num) {
		_x -= num;
		_y -= num;
		_z -= num;
		_w -= num;
		return *this;
	}

	Vector4& Vector4::operator*=(const Vector4& vec) {
		_x *= vec.x();
		_y *= vec.y();
		_z *= vec.z();
		_w *= vec.w();
		return *this;
	}

	Vector4& Vector4::operator*=(float num) {
		_x *= num;
		_y *= num;
		_z *= num;
		_w *= num;
		return *this;
	}

	Vector4& Vector4::operator/=(float num) {
		_x /= num;
		_y /= num;
		_z /= num;
		_w /= num;
		return *this;
	}

	Vector4 Vector4::operator/(float num) const {
		return Vector4(_x / num, _y / num, _z / num, _w / num);
	}

	Vector4& Vector4::operator=(const Vector4& vec) {
		_x = vec.x();
		_y = vec.y();
		_z = vec.z();
		_w = vec.w();
		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const Vector4& vec) {
		os << "(" << vec.x() << ", " << vec.y() << ", " << vec.z() << ", " << vec.w() << ")";
		return os;
	}

	std::istream& operator>>(std::istream& is, Vector4& vec) {
		is >> vec._x >> vec._y >> vec._z >> vec._w;
		return is;
	}

	Vector2 Vector4::to2D() const {
		return Vector2(_x, _y);
	}

	Vector3 Vector4::to3D() const {
		return Vector3(_x, _y, _z);
	}

}