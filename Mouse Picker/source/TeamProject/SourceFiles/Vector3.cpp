#include "../HeaderFiles/Vector3.h"

#include "../HeaderFiles/Vector2.h"
#include "../HeaderFiles/Vector4.h"
#include "../HeaderFiles/Mat3.h"


namespace avt {

	Vector3::Vector3(const Vector2& vec, float z)
		: _x(vec.x()), _y(vec.y()), _z(z) {}

	float Vector3::dot(const Vector3& vec) const {
		return _x * vec.x() + _y * vec.y() + _z * vec.z();
	}

	Vector3 Vector3::cross(const Vector3& vec) const {
		float x = _y * vec.z() - _z * vec.y();
		float y = _z * vec.x() - _x * vec.z();
		float z = _x * vec.y() - _y * vec.x();

		return Vector3(x, y, z);
	}

	float Vector3::length() const {
		return std::sqrt(_x * _x + _y * _y + _z * _z);
	}

	float Vector3::quadrance() const {
		return _x * _x + _y * _y + _z * _z;
	}

	Vector3 Vector3::normalized() const {
		if (length() == 0) {
			return *this;
		}
		return *this / length();
	}

	Vector3& Vector3::normalize() {
		if (length() == 0) {
			return *this;
		}
		return *this /= length();
	}

	Vector3 Vector3::pow(float exp) const {
		return Vector3(std::pow(_x, exp), std::pow(_y, exp), std::pow(_z, exp));
	}

	float Vector3::distanceTo(const Vector3& vec) const {
		Vector3 newV(vec.x() - _x, vec.y() - _y, vec.z() - _z);
		return newV.length();
	}

	float Vector3::angleTo(const Vector3& vec) const {
		return std::acos(dot(vec) / (length() * vec.length()));
	}

	Vector3 Vector3::rotateOnAxis(const Vector3& axis, float rad) const {
		Mat3 K = Mat3::dual(axis);
		Mat3 R = Mat3::identity() + sin(rad) * K + (1 - cos(rad)) * (K * K);
		return R * *this;
	}

	Vector3 Vector3::operator+() const {
		return Vector3(_x, _y, _z);
	}

	Vector3 Vector3::operator-() const {
		return Vector3(-_x, -_y, -_z);
	}

	Vector3 Vector3::operator+(const Vector3& vec) const {
		return Vector3(_x + vec.x(), _y + vec.y(), _z + vec.z());
	}

	Vector3 Vector3::operator+(float num) const {
		return Vector3(_x + num, _y + num, _z + num);
	}

	bool Vector3::operator==(const Vector3& vec) const {
		return _x == vec.x() && _y == vec.y() && _z == vec.z();
	}

	bool Vector3::operator!=(const Vector3& vec) const {
		return _x != vec.x() || _y != vec.y() || _z != vec.z();
	}

	float Vector3::operator*(const Vector3& vec) const {
		return dot(vec);
	}

	Vector3 Vector3::operator*(float num) const {
		return Vector3(_x * num, _y * num, _z * num);
	}


	Vector3 Vector3::operator-(const Vector3& vec) const {
		return Vector3(_x - vec.x(), _y - vec.y(), _z - vec.z());
	}

	Vector3 Vector3::operator-(float num) const {
		return Vector3(_x - num, _y - num, _z - num);
	}

	Vector3& Vector3::operator+=(const Vector3& vec) {
		_x += vec.x();
		_y += vec.y();
		_z += vec.z();
		return *this;
	}

	Vector3& Vector3::operator+=(float num) {
		_x += num;
		_y += num;
		_z += num;
		return *this;
	}

	Vector3& Vector3::operator-=(const Vector3& vec) {
		_x -= vec.x();
		_y -= vec.y();
		_z -= vec.z();
		return *this;
	}

	Vector3& Vector3::operator-=(float num) {
		_x -= num;
		_y -= num;
		_z -= num;
		return *this;
	}

	Vector3& Vector3::operator*=(const Vector3& vec) {
		_x *= vec.x();
		_y *= vec.y();
		_z *= vec.z();
		return *this;
	}

	Vector3& Vector3::operator*=(float num) {
		_x *= num;
		_y *= num;
		_z *= num;
		return *this;
	}

	Vector3& Vector3::operator/=(float num) {
		_x /= num;
		_y /= num;
		_z /= num;
		return *this;
	}

	Vector3 Vector3::operator/(float num) const {
		return Vector3(_x / num, _y / num, _z / num);
	}

	Vector3& Vector3::operator=(const Vector3& vec) {
		_x = vec.x();
		_y = vec.y();
		_z = vec.z();
		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const Vector3& vec) {
		os << "(" << vec.x() << ", " << vec.y() << ", " << vec.z() << ")";
		return os;
	}

	std::istream& operator>>(std::istream& is, Vector3& vec) {
		is >> vec._x >> vec._y >> vec._z;
		return is;
	}

	Vector2 Vector3::to2D() const {
		return Vector2(_x, _y);
	}

	Vector4 Vector3::to4D(float w) const {
		return Vector4(_x, _y, _z, w);
	}


	float Vector3::quadProd(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d) {
		return (a * c) * (b * d) - (a * d) * (b * c);
	}

}