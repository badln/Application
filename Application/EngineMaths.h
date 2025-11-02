#ifndef MATH_H 
#define MATH_H
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include <limits>

class Math {
public:

	static float Inifinity() { return std::numeric_limits<double>::infinity(); }

	static float clamp(float v, float min, float max) {
		v = (v > min) ? v : min;
		v = (v < max) ? v : max;
		return v;
	}

	static int clamp(int v, int min, int max) {
		v = (v > min) ? v : min;
		v = (v < max) ? v : max;
		return v;
	}

};

#endif