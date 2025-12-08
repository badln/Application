#pragma once
#include "main.h"
#include "GLMmath.h"

class Quaternion {

	static glm::quat RotationBetweenVectors(Vector3 start, Vector3 dest) {
		Vector3::Normalize(&start); 
		Vector3::Normalize(&dest);

		float cosTheta = dot(start, dest);
		Vector3 rotationAxis;

		if (cosTheta < -1 + 0.001f) {
			// special case when vectors in opposite directions:
			// there is no "ideal" rotation axis
			// So guess one; any will do as long as it's perpendicular to start
			rotationAxis = cross(Vector3::YAxis(), start);
			if (rotationAxis.length() < 0.01) // bad luck, they were parallel, try again!
				rotationAxis = cross(Vector3::XAxis(), start);

			return glm::angleAxis(glm::radians(180.0f), rotationAxis.Normalized().glm());
		}

		rotationAxis = cross(start, dest);

		float s = sqrt((1 + cosTheta) * 2);
		float invs = 1 / s;

		return glm::quat(
			s * 0.5f,
			rotationAxis.x * invs,
			rotationAxis.y * invs,
			rotationAxis.z * invs
		);

	}

};