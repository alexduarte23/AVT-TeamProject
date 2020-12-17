#include "../HeaderFiles/Mat4.h"


namespace avt {

	// copy assignment
	Mat4& Mat4::operator=(const Mat4& mat) {
		std::copy_n(mat.data(), LEN, _cells);
		return *this;
	}

	// move constructor
	Mat4& Mat4::operator=(Mat4&& mat) noexcept {
		delete[] _cells;
		_cells = mat.data();
		mat._cells = nullptr;
		return *this;
	}


	Mat4 Mat4::operator+(const Mat4& mat) const {
		Mat4 newM;

		for (int i = 0; i < LEN; i++) {
			newM[i] = _cells[i] + mat[i];
		}

		return newM;
	}

	Mat4 Mat4::operator+() const {
		return Mat4(*this);
	}

	Mat4 Mat4::operator-(const Mat4& mat) const {
		Mat4 newM;

		for (int i = 0; i < LEN; i++) {
			newM[i] = _cells[i] - mat[i];
		}

		return newM;
	}

	Mat4 Mat4::operator-() const {
		return *this * (-1);
	}

	Mat4 Mat4::operator*(const float num) const {
		Mat4 newM;

		for (int i = 0; i < LEN; i++) {
			newM[i] = _cells[i] * num;
		}

		return newM;

	}

	Mat4 Mat4::operator/(const float num) const {
		Mat4 newM;

		for (int i = 0; i < LEN; i++) {
			newM[i] = _cells[i] * num;
		}

		return newM;
	}

	Mat4 Mat4::operator*(const Mat4& mat) const {
		Mat4 newM;

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

	Vector4 Mat4::operator*(const Vector4& vec) const {
		float x = _cells[0] * vec.x() + _cells[4] * vec.y() + _cells[8] * vec.z() + _cells[12] * vec.w();
		float y = _cells[1] * vec.x() + _cells[5] * vec.y() + _cells[9] * vec.z() + _cells[13] * vec.w();
		float z = _cells[2] * vec.x() + _cells[6] * vec.y() + _cells[10] * vec.z() + _cells[14] * vec.w();
		float w = _cells[3] * vec.x() + _cells[7] * vec.y() + _cells[11] * vec.z() + _cells[15] * vec.w();

		return Vector4(x, y, z, w);
	}

	bool Mat4::operator==(const Mat4& mat) const {
		for (int i = 0; i < LEN; i++) {
			if (_cells[i] != mat[i])
				return false;
		}

		return true;
	}

	bool Mat4::operator!=(const Mat4& mat) const {
		for (int i = 0; i < LEN; i++) {
			if (_cells[i] != mat[i])
				return true;
		}

		return false;
	}

	Mat4& Mat4::operator+=(const Mat4& mat) {
		for (int i = 0; i < LEN; i++) {
			_cells[i] += mat[i];
		}

		return *this;
	}

	Mat4& Mat4::operator-=(const Mat4& mat) {
		for (int i = 0; i < LEN; i++) {
			_cells[i] -= mat[i];
		}

		return *this;
	}

	Mat4& Mat4::operator*=(const Mat4& mat) {
		Mat4 newM;

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

	Mat4& Mat4::operator*=(const float num) {
		for (int i = 0; i < LEN; i++) {
			_cells[i] *= num;
		}

		return *this;
	}

	Mat4& Mat4::operator/=(const float num) {
		for (int i = 0; i < LEN; i++) {
			_cells[i] /= num;
		}

		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const Mat4& mat) {
		os << "[[" << mat[0] << ", " << mat[4] << ", " << mat[8] << ", " << mat[12] << "]" << std::endl
			<< " [" << mat[1] << ", " << mat[5] << ", " << mat[9] << ", " << mat[13] << "]" << std::endl
			<< " [" << mat[2] << ", " << mat[6] << ", " << mat[10] << ", " << mat[14] << "]" << std::endl
			<< " [" << mat[3] << ", " << mat[7] << ", " << mat[11] << ", " << mat[15] << "]]";

		return os;
	}

	Mat4& Mat4::transpose() {
		std::swap(_cells[1], _cells[4]);
		std::swap(_cells[2], _cells[8]);
		std::swap(_cells[3], _cells[12]);
		std::swap(_cells[6], _cells[9]);
		std::swap(_cells[7], _cells[13]);
		std::swap(_cells[11], _cells[14]);
		return *this;
	}

	Mat4 Mat4::T() const {
		return Mat4{ _cells[0], _cells[4], _cells[8], _cells[12],
			_cells[1], _cells[5], _cells[9], _cells[13],
			_cells[2], _cells[6], _cells[10], _cells[14],
			_cells[3], _cells[7], _cells[11], _cells[15] };
	}

	Mat4 Mat4::identity() {
		return Mat4{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	}

	Mat4 Mat4::scale(Vector3 vec) {
		return Mat4{ vec.x(), 0, 0, 0, 0, vec.y(), 0, 0, 0, 0, vec.z(), 0, 0, 0, 0, 1 };
	}

	Mat4 Mat4::translation(Vector3 vec) {
		return Mat4{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, vec.x(), vec.y(), vec.z(), 1 };
	}

	Mat4 Mat4::rotationX(float angle) {
		return Mat4{ 1, 0, 0, 0, 0, cos(angle), sin(angle), 0, 0, -sin(angle), cos(angle), 0, 0, 0, 0, 1 };
	}

	Mat4 Mat4::rotationY(float angle) {
		return Mat4{ cos(angle), 0, -sin(angle), 0, 0, 1, 0, 0, sin(angle), 0, cos(angle), 0, 0, 0, 0, 1 };
	}

	Mat4 Mat4::rotationZ(float angle) {
		return Mat4{ cos(angle), sin(angle), 0, 0, -sin(angle), cos(angle), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	}

	Mat4 Mat4::inverted() const
	{
		double inv[16], det;
		int i;

		inv[0] = (double)_cells[5] * (double)_cells[10] * (double)_cells[15] -
			(double)_cells[5] * (double)_cells[11] * (double)_cells[14] -
			(double)_cells[9] * (double)_cells[6] * (double)_cells[15] +
			(double)_cells[9] * (double)_cells[7] * (double)_cells[14] +
			(double)_cells[13] * (double)_cells[6] * (double)_cells[11] -
			(double)_cells[13] * (double)_cells[7] * (double)_cells[10];

		inv[4] = -(double)_cells[4] * (double)_cells[10] * (double)_cells[15] +
			(double)_cells[4] * (double)_cells[11] * (double)_cells[14] +
			(double)_cells[8] * (double)_cells[6] * (double)_cells[15] -
			(double)_cells[8] * (double)_cells[7] * (double)_cells[14] -
			(double)_cells[12] * (double)_cells[6] * (double)_cells[11] +
			(double)_cells[12] * (double)_cells[7] * (double)_cells[10];

		inv[8] = (double)_cells[4] * (double)_cells[9] * (double)_cells[15] -
			(double)_cells[4] * (double)_cells[11] * (double)_cells[13] -
			(double)_cells[8] * (double)_cells[5] * (double)_cells[15] +
			(double)_cells[8] * (double)_cells[7] * (double)_cells[13] +
			(double)_cells[12] * (double)_cells[5] * (double)_cells[11] -
			(double)_cells[12] * (double)_cells[7] * (double)_cells[9];

		inv[12] = -(double)_cells[4] * (double)_cells[9] * (double)_cells[14] +
			(double)_cells[4] * (double)_cells[10] * (double)_cells[13] +
			(double)_cells[8] * (double)_cells[5] * (double)_cells[14] -
			(double)_cells[8] * (double)_cells[6] * (double)_cells[13] -
			(double)_cells[12] * (double)_cells[5] * (double)_cells[10] +
			(double)_cells[12] * (double)_cells[6] * (double)_cells[9];

		inv[1] = -(double)_cells[1] * (double)_cells[10] * (double)_cells[15] +
			(double)_cells[1] * (double)_cells[11] * (double)_cells[14] +
			(double)_cells[9] * (double)_cells[2] * (double)_cells[15] -
			(double)_cells[9] * (double)_cells[3] * (double)_cells[14] -
			(double)_cells[13] * (double)_cells[2] * (double)_cells[11] +
			(double)_cells[13] * (double)_cells[3] * (double)_cells[10];

		inv[5] = (double)_cells[0] * (double)_cells[10] * (double)_cells[15] -
			(double)_cells[0] * (double)_cells[11] * (double)_cells[14] -
			(double)_cells[8] * (double)_cells[2] * (double)_cells[15] +
			(double)_cells[8] * (double)_cells[3] * (double)_cells[14] +
			(double)_cells[12] * (double)_cells[2] * (double)_cells[11] -
			(double)_cells[12] * (double)_cells[3] * (double)_cells[10];

		inv[9] = -(double)_cells[0] * (double)_cells[9] * (double)_cells[15] +
			(double)_cells[0] * (double)_cells[11] * (double)_cells[13] +
			(double)_cells[8] * (double)_cells[1] * (double)_cells[15] -
			(double)_cells[8] * (double)_cells[3] * (double)_cells[13] -
			(double)_cells[12] * (double)_cells[1] * (double)_cells[11] +
			(double)_cells[12] * (double)_cells[3] * (double)_cells[9];

		inv[13] = (double)_cells[0] * (double)_cells[9] * (double)_cells[14] -
			(double)_cells[0] * (double)_cells[10] * (double)_cells[13] -
			(double)_cells[8] * (double)_cells[1] * (double)_cells[14] +
			(double)_cells[8] * (double)_cells[2] * (double)_cells[13] +
			(double)_cells[12] * (double)_cells[1] * (double)_cells[10] -
			(double)_cells[12] * (double)_cells[2] * (double)_cells[9];

		inv[2] = (double)_cells[1] * (double)_cells[6] * (double)_cells[15] -
			(double)_cells[1] * (double)_cells[7] * (double)_cells[14] -
			(double)_cells[5] * (double)_cells[2] * (double)_cells[15] +
			(double)_cells[5] * (double)_cells[3] * (double)_cells[14] +
			(double)_cells[13] * (double)_cells[2] * (double)_cells[7] -
			(double)_cells[13] * (double)_cells[3] * (double)_cells[6];

		inv[6] = -(double)_cells[0] * (double)_cells[6] * (double)_cells[15] +
			(double)_cells[0] * (double)_cells[7] * (double)_cells[14] +
			(double)_cells[4] * (double)_cells[2] * (double)_cells[15] -
			(double)_cells[4] * (double)_cells[3] * (double)_cells[14] -
			(double)_cells[12] * (double)_cells[2] * (double)_cells[7] +
			(double)_cells[12] * (double)_cells[3] * (double)_cells[6];

		inv[10] = (double)_cells[0] * (double)_cells[5] * (double)_cells[15] -
			(double)_cells[0] * (double)_cells[7] * (double)_cells[13] -
			(double)_cells[4] * (double)_cells[1] * (double)_cells[15] +
			(double)_cells[4] * (double)_cells[3] * (double)_cells[13] +
			(double)_cells[12] * (double)_cells[1] * (double)_cells[7] -
			(double)_cells[12] * (double)_cells[3] * (double)_cells[5];

		inv[14] = -(double)_cells[0] * (double)_cells[5] * (double)_cells[14] +
			(double)_cells[0] * (double)_cells[6] * (double)_cells[13] +
			(double)_cells[4] * (double)_cells[1] * (double)_cells[14] -
			(double)_cells[4] * (double)_cells[2] * (double)_cells[13] -
			(double)_cells[12] * (double)_cells[1] * (double)_cells[6] +
			(double)_cells[12] * (double)_cells[2] * (double)_cells[5];

		inv[3] = -(double)_cells[1] * (double)_cells[6] * (double)_cells[11] +
			(double)_cells[1] * (double)_cells[7] * (double)_cells[10] +
			(double)_cells[5] * (double)_cells[2] * (double)_cells[11] -
			(double)_cells[5] * (double)_cells[3] * (double)_cells[10] -
			(double)_cells[9] * (double)_cells[2] * (double)_cells[7] +
			(double)_cells[9] * (double)_cells[3] * (double)_cells[6];

		inv[7] = (double)_cells[0] * (double)_cells[6] * (double)_cells[11] -
			(double)_cells[0] * (double)_cells[7] * (double)_cells[10] -
			(double)_cells[4] * (double)_cells[2] * (double)_cells[11] +
			(double)_cells[4] * (double)_cells[3] * (double)_cells[10] +
			(double)_cells[8] * (double)_cells[2] * (double)_cells[7] -
			(double)_cells[8] * (double)_cells[3] * (double)_cells[6];

		inv[11] = -(double)_cells[0] * (double)_cells[5] * (double)_cells[11] +
			(double)_cells[0] * (double)_cells[7] * (double)_cells[9] +
			(double)_cells[4] * (double)_cells[1] * (double)_cells[11] -
			(double)_cells[4] * (double)_cells[3] * (double)_cells[9] -
			(double)_cells[8] * (double)_cells[1] * (double)_cells[7] +
			(double)_cells[8] * (double)_cells[3] * (double)_cells[5];

		inv[15] = (double)_cells[0] * (double)_cells[5] * (double)_cells[10] -
			(double)_cells[0] * (double)_cells[6] * (double)_cells[9] -
			(double)_cells[4] * (double)_cells[1] * (double)_cells[10] +
			(double)_cells[4] * (double)_cells[2] * (double)_cells[9] +
			(double)_cells[8] * (double)_cells[1] * (double)_cells[6] -
			(double)_cells[8] * (double)_cells[2] * (double)_cells[5];

		det = (double)_cells[0] * inv[0] + (double)_cells[1] * inv[4] + (double)_cells[2] * inv[8] + (double)_cells[3] * inv[12];

		if (det == 0)
			return *this;

		det = 1.0 / det;

		Mat4 inverted;

		for (i = 0; i < 16; i++)
			inverted[i] = (float)(inv[i] * det);

		return inverted;
	}

}