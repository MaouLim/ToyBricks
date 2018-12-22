#ifndef _MATH_BASE_H_
#define _MATH_BASE_H_

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

}

#endif // _PRIMITIVES_H_
