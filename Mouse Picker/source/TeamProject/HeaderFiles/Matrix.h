#pragma once

#include <iostream>
#include <vector>
#include <initializer_list>
#include <array>

#include <GL/glew.h>


namespace avt {

	template<unsigned int N>
	class Matrix {
	protected:
		// values stored in column major for better performance
		float* _cells;
		static const int SIZE = N;
		static const int LEN = N * N;

		Matrix()
			: _cells(new float[LEN] {0}) {}

		Matrix(std::initializer_list<float> vals)
			: _cells(new float[LEN] {0}) {
			size_t n = LEN > vals.size() ? vals.size() : LEN;
			std::copy_n(vals.begin(), n, _cells);
		}

		Matrix(const Matrix<N>& mat)
			: _cells(new float[LEN]) {

			std::copy_n(mat.data(), LEN, _cells);
		}

		Matrix(Matrix<N>&& mat) noexcept
			: _cells(mat._cells) {
			mat._cells = nullptr;
		}

		virtual ~Matrix() {
			delete[] _cells;
		}

	public:
		float operator[](int i) const {
			return _cells[i];
		}

		float& operator[](int i) {
			return _cells[i];
		}

		float* data() const {
			return _cells;
		}

		float* dataCopy() {
			float* arr = new double[N * N];
			std::copy_n(_cells, LEN, arr);
			return arr;
		}

		GLfloat* GLdata() const {
			return _cells;
		}

		float at(int row, int col) const {
			return _cells[N * col + row];
		}

		float& at(int row, int col) {
			return _cells[N * col + row];
		}

		void clean() {
			for (int i = 0; i < LEN; i++) {
				if (fabs(_cells[i]) < 1.0e-5) _cells[i] = 0.0f;
			}
		}

	};

}