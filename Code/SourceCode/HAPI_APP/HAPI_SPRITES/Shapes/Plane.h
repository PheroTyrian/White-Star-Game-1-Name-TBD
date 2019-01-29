#pragma once

//#include "Vector.h"

namespace HAPISPACE {

	// very simple plane class - will be expanded later
	// A plane can be represented by a normal and an offset
	class Plane
	{
	public:
		VectorF normal;
		float offset;

		Plane() noexcept = default;

		Plane(const VectorF& norm, float off) noexcept : normal(norm), offset(off) {}
	};

}
