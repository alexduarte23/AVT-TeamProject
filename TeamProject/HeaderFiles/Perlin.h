#pragma once

#include "avt_math.h"


namespace avt {

	class Perlin {
	private:
		static float interpolate(float a0, float a1, float w) {
			//return (a1 - a0) * w + a0;
			return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
		}

		static Vector2 randomGradient(int ix, int iy) {
			// Random float. No precomputed gradients mean this works for any number of grid coordinates
			float random = 2920.f * sin(ix * 21942.f + iy * 171324.f + 8912.f) * cos(ix * 23157.f * iy * 217832.f + 9758.f);
			Vector2 vec = { cos(random) , sin(random) };
			return vec;
		}

		static float dotGridGradient(int ix, int iy, float x, float y) {
			// Get gradient from integer coordinates
			Vector2 gradient = randomGradient(ix, iy);

			// Compute the distance vector
			float dx = x - (float)ix;
			float dy = y - (float)iy;

			// Compute the dot-product
			return (dx * gradient.x() + dy * gradient.y());
		}

	public:
		static float perlin(float x, float y) {
			// Determine grid cell coordinates
			int x0 = (int)x;
			int x1 = x0 + 1;
			int y0 = (int)y;
			int y1 = y0 + 1;

			// Determine interpolation weights
			// Could also use higher order polynomial/s-curve here
			float sx = 0.8 + x - (float)x0;
			float sy = 0.8 + y - (float)y0;

			// Interpolate between grid point gradients
			float n0, n1, ix0, ix1, value;

			n0 = dotGridGradient(x0, y0, x, y);
			n1 = dotGridGradient(x1, y0, x, y);
			ix0 = interpolate(n0, n1, sx);

			n0 = dotGridGradient(x0, y1, x, y);
			n1 = dotGridGradient(x1, y1, x, y);
			ix1 = interpolate(n0, n1, sx);

			value = interpolate(ix0, ix1, sy);
			return value;
		}
	};

}
