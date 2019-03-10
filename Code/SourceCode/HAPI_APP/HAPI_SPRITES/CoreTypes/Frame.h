#pragma once

namespace HAPISPACE {

	// Simple POD describing a frame of a spritesheet
	// In term of its area, name and collider
	class Frame final
	{
	public:

		// Construct from rectangle and name
		Frame(std::string nameIn, RectangleI rectIn, std::string setName = std::string(), int animOrder = 0) noexcept :
			name{ nameIn }, rect{ rectIn }, frameSetName(setName), setOrder(animOrder), collider{ rect.TranslatedToOrigin() } {}

		// Name empty, rectangle invalid, no set data
		Frame() noexcept = default;

		// Every frame must have a name and it must be unique in the spritesheet
		std::string name;

		// Used for grouping multiple animation sets in one spritesheet
		std::string frameSetName;
		int setOrder{ 0 };

		// Rectangular area of surface that makes up this frame in surface space
		RectangleI rect;

		// Associated collider, defaults to frame rect and circle around it
		// In frame space wiith top left 0,0
		ColliderGroup collider;

		// Convert from surface space to local frame space
		VectorI SurfaceToFrameLocal(VectorI surfaceSpace) const { return surfaceSpace - rect.TopLeft(); }

		// Convert from local frame space to surface space
		VectorI FrameLocalToSurface(VectorI localSpace) const { return localSpace + rect.TopLeft(); }

		// For editor sort
		// Only works if frames in lines with same y per row
		bool operator<(const Frame &rhs) const 
		{
			// Base on x,y pos of top left, sort across and down
			if (rect.top < rhs.rect.top)
				return true;

			if (rect.top == rhs.rect.top && rect.left < rhs.rect.left)
				return true;

			return false;
		}
	};
}