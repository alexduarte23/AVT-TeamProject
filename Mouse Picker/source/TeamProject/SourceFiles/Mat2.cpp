#include "../HeaderFiles/Mat2.h"


namespace avt {

	// copy assignment
	Mat2& Mat2::operator=(const Mat2& mat) {
		std::copy_n(mat.data(), LEN, _cells);
		return *this;
	}

	// move constructor
	Mat2& Mat2::operator=(Mat2&& mat) noexcept {
		delete[] _cells;
		_cells = mat.data();
		mat._cells = nullptr;
		return *this;
	}


	Mat2 Mat2::operator+(const Mat2& mat) const {
		Mat2 newM;
		newM[0] = _cells[0] + mat[0];
		newM[1] = _cells[1] + mat[1];
		newM[2] = _cells[2] + mat[2];
		newM[3] = _cells[3] + mat[3];

		return newM;
	}

	Mat2 Mat2::operator+() const {
		return Mat2{ _cells[0], _cells[1], _cells[2], _cells[3] };
	}

	Mat2 Mat2::operator-(const Mat2& mat) const {
		Mat2 newM;
		newM[0] = _cells[0] - mat[0];
		newM[1] = _cells[1] - mat[1];
		newM[2] = _cells[2] - mat[2];
		newM[3] = _cells[3] - mat[3];

		return newM;
	}

	Mat2 Mat2::operator-() const {
		return Mat2{ -_cells[0], -_cells[1], -_cells[2], -_cells[3] };
	}

	Mat2 Mat2::operator*(const float num) const {
		Mat2 newM;

		newM[0] = _cells[0] * num;
		newM[1] = _cells[1] * num;
		newM[2] = _cells[2] * num;
		newM[3] = _cells[3] * num;

		return newM;
	}

	Mat2 Mat2::operator/(const float num) const {
		Mat2 newM;

		newM[0] = _cells[0] / num;
		newM[1] = _cells[1] / num;
		newM[2] = _cells[2] / num;
		newM[3] = _cells[3] / num;

		return newM;
	}

	Mat2 Mat2::operator*(const Mat2& mat) const {
		Mat2 newM;

		newM[0] = _cells[0] * mat[0] + _cells[1] * mat[2];
		newM[1] = _cells[0] * mat[1] + _cells[1] * mat[3];
		newM[2] = _cells[1] * mat[0] + _cells[3] * mat[2];
		newM[3] = _cells[1] * mat[1] + _cells[3] * mat[3];

		return newM;
	}

	Vector2 Mat2::operator*(const Vector2& vec) const {
		Vector2 newV;
		newV.setX(_cells[0] * vec.x() + _cells[1] * vec.y());
		newV.setY(_cells[2] * vec.x() + _cells[3] * vec.y());

		return newV;
	}

	bool Mat2::operator==(const Mat2& mat) const {
		return _cells[0] == mat[0] && _cells[1] == mat[1]
			&& _cells[2] == mat[2] && _cells[3] == mat[3];
	}

	bool Mat2::operator!=(const Mat2& mat) const {
		return _cells[0] != mat[0] || _cells[1] != mat[1]
			&& _cells[2] != mat[2] || _cells[3] != mat[3];
	}

	Mat2& Mat2::operator+=(const Mat2& mat) {
		_cells[0] += mat[0];
		_cells[1] += mat[1];
		_cells[2] += mat[2];
		_cells[3] += mat[3];

		return *this;
	}

	Mat2& Mat2::operator-=(const Mat2& mat) {
		_cells[0] -= mat[0];
		_cells[1] -= mat[1];
		_cells[2] -= mat[2];
		_cells[3] -= mat[3];

		return *this;
	}

	Mat2& Mat2::operator*=(const Mat2& mat) {
		_cells[0] = _cells[0] * mat[0] + _cells[1] * mat[2];
		_cells[1] = _cells[0] * mat[1] + _cells[1] * mat[3];
		_cells[2] = _cells[1] * mat[0] + _cells[3] * mat[2];
		_cells[3] = _cells[1] * mat[1] + _cells[3] * mat[3];

		return *this;
	}

	Mat2& Mat2::operator*=(const float num) {
		_cells[0] *= num;
		_cells[1] *= num;
		_cells[2] *= num;
		_cells[3] *= num;

		return *this;
	}

	Mat2& Mat2::operator/=(const float num) {
		_cells[0] /= num;
		_cells[1] /= num;
		_cells[2] /= num;
		_cells[3] /= num;

		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const Mat2& mat) {
		os << "[[" << mat[0] << ", " << mat[2] << "]" << std::endl
			<< " [" << mat[1] << ", " << mat[3] << "]]";

		return os;
	}

	Mat2& Mat2::transpose() {
		std::swap(_cells[1], _cells[2]);
		return *this;
	}

	Mat2 Mat2::T() const {
		return Mat2{ _cells[0], _cells[2], _cells[1], _cells[3] };
	}

	float Mat2::det() const {
		return _cells[0] * _cells[3] - _cells[1] * _cells[2];
	}

	Mat2& Mat2::invert() {
		float dt = det();
		if (dt == 0) return *this;

		float multiplier = 1 / dt;

		std::swap(_cells[0], _cells[3]);
		_cells[1] = -_cells[1];
		_cells[2] = -_cells[2];

		return *this *= multiplier;
	}

	Mat2 Mat2::inverted() const {
		float dt = det();
		if (dt == 0) return *this;

		float multiplier = 1 / dt;

		Mat2 newM{ _cells[3], -_cells[2], -_cells[1], _cells[0] };

		return newM * multiplier;
	}

	Mat2 Mat2::identity() {
		return Mat2{ 1, 0, 0, 1 };
	}

}