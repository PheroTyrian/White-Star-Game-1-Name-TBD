#pragma once

#include "Shapes\Shapes.h"
#include "Transform.h"

namespace HAPISPACE {

	class Surface;

	// Information about the collision check result
	enum class ECollisionResult
	{
		eBoundingFail,
		eBoundingPass,
		eOneOfFail,
		eOneOfPass,
		ePixelPerfectFail,
		ePixelPerfectPass
	};

	// Information about the relevant test that resulted in ECollisionResult
	enum class ECollisionShapeType
	{		
		eRectRect,
		eCircleCircle,
		eRectCircle,
		eCircleRect, // as above but this circle with other rect
		ePixel
	};
	
	// Determines transform, which collision checks are carried out and what data is returned
	struct CollisionSettings
	{
		// Transforms for the two colliders involved in the collision test
		Transform thisTransform;
		Transform otherTransform;

		// When a collider has a collection of 'one of' shapes, setting this to true will test those (after normal bounds checks)
		// A collision is judged to have happened if one of them collides
		bool enableOneOfTests{ false };

		// Returns position and normal of a collision, requires the collisionLine to be provided (if not doing pixel perfect)
		bool calculateCollisionData{ false };

		// Does not use collider shapes but the sprite surface itself (after normal bounds checks)
		bool enablePixelPerfectCollisions{ false };

		// If checking via ColliderComponent this is filled in automatically for you
		// Required to calculate collision data when calculateCollisionData is true (but not when enablePixelPerfectCollisions is true)
		// Should be formed from the last position and the new position of the entity		
		LineF collisionLine;
	};	

	// Returned information about a collision test
	struct CollisionInfo
	{
		// Result of the collision
		ECollisionResult result{ ECollisionResult::eBoundingFail };

		// Type of the collision
		ECollisionShapeType type{ ECollisionShapeType::eRectRect };

		// Name of the collider in this sprite that collided
		std::string thisColliderName;
		
		// Name of the collider in other sprite that collided
		std::string otherColliderName;
		
		// The following are only provided if CollisionSettings has calculateCollisionData set to true
		// Or a pixel perfect collision is being carried out

		// Pixel on the screen where the collision occured
		VectorI screenPos;

		// Normal at the point of collision on 'other'
		VectorF normal;

		// Local position relative to the frame of 'this' where collision happened
		VectorI thisLocalPos;

		// Local position relative to the frame of 'other' where collision happened
		VectorI otherLocalPos;

		// Helper
		static std::string CollisionShapeTypeToString(ECollisionShapeType type)
		{
			switch (type)
			{
			case ECollisionShapeType::eRectRect:
				return "Rectangle v Rectangle Shape Test";
			case ECollisionShapeType::eCircleCircle:
				return "Circle v Circle Shape Test";
			case ECollisionShapeType::eRectCircle:
				return "Rectangle v Circle Shape Test";
			case ECollisionShapeType::eCircleRect:
				return "Circle v Rectangle Shape Test";
			case ECollisionShapeType::ePixel:
				return "Pixel v Pixel Test";
			default:
				break;
			}

			return "ERROR: ECollisionShapeType Unknown enum";
		}

		// Helper
		static std::string CollisionResultToString(ECollisionResult result)
		{
			switch (result)
			{
			case ECollisionResult::eBoundingFail:
				return "Bounding Fail";
			case ECollisionResult::eBoundingPass:
				return "Bounding Pass";
			case ECollisionResult::eOneOfFail:
				return "One Of Fail";
			case ECollisionResult::eOneOfPass:
				return "One Of Fail";
			case ECollisionResult::ePixelPerfectFail:
				return "Pixel Perfect Fail";
			case ECollisionResult::ePixelPerfectPass:
				return "Pixel Perfect Pass";
			default:
				break;
			}

			return "ERROR: Unknown ECollisionResult enum";
		}
	};


	// Supported collider shapes
	enum class EColliderShape
	{
		eRectangle,
		eCircle
		// polygon. capsule... TODO
	};	

	// Supported collider types
	enum class EColliderType
	{
		eBounding,	// A bounding area collider, must be intersected for a true collision response
		eOneOf,		// One or more of these collider types must be hit for a true collision response
		eMarker		// Used to determine screen positions for points or to detect areas hit etc. not part of collision result
	};

	// Colliders are all defined in local space i.e. with 0,0 top left of frame
	// They are all shapes. Pixel perfect colliisions are done via the component or sprite directly
	// Base collider class
	class Collider
	{
	protected:
		std::string m_name;
		EColliderType m_type;
		Collider() = default;

		void WriteBase(CHapiXMLNode* thisNode) const;
		bool ReadBase(CHapiXMLNode* thisNode);

		// For debug purposes can render the collider to a surface. Cannot be called direct as needs cache update.
		friend class ColliderGroup;
		virtual void Render(std::shared_ptr<Surface> &renderSurface, const Transform& transform, const Colour255& col) const = 0;
		virtual void CacheTransform(const Transform& trans) = 0;
	public:
		Collider(std::string name, EColliderType type) noexcept : m_name(name), m_type(type) {}
		virtual ~Collider() = default;

		virtual EColliderShape GetShape() const = 0;		
		virtual void WriteXML(CHapiXMLNode* parentNode) const = 0;
		
		const std::string& GetName() const { return m_name; }
		EColliderType GetType() const { return m_type; }

		// Prefer to do via ColliderGroup to maintain cache integrity
		void ChangeType(EColliderType newType) { m_type = newType; }
		void ChangeName(std::string newName) { m_name = newName; }

		virtual RectangleI GetLocalSpaceBoundingRectangle() const = 0;
	};

	// A rectangle collider
	class RectangleCollider : public Collider
	{
	private:
		RectangleI m_rect;
		// Cached result. Transformed RectangleI gives a RectangleOrientedF
		RectangleOrientedF m_transformed;	
		// Additional caching as this was taking a lot of time - monitor
		Polygon m_tranformedAsPolygon;
	protected:
		// For debug purposes can render the collider to a surface. Cannot be called direct as needs cache update.
		void Render(std::shared_ptr<Surface> &renderSurface, const Transform& transform, const Colour255& col) const override final;
		void CacheTransform(const Transform& trans) override final;
		
	public:
		// Construct from rect
		RectangleCollider(std::string name, RectangleI rect, EColliderType type) noexcept : m_rect(rect), Collider(name, type){}

		// Construct from XML
		RectangleCollider(const CHapiXML& xml, CHapiXMLNode* node);

		const RectangleI &GetRect() const { return m_rect; }

		// Prefer to do via ColliderGroup to maintain cache integrity
		void ChangeRect(RectangleI newRect) { m_rect = newRect; }

		void WriteXML(CHapiXMLNode* parentNode) const override final;

		EColliderShape GetShape() const override final { return EColliderShape::eRectangle; }		

		const RectangleOrientedF& CachedResult() const { return m_transformed; }
		const Polygon& CachedOutline() const { return m_tranformedAsPolygon; }

		RectangleI GetLocalSpaceBoundingRectangle() const override final { return m_rect; }
	};

	class CircleCollider : public Collider
	{
	private:
		Circle m_circle;
		// Cached result
		Circle m_transformed;
	protected:
		// For debug purposes can render the collider to a surface. Cannot be called direct as needs cache update.
		void Render(std::shared_ptr<Surface> &renderSurface, const Transform& transform, const Colour255& col) const override final;
		void CacheTransform(const Transform& trans) override final;
		
	public:
		CircleCollider(std::string name, Circle circ, EColliderType type) noexcept : m_circle(circ), Collider(name,type) {}

		// Construct from XML
		CircleCollider(const CHapiXML& xml, CHapiXMLNode* node);

		const Circle& GetCircle() const { return m_circle; }

		// Prefer to do via ColliderGroup to maintain cache integrity
		void ChangeCircle(Circle newCircle) { m_circle = newCircle; }

		void WriteXML(CHapiXMLNode* parentNode) const override final;

		EColliderShape GetShape() const override final { return EColliderShape::eCircle; }

		const Circle& CachedResult() const { return m_transformed; }
	
		RectangleI GetLocalSpaceBoundingRectangle() const override final { return (RectangleI)m_circle.GetEnclosingAABB(); }
	};
}
