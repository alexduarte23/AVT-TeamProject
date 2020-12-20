#include "../HeaderFiles/Mat3.h"

#include "../HeaderFiles/Vector3.h"


namespace avt {

	// copy assignment
	Mat3& Mat3::operator=(const Mat3& mat) {
		std::copy_n(mat.data(), LEN, _cells);
		return *this;
	}

	// move constructor
	Mat3& Mat3::operator=(Mat3&& mat) noexcept {
		delete[] _cells;
		_cells = mat.data();
		mat._cells = nullptr;
		return *this;
	}


	Mat3 Mat3::operator+(const Mat3& mat) const {
		Mat3 newM;

		for (int i = 0; i < LEN; i++) {
			newM[i] = _cells[i] + mat[i];
		}

		return newM;
	}

	Mat3 Mat3::operator+() const {
		return Mat3(*this);
	}

	Mat3 Mat3::operator-(const Mat3& mat) const {
		Mat3 newM;

		for (int i = 0; i < LEN; i++) {
			newM[i] = _cells[i] - mat[i];
		}

		return newM;
	}

	Mat3 Mat3::operator-() const {
		return *this * (-1);
	}

	Mat3 Mat3::operator*(const float num) const {
		Mat3 newM;

		for (int i = 0; i < LEN; i++) {
			newM[i] = _cells[i] * num;
		}

		return newM;

	}

	Mat3 Mat3::operator/(const float num) const {
		Mat3 newM;

		for (int i = 0; i < LEN; i++) {
			newM[i] = _cells[i] * num;
		}

		return newM;
	}

	Mat3 Mat3::operator*(const Mat3& mat) const {
		Mat3 newM;

		for (int row = 0; row < SIZE; row++) {
			for (int col = 0; col < SIZE; col++) {
				float val = 0;
				for (int k = 0; k < SIZE; k++) {
					val += at(row, k) * mat.at(k, col);
				}
				newM.at(row, col) = val;
			}
		}

		return newM;
	}

	Vector3 Mat3::operator*(const Vector3& vec) const {
		float x = _cells[0] * vec.x() + _cells[3] * vec.y() + _cells[6] * vec.z();
		float y = _cells[1] * vec.x() + _cells[4] * vec.y() + _cells[7] * vec.z();
		float z = _cells[2] * vec.x() + _cells[5] * vec.y() + _cells[8] * vec.z();

		return Vector3(x, y, z);
	}

	bool Mat3::operator==(const Mat3& mat) const {
		for (int i = 0; i < LEN; i++) {
			if (_cells[i] != mat[i])
				return false;
		}

		return true;
	}

	bool Mat3::operator!=(const Mat3& mat) const {
		for (int i = 0; i < LEN; i++) {
			if (_cells[i] != mat[i])
				return true;
		}

		return false;
	}

	Mat3& Mat3::operator+=(const Mat3& mat) {
		for (int i = 0; i < LEN; i++) {
			_cells[i] += mat[i];
		}

		return *this;
	}

	Mat3& Mat3::operator-=(const Mat3& mat) {
		for (int i = 0; i < LEN; i++) {
			_cells[i] -= mat[i];
		}

		return *this;
	}

	Mat3& Mat3::operator*=(const Mat3& mat) {
		Mat3 newM;

		for (int row = 0; row < SIZE; row++) {
			for (int col = 0; col < SIZE; col++) {
				float val = 0;
				for (int k = 0; k < SIZE; k++) {
					val += at(row, k) * mat.at(k, col);
				}
				newM.at(row, col) = val;
			}
		}

		return *this = newM;
	}

	Mat3& Mat3::operator*=(const float num) {
		for (int i = 0; i < LEN; i++) {
			_cells[i] *= num;
		}

		return *this;
	}

	Mat3& Mat3::operator/=(const float num) {
		for (int i = 0; i < LEN; i++) {
			_cells[i] /= num;
		}

		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const Mat3& mat) {
		os << "[[" << mat[0] << ", " << mat[3] << ", " << mat[6] << "]" << std::endl
			<< " [" << mat[1] << ", " << mat[4] << ", " << mat[7] << "]" << std::endl
			<< " [" << mat[2] << ", " << mat[5] << ", " << mat[8] << "]]";

		return os;
	}


	Mat3& Mat3::transpose() {
		std::swap(_cells[1], _cells[3]);
		std::swap(_cells[2], _cells[6]);
		std::swap(_cells[5], _cells[7]);
		return *this;
	}

	Mat3 Mat3::T() const {
		return Mat3{ _cells[0], _cells[3], _cells[6],
			_cells[1], _cells[4], _cells[7],
			_cells[2], _cells[5], _cells[8] };
	}

	float Mat3::det() const {
		return _cells[0] * (_cells[4] * _cells[8] - _cells[7] * _cells[5])
			- _cells[3] * (_cells[1] * _cells[8] - _cells[7] * _cells[2])
			+ _cells[6] * (_cells[1] * _cells[5] - _cells[4] * _cells[2]);
	}

	Mat3& Mat3::invert() {
		float dt = det();
		if (dt == 0) return *this;

		float multiplier = 1 / dt;

		float a = _cells[0];
		float b = _cells[3];
		float c = _cells[6];
		float d = _cells[1];
		float e = _cells[4];
		float f = _cells[7];
		float g = _cells[2];
		float h = _cells[5];
		float k = _cells[8];

		_cells[0] = e * k - f * h;
		_cells[3] = -(b * k - c * h);
		_cells[6] = b * f - c * e;
		_cells[1] = -(d * k - f * g);
		_cells[4] = a * k - c * g;
		_cells[7] = -(a * f - c * d);
		_cells[2] = d * h - e * g;
		_cells[5] = -(a * h - b * g);
		_cells[8] = a * e - b * d;

		return *this *= multiplier;

	}

	Mat3 Mat3::inverted() const {
		float dt = det();
		if (dt == 0) return *this;

		float multiplier = 1 / dt;

		Mat3 newM{
			  _cells[4] * _cells[8] - _cells[7] * _cells[5],
			-(_cells[1] * _cells[8] - _cells[7] * _cells[2]),
			  _cells[1] * _cells[5] - _cells[4] * _cells[2],
			-(_cells[3] * _cells[8] - _cells[6] * _cells[5]),
			  _cells[0] * _cells[8] - _cells[6] * _cells[2],
			-(_cells[0] * _cells[5] - _cells[3] * _cells[2]),
			  _cells[3] * _cells[7] - _cells[6] * _cells[4],
			-(_cells[0] * _cells[7] - _cells[6] * _cells[1]),
			  _cells[0] * _cells[4] - _cells[3] * _cells[1]
		};

		return newM * multiplier;

	}

	Mat3 Mat3::identity() {
		return Mat3{ 1, 0, 0, 0, 1, 0, 0, 0, 1 };
	}

	Mat3 Mat3::dual(Vector3 vec) {
		return Mat3{ 0, vec.z(), -vec.y(), -vec.z(), 0, vec.x(), vec.y(), -vec.x(), 0 };
	}

}