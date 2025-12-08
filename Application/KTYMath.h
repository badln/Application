#ifndef MATH_H 
#define MATH_H
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include <limits>
#include <cmath>

class Math {
public:

	inline static const double pi = 3.141592653589793;
	inline static const double tau = 6.283185307179586;
	inline static const double goldenRatio = (glm::sqrt(5) - 1) / 2;

	static double Inifinity() { return std::numeric_limits<double>::infinity(); }

	/// <summary>
	/// Wrapper for generic cos function. (std::cos)
	/// </summary>
	/// <param name="theta"></param>
	/// <returns></returns>
	double cos(double theta) {
		return std::cos(theta);
	}
	/// <summary>
	/// Wrapper for generic sin function. (std::sin)
	/// </summary>
	/// <param name="theta"></param>
	/// <returns></returns>
	double sin(double theta) {
		return std::sin(theta);
	}
	/// <summary>
	/// Wrapper for generic acos function. (std::acos)
	/// </summary>
	/// <param name="theta"></param>
	/// <returns></returns>
	double arccos(double theta) {
		return std::acos(theta);
	}
	/// <summary>
	/// Wrapper for generic asin function. (std::asin)
	/// </summary>
	/// <param name="theta"></param>
	/// <returns></returns>
	double arcsin(double theta) {
		return std::asin(theta);
	}
	template <typename T>
	static T clamp(T v, T min, T max) {
		v = (v > min) ? v : min;
		v = (v < max) ? v : max;
		return v;
	}
	static double abs(double x) {
		if (x < 0)
			return -1 * x;
		return x;
	}
	static Vector2 abs(Vector2 x) {
		return Vector2(abs(x.x), abs(x.y));
	}
	static Vector3 abs(Vector3 x) {
		return Vector3(abs(x.x), abs(x.y), abs(x.z));
	}
	static Vector4 abs(Vector4 x) {
		return Vector4(abs(x.x), abs(x.y), abs(x.z), abs(x.w));
	}

	static double Radians(double degrees) {
		return degrees * (pi / (double)180);
	}
	static Vector2 Radians(Vector2 degrees) {
		return Vector2(
			degrees.x * (pi / (double)180),
			degrees.y * (pi / (double)180)
		);
	}
	static Vector3 Radians(Vector3 degrees) {
		return Vector3(
			degrees.x * (pi / (double)180),
			degrees.y * (pi / (double)180),
			degrees.z * (pi / (double)180)
		);
	}
	static Vector4 Radians(Vector4 degrees) {
		return Vector4(
			degrees.x * (pi / (double)180),
			degrees.y * (pi / (double)180),
			degrees.z * (pi / (double)180),
			degrees.w * (pi / (double)180)
		);
	}

	static double Degrees(double radians) {
		return radians * ((double)180 / pi);
	}
	static Vector2 Degrees(Vector2 radians) {
		return Vector2(
			radians.x * ((double)180 / pi),
			radians.y * ((double)180 / pi)
		);
	}
	static Vector3 Degrees(Vector3 radians) {
		return Vector3(
			radians.x * ((double)180 / pi),
			radians.y * ((double)180 / pi),
			radians.z * ((double)180 / pi)
		);
	}
	static Vector4 Degrees(Vector4 radians) {
		return Vector4(
			radians.x * ((double)180 / pi),
			radians.y * ((double)180 / pi),
			radians.z * ((double)180 / pi),
			radians.w * ((double)180 / pi)
		);
	}

};

#endif