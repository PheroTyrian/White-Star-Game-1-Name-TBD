#pragma once
#include "IComponent.h"

namespace HAPISPACE {

	class ColliderGroup;

	// Collider component used by Sprite
	class ComponentCollider :
		public IComponent
	{
	private:
		mutable CollisionSettings m_collisionSettings;
	public:
		ComponentCollider(Sprite &owner);
		~ComponentCollider() {}

		// Read access to settings for shape collision tests
		const CollisionSettings& GetCollisionSettings() const { return m_collisionSettings; }

		// Functions to chnage settings for this collider component
		
		// When a collider has a collection of 'one of' shapes, setting this to true will test those (after normal bounds checks)
		// A collision is judged to have happened if one of them collides
		void EnableOneOfTests(bool set) { m_collisionSettings.enableOneOfTests = set; }

		// Mean position and normal of a collision are calculated, requires the collisionLine to be provided (if not doing pixel perfect)
		void CalculateCollisionData(bool set) { m_collisionSettings.calculateCollisionData = set; }

		// Does not use collider shapes but the sprite surface itself (after normal bounds checks)
		void EnablePixelPerfectCollisions(bool set) { m_collisionSettings.enablePixelPerfectCollisions = set; }

		// Change all settings in one go
		void ChangeCollisionSettings(CollisionSettings newOnes) { m_collisionSettings = newOnes; }

		// Component type
		EComponentType GetType() const override final { return EComponentType::eCollider; }		

		// Determine if this COMP collider collides with other
		bool CheckCollision(const ComponentCollider& other, CollisionInfo &collisionInfo) const;

		// Determine if this COMP collider collides with raw collider group
		// Optionally return info on the collision
		bool CheckCollision(const ColliderGroup& collider, const Transform& colliderTransform, CollisionInfo &collisionInfo) const;
	
		// Read access to collider group
		const ColliderGroup& GetColliderGroup() const;

		// AA Bounding Rect for current frame
		const RectangleI GetAABoundingRect() const;

		// Bounding Circle for current frame
		const Circle GetBoundingCircle() const;		
	};
}