#pragma once
/*
Sprite type
A sprite handles all rendering and collisions and maintains data on its transform and current frame of animation
It uses a surface and a spritesheet with a set of frames describing the surface layout

A note on spaces

- Texture space is the frame rectangle within the surface
- Sprite space (local space) is top left 0,0, width, height from frame rectangle and unscaled and unrotated
- Origin space is sprite space translated by -origin vector
- Screen space is origin space scaled, translated and rotated
*/

#include "Components\Components.h"

namespace HAPISPACE {

	class HardwareSprite;

	// holds data on auto anioation
	struct AutoAnimate
	{
		bool enabled{ false };
		bool loop{ true };
		DWORD updateTime{ 0 };
		DWORD lastTime{ 0 };
	};

	// A sprite class that handles all blitting and frames
	class Sprite final
	{
	private:
		std::shared_ptr<SpriteSheet> m_spriteSheet;

		// Every sprite has a transform component
		std::unique_ptr<ComponentTransform> m_componentTransform;

		// Every sprite has a collider component
		std::unique_ptr<ComponentCollider> m_componentCollider;

		// A sprite can use hardware acceleration if drawn to a HW surface
		std::unique_ptr<HardwareSprite> m_hardwareSprite;

		// Used when auto animate is enabled to advance frames based on time
		AutoAnimate m_autoAnimate;

		// If a frame set is specified the frame number is a sequence number
		int m_frameNumber{ 0 };
		std::string m_frameSetName;

		EBlendMode m_blendMode{ EBlendMode::eAlphaBlend };
		Colour255 m_modulatingColour{ Colour255::ZERO };

		// Blend function
		std::function<void(const VectorI &scanlineStart, Colour255* destination, Colour255 const *source, int num)> *m_blendLambda{ nullptr };

		VectorI SpriteSpaceToScreenSpace(VectorI spriteSpace) const;

		// Checks to see if the other sprite and this have collided. Does pixel perfect check.
		// Can optionally provide a CollisionInfo that will be filled with the points of collision 
		friend class ComponentCollider;
		bool CheckPixelCollision(const Sprite& other, CollisionInfo *collisionInfo = nullptr) const;

		// Checks for collision with rectangular area and this' solid pixels
		bool CheckPixelCollision(const RectangleI& rectArea, CollisionInfo *collisionInfo = nullptr) const;

		// First time called with true causes the surfaces to be uploaded onto the GPU for fast rendering
		// Subsequent changes to the surface will not be seen until called again
		void CreateHWRepresentation();

		// Collision functions
		bool RotatedScaledCollisionCheck(const Sprite& other, CollisionInfo *collisionInfo = nullptr) const;
		bool RotatedScaledCollisionCheck(const RectangleI& otherRect, CollisionInfo *collisionInfo = nullptr) const;
	public:
		// A sprite must be created with an attached spriteSheet
		Sprite(std::shared_ptr<SpriteSheet> spriteSheet);

		// This creates a spritesheet for the surface with default values only
		Sprite(std::shared_ptr<Surface> surface);

		// No copy constructor - to copy, create with spritesheet of other
		~Sprite();

		// Render sprite's surface on to passed surface with current settings
		void Render(std::shared_ptr<Surface>& surface);

		// Checks collision between two sprites using their colliders and the settings in the collider component
		// Optionally can retrieve information about the collision
		bool CheckCollision(const Sprite& other, CollisionInfo *collisionInfo = nullptr) const;

		// Checks collision between this sprite and a collider
		// Optionally can retrieve information about the collision
		bool CheckCollision(const ColliderGroup& collider, const Transform& colliderTransform, CollisionInfo *collisionInfo = nullptr) const;


		// Joins another sprite to this one
		// if alongX is true then other sprite frames are added to the right of this
		// if alongX is false then other sprite frames is added below this
		// Note: does memory allocation so potentially slow
		void CreateCombined(const Sprite& other, EDirection dir = EDirection::eEast);

		// Flips image data for each frame and creates a new frame with extension _flipHoriz
		// Currently only supports horizontal flip (TODO)
		// Horizontal flag is for frame layout
		// You can specify which frame set to process (empty string for all) and a frame set name for the new frames
		// Useful if you have a character graphic facing one way and need it to face another way as well
		// Not for realtime use, does lots of memory allocation and creates a new spritesheet
		void CreateFlippedFrames(const std::string& whichFrameSet = std::string(), const std::string& newFrameSetName = std::string(), bool horizontal = false);


		// Set current frame number to render (and which is used for collisions)
		// Optionally can provide a frame set name (if set in editor) meaning frame number is sequence number
		void SetFrameNumber(int frameNum, const std::string& setName = std::string());

		// Moves to next frame. If previously setName has been assigned using the above function then that set is used
		// Returns false if reached end of sequence. Irrespective of this, loop will cause an automatic loop back to the start.
		bool AdvanceToNextFrame(bool loop = true);

		// Once called, every time render called the frame number is automatically updated (depending on frame rate)
		// If loop is false, auto will turn off when the last frame is reached
		// Optionally can provide a frame set name (if set in editor) meaning frame number is sequence number
		// To manually turn off call SetFrameNumber
		void SetAutoAnimate(float framesPerSecond, bool loop = true, const std::string& setName = std::string());

		// Every sprite has to have a transform component. R access
		const ComponentTransform& GetTransformComp() const { return *m_componentTransform.get(); }

		// Every sprite has to have a transform component. RW access
		ComponentTransform& GetTransformComp() { return *m_componentTransform.get(); }

		// Every sprite has a collider component. R access
		const ComponentCollider& GetColliderComp() const { return *m_componentCollider.get(); }

		// Every sprite has a collider component. RW access
		ComponentCollider& GetColliderComp() { return *m_componentCollider.get(); }

		// Blending used in all Blit operatorions, default is alpha blend
		void SetBlendMode(EBlendMode newMode) { m_blendMode = newMode; }

		// Modulating colour used in all Blits to modulate colour prior to blending
		void SetModulatingColour(Colour255 modulate) { m_modulatingColour = modulate; }

		// Allows a blend Lambda to be set using scanlines for speed and Colour255
		void SetBlendLambda(std::function<void(const VectorI &scanlineStart, Colour255* destination, Colour255 const *source, int num)> *blendLambda) { m_blendLambda = blendLambda; }

		// Shortcut to spritesheet data
		std::shared_ptr<SpriteSheet> GetSpritesheet() const { return m_spriteSheet; }

		// Access to the underlying surface used by the spritesheet
		std::shared_ptr<Surface> GetSurface() const { return m_spriteSheet->GetSurface(); }

		// Current frame number
		int GetFrameNumber() const { return m_frameNumber; }

		// Current frame depends on frame number and frame set (if used)
		const Frame& GetCurrentFrame() const { return m_spriteSheet->GetFrame(m_frameNumber, m_frameSetName); }

		// Num frames depends on frame set (if used)
		int GetNumFrames() const { return m_spriteSheet->GetNumFrames(m_frameSetName); }

		// Current frame rect depends on frame number and frame set (if used)
		RectangleI GetCurrentFrameRect() const { return m_spriteSheet->GetFrameRect(m_frameNumber, m_frameSetName); }

		// Current collider depends on frame number and frame set (if used)
		const ColliderGroup& GetCurrentFrameCollider() const { return m_spriteSheet->GetFrameCollider(m_frameNumber, m_frameSetName); }

		// Current frame width
		int FrameWidth() const { return GetCurrentFrameRect().Width(); }

		// Current frame height
		int FrameHeight() const { return GetCurrentFrameRect().Height(); }

		// Size of current frame when rendered i.e. taking into account transform scaling
		RectangleI GetRenderSize() const;

		// Render normals for debug usage - potentially slow as there are lots of them
		void DebugDrawNormals() const;

		// Saves the sprite by saving an xml spritesheet and the surface under its current filename
		// You may wish to change the surface filename in advance
		bool Save(const std::string& filename) const;
	};
}