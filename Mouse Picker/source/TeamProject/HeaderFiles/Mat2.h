#pragma once

#include "Matrix.h"
#include "Vector2.h"

namespace avt {

	class Mat2 : public Matrix<2> {
	public:
		Mat2()
			: Matrix() {}

		Mat2(std::initializer_list<float> vals)
			: Matrix(vals) {}

		// copy constructor
		Mat2(const Mat2& mat)
			: Matrix(mat) {}

		// move constructor
		Mat2(Mat2&& mat) noexcept
			: Matrix(std::move(mat)) {}

		// copy assignment
		Mat2& operator=(const Mat2& mat);

		// move constructor
		Mat2& operator=(Mat2&& mat) noexcept;

		Mat2 operator+(const Mat2& mat) const;

		Mat2 operator+() const;

		Mat2 operator-(const Mat2& mat) const;

		Mat2 operator-() const;

		Mat2 operator*(const float num) const;

		Mat2 operator/(const float num) const;

		Mat2 operator*(const Mat2& mat) const;

		Vector2 operator*(const Vector2& vec) const;

		friend Mat2 operator*(const float num, const Mat2& mat) {
			return mat * num;
		}

		bool operator==(const Mat2& mat) const;

		bool operator!=(const Mat2& mat) const;

		Mat2& operator+=(const Mat2& mat);

		Mat2& operator-=(const Mat2& mat);

		Mat2& operator*=(const Mat2& mat);

		Mat2& operator*=(const float num);

		Mat2& operator/=(const float num);

		friend std::ostream& operator<<(std::ostream& os, const Mat2& mat);

		Mat2& transpose();

		Mat2 T() const;

		float det() const;

		Mat2& invert();

		Mat2 inverted() const;

		static Mat2 identity();

	};
}

