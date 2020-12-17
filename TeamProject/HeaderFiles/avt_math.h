#pragma once

#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"

#include "Matrix.h"
#include "Mat4.h"
#include "Mat3.h"
#include "Mat2.h"

#include "Quaternion.h"

#include <cstdlib>

namespace avt {

	constexpr float PI = 3.14159265359f;

	inline float toRad(const float deg) {
		return deg * PI / 180;
	}

	inline float toDeg(const float rad) {
		return rad * 180 / PI;
	}

	inline float random() {
		return std::rand() % 10000 / 10000.0f;
	}
	
	inline float randrange(float lower, float upper) {
		return random() * (upper-lower) + lower;
	}


	// Vectors

	class Vector2;
	class Vector3;
	class Vector4;


	// Matrices

	template<unsigned int N>
	class Matrix;

	class Mat2;
	class Mat3;
	class Mat4;

	// Quaternions
	class Quaternion;

}