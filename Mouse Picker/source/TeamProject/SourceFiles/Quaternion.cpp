#include "../HeaderFiles/Quaternion.h"

#include "../HeaderFiles/Vector3.h"
#include "../HeaderFiles/Vector4.h"
#include "../HeaderFiles/Mat4.h"

namespace avt {

	Quaternion::Quaternion(Vector3 axis, float rad) {
		Vector3 axisn = axis.normalized();
		Vector3 v = sin(rad / 2.0f) * axisn;

		_t = cos(rad / 2.0f);
		_x = v.x();
		_y = v.y();
		_z = v.z();

		clean();
		normalize();
	}

	Vector3 Quaternion::getAxis() const {
		Quaternion qn = normalized();
		float s = sqrt(1.0f - qn.t() * qn.t());
		if (s < THRESHOLD) {
			return Vector3(1.0f, 0, 0);
		} else {
			return Vector3(qn.x(), qn.y(), qn.z()) / s;
		}
	}

	float Quaternion::getAngle() const {
		return 2.0f * acos(_t);
	}


	void Quaternion::clean() {
		if (fabs(_t) < THRESHOLD) _t = 0.0f;
		if (fabs(_x) < THRESHOLD) _x = 0.0f;
		if (fabs(_y) < THRESHOLD) _y = 0.0f;
		if (fabs(_z) < THRESHOLD) _z = 0.0f;
	}

	float Quaternion::quadrance() const	{
		return _t * _t + _x * _x + _y * _y + _z * _z;
	}

	float Quaternion::length() const {
		return sqrt(quadrance());
	}

	void Quaternion::normalize() {
		*this /= length();
	}

	Quaternion Quaternion::normalized() const {
		return *this / length();
	}

	Quaternion Quaternion::conj() const {
		return Quaternion(_t, -_x, -_y, -_z);
	}

	Quaternion Quaternion::inv() const {
		return conj() / quadrance();
	}

	Quaternion Quaternion::operator+(const Quaternion& q) const {
		return Quaternion(_t + q.t(), _x + q.x(), _y + q.y(), _z + q.z());
	}

	Quaternion& Quaternion::operator+=(const Quaternion& q) {
		_t += q.t();
		_x += q.x();
		_y += q.y();
		_z += q.z();

		return *this;
	}

	Quaternion Quaternion::operator-(const Quaternion& q) const {
		return Quaternion(_t - q.t(), _x - q.x(), _y - q.y(), _z - q.z());
	}

	Quaternion& Quaternion::operator-=(const Quaternion& q) {
		_t -= q.t();
		_x -= q.x();
		_y -= q.y();
		_z -= q.z();

		return *this;
	}

	Quaternion Quaternion::operator*(const float s) const {
		return Quaternion(_t * s, _x * s, _y * s, _z * s);
	}

	Quaternion& Quaternion::operator*=(const float s) {
		_t *= s;
		_x *= s;
		_y *= s;
		_z *= s;

		return *this;
	}

	Quaternion Quaternion::operator/(const float s) const {
		return Quaternion(_t / s, _x / s, _y / s, _z / s);
	}

	Quaternion& Quaternion::operator/=(const float s) {
		_t /= s;
		_x /= s;
		_y /= s;
		_z /= s;

		return *this;
	}

	Quaternion Quaternion::operator*(const Quaternion& q) const {
		Quaternion res;
		res.t() = _t * q.t() - _x * q.x() - _y * q.y() - _z * q.z();
		res.x() = _t * q.x() + _x * q.t() + _y * q.z() - _z * q.y();
		res.y() = _t * q.y() + _y * q.t() + _z * q.x() - _x * q.z();
		res.z() = _t * q.z() + _z * q.t() + _x * q.y() - _y * q.x();
		return res;
	}

	Quaternion& Quaternion::operator*=(const Quaternion& q) {
		Quaternion res;
		res.t() = _t * q.t() - _x * q.x() - _y * q.y() - _z * q.z();
		res.x() = _t * q.x() + _x * q.t() + _y * q.z() - _z * q.y();
		res.y() = _t * q.y() + _y * q.t() + _z * q.x() - _x * q.z();
		res.z() = _t * q.z() + _z * q.t() + _x * q.y() - _y * q.x();
		
		_t = res.t();
		_x = res.x();
		_y = res.y();
		_z = res.z();

		return *this;
	}

	Mat4 Quaternion::toMat() const {
		Quaternion qn = normalized();
		Mat4 mat;

		float xx = qn.x() * qn.x();
		float xy = qn.x() * qn.y();
		float xz = qn.x() * qn.z();
		float xt = qn.x() * qn.t();
		float yy = qn.y() * qn.y();
		float yz = qn.y() * qn.z();
		float yt = qn.y() * qn.t();
		float zz = qn.z() * qn.z();
		float zt = qn.z() * qn.t();

		mat[0] = 1.0f - 2.0f * (yy + zz);
		mat[1] = 2.0f * (xy + zt);
		mat[2] = 2.0f * (xz - yt);
		mat[3] = 0.0f;

		mat[4] = 2.0f * (xy - zt);
		mat[5] = 1.0f - 2.0f * (xx + zz);
		mat[6] = 2.0f * (yz + xt);
		mat[7] = 0.0f;

		mat[8] = 2.0f * (xz + yt);
		mat[9] = 2.0f * (yz - xt);
		mat[10] = 1.0f - 2.0f * (xx + yy);
		mat[11] = 0.0f;

		mat[12] = 0.0f;
		mat[13] = 0.0f;
		mat[14] = 0.0f;
		mat[15] = 1.0f;

		mat.clean();
		return mat;
	}

	Quaternion Quaternion::lerp(const Quaternion& qEnd, float k) const {
		float cos_angle = _x * qEnd.x() + _y * qEnd.y() + _z * qEnd.z() + _t * qEnd.t();
		float k0 = 1.0f - k;
		float k1 = (cos_angle > 0) ? k : -k;
		
		return ((*this * k0) + (qEnd * k1)).normalized();
	}

	Quaternion Quaternion::slerp(const Quaternion& qEnd, float k) const {
		float angle = acos(_x * qEnd.x() + _y * qEnd.y() + _z * qEnd.z() + _t * qEnd.t());
		if (angle == 0) return *this;

		float k0 = sin((1 - k) * angle) / sin(angle);
		float k1 = sin(k * angle) / sin(angle);
		
		return ((*this * k0) + (qEnd * k1)).normalized();
	}

	bool Quaternion::operator==(const Quaternion& q) const {
		return (fabs(_t - q.t()) < THRESHOLD && fabs(_x - q.x()) < THRESHOLD &&
			fabs(_y - q.y()) < THRESHOLD && fabs(_z - q.z()) < THRESHOLD);
	}

	bool Quaternion::operator!=(const Quaternion& q) const {
		return (fabs(_t - q.t()) >= THRESHOLD || fabs(_x - q.x()) >= THRESHOLD ||
			fabs(_y - q.y()) >= THRESHOLD || fabs(_z - q.z()) >= THRESHOLD);
	}

}