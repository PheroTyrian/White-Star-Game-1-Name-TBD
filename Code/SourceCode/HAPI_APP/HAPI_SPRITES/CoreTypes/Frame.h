////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	Frame.h
//
// summary:	Declares the frame class
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

namespace HAPISPACE {

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Simple POD describing a frame of a spritesheet in term of its area, name and collider.
	/// </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class Frame final
	{
	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Construct from rectangle and name. </summary>
		///
		/// <param name="nameIn">   	The frame name. </param>
		/// <param name="rectIn">   	The rectangle. </param>
		/// <param name="setName">  	(Optional) Name of the animation set. </param>
		/// <param name="animOrder">	(Optional) The animation order. </param>
		/// <param name="origin">   	(Optional) The origin. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Frame(std::string nameIn, RectangleI rectIn, std::string setName = std::string(), int animOrder = 0, VectorF origin=VectorF(0,0)) noexcept :
			name{ nameIn }, rect{ rectIn }, frameSetName(setName), setOrder(animOrder), collider{ rect.TranslatedToOrigin() } {}

		/// <summary>	Name empty, rectangle invalid, no set data. </summary>
		Frame() noexcept = default;

		/// <summary>	Every frame must have a name and it must be unique in the spritesheet. </summary>
		std::string name;

		/// <summary>	Used for grouping multiple animation sets in one spritesheet. </summary>
		std::string frameSetName;

		/// <summary>	The set order (frame number). </summary>
		int setOrder{ 0 };

		/// <summary>	Rectangular area of surface that makes up this frame in surface space. </summary>
		RectangleI rect;

		/// <summary>	Each frame can have a different origin (added in V0.84). </summary>
		VectorF origin{ 0,0 };

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Associated collider, defaults to frame rect and circle around it In frame space wiith top
		/// left 0,0.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		ColliderGroup collider;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Convert from surface space to local frame space. </summary>
		///
		/// <param name="surfaceSpace">	The surface space. </param>
		///
		/// <returns>	A VectorI. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		VectorI SurfaceToFrameLocal(VectorI surfaceSpace) const { return surfaceSpace - rect.TopLeft(); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Convert from local frame space to surface space. </summary>
		///
		/// <param name="localSpace">	The local space. </param>
		///
		/// <returns>	A VectorI. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		VectorI FrameLocalToSurface(VectorI localSpace) const { return localSpace + rect.TopLeft(); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	For editor sort. Only works if frames in lines with same y per row. </summary>
		///
		/// <param name="rhs">	The frame to compare. </param>
		///
		/// <returns>	True if the first parameter is 'less than' the second. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
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