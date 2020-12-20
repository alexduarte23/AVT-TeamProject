#pragma once

#include "Matrix.h"
#include "Vector4.h"
#include "Vector3.h"


namespace avt {

	class Mat4 : public Matrix<4> {
	public:
		Mat4()
			: Matrix() {}

		Mat4(std::initializer_list<float> vals)
			: Matrix(vals) {}

		// copy constructor
		Mat4(const Mat4& mat)
			: Matrix(mat) {}

		// move constructor
		Mat4(Mat4&& mat) noexcept
			: Matrix(std::move(mat)) {}

		// copy assignment
		Mat4& operator=(const Mat4& mat);

		// move constructor
		Mat4& operator=(Mat4&& mat) noexcept;




		Mat4 operator+(const Mat4& mat) const;

		Mat4 operator+() const;

		Mat4 operator-(const Mat4& mat) const;

		Mat4 operator-() const;

		Mat4 operator*(const float num) const;

		Mat4 operator/(const float num) const;

		Mat4 operator*(const Mat4& mat) const;

		Vector4 operator*(const Vector4& vec) const;

		friend Mat4 operator+(const float num, const Mat4& mat) {
			return mat * num;
		}

		bool operator==(const Mat4& mat) const;

		bool operator!=(const Mat4& mat) const;

		Mat4& operator+=(const Mat4& mat);

		Mat4& operator-=(const Mat4& mat);

		Mat4& operator*=(const Mat4& mat);

		Mat4& operator*=(const float num);

		Mat4& operator/=(const float num);

		friend std::ostream& operator<<(std::ostream& os, const Mat4& mat);

		Mat4& transpose();

		Mat4 T() const;

		static Mat4 identity();

		static Mat4 scale(Vector3 vec);

		static Mat4 translation(Vector3 vec);

		static Mat4 rotationX(float angle); //rads

		static Mat4 rotationY(float angle); //rads

		static Mat4 rotationZ(float angle); //rads

		Mat4 inverted() const;
	};

}