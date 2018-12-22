#ifndef _MATH_BASE_H_
#define _MATH_BASE_H_

#include <cmath>

namespace tools {

	namespace precise {
		struct lowp  { };
		struct midp  { };
		struct highp { };
	}

	constexpr float pi = 3.14159265359f;

	constexpr float straight_angle = 180.0f;
	constexpr float right_angle    = 90.0f;

	constexpr float radians(float angle) {
		return pi * angle / straight_angle;
	}

	double sigmoid(double x) {
		if (0.0 < x) { return 1.0 / (1.0 + exp(-x)); }
		double exp_x = exp(x);
		return exp_x / (1 + exp_x);
	}
}

#endif // _PRIMITIVES_H_
