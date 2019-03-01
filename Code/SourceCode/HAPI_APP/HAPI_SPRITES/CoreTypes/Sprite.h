////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	Sprite.h
//
// summary:	Declares the sprite class
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Components\Components.h"

namespace HAPISPACE {

	class HardwareSprite;

	/// <summary>	holds data on auto anioation. </summary>
	struct AutoAnimate
	{
		/// <summary>	True to enable, false to disable. </summary>
		bool enabled{ false };
		/// <summary>	True to loop. </summary>
		bool loop{ true };
		/// <summary>	The update time. </summary>
		DWORD updateTime{ 0 };
		/// <summary>	The last time. </summary>
		DWORD lastTime{ 0 };
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// A sprite class that handles all blitting and frames. A sprite handles all rendering and
	/// collisions and maintains data on its transform and current frame of animation. It uses a
	/// surface and a spritesheet with a set of frames describing the surface layout.
	/// 
	/// A note on spaces:
	/// 
	/// - Texture space is the frame rectangle within the surface
	/// - Sprite space(local space) is top left 0, 0, width, height from frame rectangle and unscaled
	/// and unrotated
	/// - Origin space is sprite space translated by - origin vector
	/// - Screen space is origin space scaled, translated and rotated.
	/// </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class Sprite final
	{
	private:
		/// <summary>	The sprite sheet. </summary>
		/// <summary>	The sprite sheet. </summary>
		std::shared_ptr<SpriteSheet> m_spriteSheet;

		/// <summary>	Every sprite has a transform component. </summary>
		std::unique_ptr<ComponentTransform> m_componentTransform;

		/// <summary>	Every sprite has a collider component. </summary>
		std::unique_ptr<ComponentCollider> m_componentCollider;

		/// <summary>	A sprite can use hardware acceleration if drawn to a HW surface. </summary>
		std::unique_ptr<HardwareSprite> m_hardwareSprite;

		/// <summary>	Used when auto animate is enabled to advance frames based on time. </summary>
		AutoAnimate m_autoAnimate;

		/// <summary>	If a frame set is specified the frame number is a sequence number. </summary>
		int m_frameNumber{ 0 };

		/// <summary>	Name of the frame set. </summary>
		std::string m_frameSetName;

		/// <summary>	The blend mode. </summary>
		EBlendMode m_blendMode{ EBlendMode::eAlphaBlend };
		/// <summary>	The modulating colour. </summary>
		Colour255 m_modulatingColour{ Colour255::ZERO };

		/// <summary>	Blend function. </summary>
		std::function<void(const VectorI &scanlineStart, Colour255* destination, Colour255 const *source, int num)> *m_blendLambda{ nullptr };

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sprite space to screen space. </summary>
		///
		/// <param name="spriteSpace">	The sprite space. </param>
		///
		/// <returns>	A VectorI. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		VectorI SpriteSpaceToScreenSpace(VectorI spriteSpace) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Checks to see if the other sprite and this have collided. Does pixel perfect check. Can
		/// optionally provide a CollisionInfo that will be filled with the points of collision.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		friend class ComponentCollider;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Check pixel collision. </summary>
		///
		/// <param name="other">			The other. </param>
		/// <param name="collisionInfo">	[in,out] (Optional) If non-null, information describing the
		/// 								collision. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool CheckPixelCollision(const Sprite& other, CollisionInfo *collisionInfo = nullptr) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Checks for collision with rectangular area and this' solid pixels. </summary>
		///
		/// <param name="rectArea">			The rectangle area. </param>
		/// <param name="collisionInfo">	[in,out] (Optional) If non-null, information describing the
		/// 								collision. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool CheckPixelCollision(const RectangleI& rectArea, CollisionInfo *collisionInfo = nullptr) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// First time called with true causes the surfaces to be uploaded onto the GPU for fast
		/// rendering Subsequent changes to the surface will not be seen until called again.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void CreateHWRepresentation();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Collision functions. </summary>
		///
		/// <param name="other">			The other. </param>
		/// <param name="collisionInfo">	[in,out] (Optional) If non-null, information describing the
		/// 								collision. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool RotatedScaledCollisionCheck(const Sprite& other, CollisionInfo *collisionInfo = nullptr) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Rotated scaled collision check. </summary>
		///
		/// <param name="otherRect">		The other rectangle. </param>
		/// <param name="collisionInfo">	[in,out] (Optional) If non-null, information describing the
		/// 								collision. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool RotatedScaledCollisionCheck(const RectangleI& otherRect, CollisionInfo *collisionInfo = nullptr) const;
	public:
		// A sprite must be created with an attached spriteSheet

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="spriteSheet">	The sprite sheet. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Sprite(std::shared_ptr<SpriteSheet> spriteSheet);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	This creates a spritesheet for the surface with default values only. </summary>
		///
		/// <param name="surface">	The surface. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Sprite(std::shared_ptr<Surface> surface);

		/// <summary>	No copy constructor - to copy, create with spritesheet of other. </summary>
		~Sprite();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Render sprite's surface on to passed surface with current settings. </summary>
		///
		/// <param name="surface">	[in,out] The surface. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Render(std::shared_ptr<Surface>& surface);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Checks collision between two sprites using their colliders and the settings in the collider
		/// component Optionally can retrieve information about the collision.
		/// </summary>
		///
		/// <param name="other">			The other. </param>
		/// <param name="collisionInfo">	[in,out] (Optional) If non-null, information describing the
		/// 								collision. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool CheckCollision(const Sprite& other, CollisionInfo *collisionInfo = nullptr) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Checks collision between this sprite and a collider Optionally can retrieve information about
		/// the collision.
		/// </summary>
		///
		/// <param name="collider">				The collider. </param>
		/// <param name="colliderTransform">	The collider transform. </param>
		/// <param name="collisionInfo">		[in,out] (Optional) If non-null, information describing
		/// 									the collision. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool CheckCollision(const ColliderGroup& collider, const Transform& colliderTransform, CollisionInfo *collisionInfo = nullptr) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Joins another sprite to this one if alongX is true then other sprite frames are added to the
		/// right of this if alongX is false then other sprite frames is added below this Note: does
		/// memory allocation so potentially slow.
		/// </summary>
		///
		/// <param name="other">	The other. </param>
		/// <param name="dir">  	(Optional) The dir. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void CreateCombined(const Sprite& other, EDirection dir = EDirection::eEast);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Flips image data for each frame and creates a new frame with extension _flipHoriz Currently
		/// only supports horizontal flip (TODO)
		/// Horizontal flag is for frame layout You can specify which frame set to process (empty string
		/// for all) and a frame set name for the new frames Useful if you have a character graphic
		/// facing one way and need it to face another way as well Not for realtime use, does lots of
		/// memory allocation and creates a new spritesheet.
		/// </summary>
		///
		/// <param name="whichFrameSet">  	(Optional) Set the which frame belongs to. </param>
		/// <param name="newFrameSetName">	(Optional) Name of the new frame set. </param>
		/// <param name="horizontal">	  	(Optional) True to horizontal. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void CreateFlippedFrames(const std::string& whichFrameSet = std::string(), const std::string& newFrameSetName = std::string(), bool horizontal = false);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Set current frame number to render (and which is used for collisions)
		/// Optionally can provide a frame set name (if set in editor) meaning frame number is sequence
		/// number.
		/// </summary>
		///
		/// <param name="frameNum">	The frame number. </param>
		/// <param name="setName"> 	(Optional) Name of the set. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetFrameNumber(int frameNum, const std::string& setName = std::string());

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Moves to next frame. If previously setName has been assigned using the above function then
		/// that set is used Returns false if reached end of sequence. Irrespective of this, loop will
		/// cause an automatic loop back to the start.
		/// </summary>
		///
		/// <param name="loop">	(Optional) True to loop. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool AdvanceToNextFrame(bool loop = true);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Once called, every time render called the frame number is automatically updated (depending on
		/// frame rate)
		/// If loop is false, auto will turn off when the last frame is reached Optionally can provide a
		/// frame set name (if set in editor) meaning frame number is sequence number To manually turn
		/// off call SetFrameNumber.
		/// </summary>
		///
		/// <param name="framesPerSecond">	The frames per second. </param>
		/// <param name="loop">			  	(Optional) True to loop. </param>
		/// <param name="setName">		  	(Optional) Name of the set. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetAutoAnimate(float framesPerSecond, bool loop = true, const std::string& setName = std::string());

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Every sprite has to have a transform component. R access. </summary>
		///
		/// <returns>	The transform component. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const ComponentTransform& GetTransformComp() const { return *m_componentTransform.get(); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Every sprite has to have a transform component. RW access. </summary>
		///
		/// <returns>	The transform component. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		ComponentTransform& GetTransformComp() { return *m_componentTransform.get(); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Every sprite has a collider component. R access. </summary>
		///
		/// <returns>	The collider component. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const ComponentCollider& GetColliderComp() const { return *m_componentCollider.get(); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Every sprite has a collider component. RW access. </summary>
		///
		/// <returns>	The collider component. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		ComponentCollider& GetColliderComp() { return *m_componentCollider.get(); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Blending used in all Blit operatorions, default is alpha blend. </summary>
		///
		/// <param name="newMode">	The new mode. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetBlendMode(EBlendMode newMode) { m_blendMode = newMode; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Modulating colour used in all Blits to modulate colour prior to blending. </summary>
		///
		/// <param name="modulate">	The modulate. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetModulatingColour(Colour255 modulate) { m_modulatingColour = modulate; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Allows a blend Lambda to be set using scanlines for speed and Colour255. </summary>
		///
		/// <param name="blendLambda">	The blend lambda. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetBlendLambda(std::function<void(const VectorI &scanlineStart, Colour255* destination, Colour255 const *source, int num)> *blendLambda) { m_blendLambda = blendLambda; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Shortcut to spritesheet data. </summary>
		///
		/// <returns>	The spritesheet. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::shared_ptr<SpriteSheet> GetSpritesheet() const { return m_spriteSheet; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Access to the underlying surface used by the spritesheet. </summary>
		///
		/// <returns>	The surface. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::shared_ptr<Surface> GetSurface() const { return m_spriteSheet->GetSurface(); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Current frame number. </summary>
		///
		/// <returns>	The frame number. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		int GetFrameNumber() const noexcept { return m_frameNumber; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Current frame depends on frame number and frame set (if used) </summary>
		///
		/// <returns>	The current frame. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const Frame& GetCurrentFrame() const { return m_spriteSheet->GetFrame(m_frameNumber, m_frameSetName); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Num frames depends on frame set (if used) </summary>
		///
		/// <returns>	The number frames. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		int GetNumFrames() const { return m_spriteSheet->GetNumFrames(m_frameSetName); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Current frame rect depends on frame number and frame set (if used) </summary>
		///
		/// <returns>	The current frame rectangle. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleI GetCurrentFrameRect() const { return m_spriteSheet->GetFrameRect(m_frameNumber, m_frameSetName); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Current collider depends on frame number and frame set (if used) </summary>
		///
		/// <returns>	The current frame collider. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const ColliderGroup& GetCurrentFrameCollider() const { return m_spriteSheet->GetFrameCollider(m_frameNumber, m_frameSetName); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Current frame width. </summary>
		///
		/// <returns>	An int. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		int FrameWidth() const { return GetCurrentFrameRect().Width(); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Current frame height. </summary>
		///
		/// <returns>	An int. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		int FrameHeight() const { return GetCurrentFrameRect().Height(); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Size of current frame when rendered i.e. taking into account transform scaling.
		/// </summary>
		///
		/// <returns>	The render size. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleI GetRenderSize() const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Render normals for debug usage - potentially slow as there are lots of them.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void DebugDrawNormals() const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Saves the sprite by saving an xml spritesheet and the surface under its current filename You
		/// may wish to change the surface filename in advance.
		/// </summary>
		///
		/// <param name="filename">	The filename to save. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool Save(const std::string& filename) const;
	};
}