#pragma once

//#include "Vector.h"
//#include "Line.h"
//#include "Circle.h"

namespace HAPISPACE {

	// A ray has origin and direction to infinity
	// TODO: not fully implemented. HS uses a LineF instead.
	class Ray
	{
	public:
		VectorF origin;
		VectorF direction;

		// Default leaves values undefined
		Ray()  noexcept = default;

		// Construct with values
		// Note: dir must be normalized
		Ray(const VectorF& ori, const VectorF& dir)  noexcept : origin(ori), direction(dir) {}

		// Construct from line
		Ray(const LineI& line) noexcept : origin(line.p1), direction(line.Direction()) {}

		// Assumes direction is correctly normalized
		VectorF GetVectorAlongRay(float distance) const {
			return origin + direction * distance;
		}

		// Determines if this ray intersects the circle
		// If dist is provided the distance along the ray is returned (call GetVectorAlongRay to get actual point)
		// TODO:
		//bool Intersects(const Circle& circle, float *dist = nullptr) const;
	};
}