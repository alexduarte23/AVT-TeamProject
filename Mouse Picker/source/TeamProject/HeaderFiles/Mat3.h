#pragma once

#include "Matrix.h"

namespace avt {

	class Vector3;

	class Mat3 : public Matrix<3> {
	public:
		Mat3()
			: Matrix() {}

		Mat3(std::initializer_list<float> vals)
			: Matrix(vals) {}

		// copy constructor
		Mat3(const Mat3& mat)
			: Matrix(mat) {}

		// move constructor
		Mat3(Mat3&& mat) noexcept
			: Matrix(std::move(mat)) {}

		// copy assignment
		Mat3& operator=(const Mat3& mat);

		// move constructor
		Mat3& operator=(Mat3&& mat) noexcept;




		Mat3 operator+(const Mat3& mat) const;

		Mat3 operator+() const;

		Mat3 operator-(const Mat3& mat) const;

		Mat3 operator-() const;

		Mat3 operator*(const float num) const;

		Mat3 operator/(const float num) const;

		Mat3 operator*(const Mat3& mat) const;

		Vector3 operator*(const Vector3& vec) const;

		friend Mat3 operator*(const float num, const Mat3& mat) {
			return mat * num;
		}

		bool operator==(const Mat3& mat) const;

		bool operator!=(const Mat3& mat) const;

		Mat3& operator+=(const Mat3& mat);

		Mat3& operator-=(const Mat3& mat);

		Mat3& operator*=(const Mat3& mat);

		Mat3& operator*=(const float num);

		Mat3& operator/=(const float num);

		friend std::ostream& operator<<(std::ostream& os, const Mat3& mat);

		Mat3& transpose();

		Mat3 T() const;

		float det() const;

		Mat3& invert();

		Mat3 inverted() const;

		static Mat3 identity();

		static Mat3 dual(Vector3 vec);

	};

}