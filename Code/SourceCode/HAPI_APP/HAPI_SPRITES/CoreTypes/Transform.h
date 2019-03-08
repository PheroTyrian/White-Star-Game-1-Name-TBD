#pragma once

namespace HAPISPACE {

	// Transform information
	struct Transform
	{
		VectorF position{ 0 };
		VectorF scale{ 1 };
		VectorF origin{ 0, 0 };
		float rotation{ 0 };

		Transform() noexcept {};
		Transform(VectorF pos) noexcept : position(std::move(pos)){}

		bool IsRotated() const { return rotation != 0; }
		bool IsScaled() const { return (scale.x != 1.0f || scale.y != 1.0f); }
		void Translate(const VectorF &trans) { position += trans; }
		void Rotate(float radians) { rotation += radians; }
	};

	inline bool operator==(const Transform& lhs, const Transform& rhs)
	{
		return (lhs.position == rhs.position && lhs.scale == rhs.scale && lhs.rotation == rhs.rotation && lhs.origin == rhs.origin);
	}
}