#pragma once

#include "Collider.h"

namespace HAPISPACE {

	// For optimisation uses results are cached in this pod
	struct CachedResults
	{
		bool dirty{ true };
		Transform transform;

		// Shortcuts to bounding shape
		// Note: every collider is guaranteed to have 1 bounding rect and 1 bounding circle (and only 1)
		// Other's must be of a different type e.g. one of
		RectangleCollider *boundingRect{ nullptr };
		CircleCollider* boundingCircle{ nullptr };

		// Avoids constant recalculation for quick outs:
		RectangleI boundingRectAABR;

		// Clear cache
		void Clear() { dirty = true; }

		// Determine if the cache 'hit' or missed
		bool CacheHit(const Transform &trans) const {
			if (dirty) return false;
			return (transform == trans);
		}
	};

	// A collection of collider shapes
	class ColliderGroup
	{
	private:
		// Each collider can contain a number of shapes to allow for composite shapes
		// All defined in local space i.e. with 0,0 top left of frame

		// Could later be a map with name as key but doubt there will be enough to cause lookup slowdowns
		std::vector<std::shared_ptr<Collider>> m_colliders;

		// Optimisation
		mutable CachedResults m_cache;

		// Carries out collision tests between shapes of the specified type
		bool CheckCollisionsForType(const CollisionSettings &settings, const ColliderGroup& other,
			EColliderType typeToCheck, bool allMustPass,
			CollisionInfo& collisionInfo) const;

		bool CalculateCollisionInfo(const CollisionSettings &settings, const std::shared_ptr<Collider>& thisCollider, 
			const std::shared_ptr<Collider>& otherCollider,
			CollisionInfo& collisionInfo) const;

		friend class SpriteSheet;
		friend class SpriteEditorToolboxWindow;
		friend class SpriteEditorCopyWindow;

		// Mainly intended for editor use. Prefer calling these rather than accessing the collider directly
		bool GetRectangle(const std::string& name, RectangleI& rect) const;
		bool GetCircle(const std::string& name, Circle& circle) const;
		bool Change(const std::string& name, RectangleI rct);
		bool Change(const std::string& name, Circle circle);
		bool ChangeType(const std::string& name, EColliderType type);
		bool ChangeName(const std::string& name, std::string newName);
		bool Translate(const std::string& name, const VectorI& trans);
		
		void AddCopyOfCollider(const std::shared_ptr<Collider>& collider);
		//void AddBase(Collider* col) { m_colliders.push_back(std::make_shared<Collider>(col)); }

		unsigned int m_autoNameCount{ 1 };

		// Check name is unique, if not (or if empty) make a unique one
		std::string GetAutoName(std::string root);

		// Transform all collider shapes
		void TransformShapes(const Transform &thisTransform) const;
		bool CheckColliderCollision(const CollisionSettings &settings, const std::shared_ptr<Collider>& thisCollider, const std::shared_ptr<Collider>& otherCollider, CollisionInfo& collisionInfo) const;
	public:
		// Must be constructed with at least a rect in local space as bounding colliders
		ColliderGroup(RectangleI rect)
		{
			// I should not have to do this but it appears Vis does not like my call to TranslatedToOrigin:
			// Frame(std::string nameIn, RectangleI rectIn) :
			//	name{ nameIn }, rect{ rectIn }, collider{ rect.TranslatedToOrigin() } {}
			// Revisit once Viz updated

			rect.TranslateToOrigin();
			Add(rect, EColliderType::eBounding, "BoundingRect");
			Add(Circle((VectorF)rect.GetCentre(), rect.GetRadius()), EColliderType::eBounding, "BoundingCircle");
		}

		// Construct from both a rectangle collider and a circle collider
		ColliderGroup(RectangleI rect, Circle circle)
		{
			Add(rect, EColliderType::eBounding, "BoundingRect");
			Add(circle, EColliderType::eBounding, "BoundingCircle");
		}

		// Copy constructor
		ColliderGroup(const ColliderGroup& other);
		
		// Move constructor
		ColliderGroup(const ColliderGroup&& other) noexcept;

		// Assignment operator
		ColliderGroup& operator=(const ColliderGroup& other);

		// Allows bounding colliders to be changed after creation , perhaps set to auto or restored
		void ChangeBoundingColliders(RectangleI rect);

		// Potentially slow
		std::vector<std::string> GetAllColliderNames() const;
		std::vector<std::string> GetColliderNamesByType(EColliderType type) const;

		// Get RW access to a collider by name, returns null if does not exist
		// Note: every collider group has a "BoundingRect" and "BoundingCircle"
		std::shared_ptr<Collider> GetColliderByName(const std::string& name) const;

		// Returns a vector filled with all rectangle colliders
		std::vector<RectangleCollider*> GetAllRectangles() const;

		// Fills a vector with all circle colliders
		std::vector<CircleCollider*> GetAllCircles() const;

		// For rectangle this is TL for circle it is centre
		// Returns 0,0 if collider not found. Check log.
		VectorI GetColliderPosition(const std::string& colliderName) const;

		// Returns 0,0 if collider not found or not rectangle. Check log.
		VectorI GetRectangleColliderSize(const std::string& colliderName) const;

		// Returns 0 if collider not found or not circle. Check log.
		float GetCircleColliderRadius(const std::string& colliderName) const;

		// Changes position of collider. For rectangles TL is changed. For circles, centre.
		// Remember colliders are defined in frame space.
		// Returns false if collider not found
		bool ChangeColliderPosition(const std::string& colliderName, VectorI newPos);

		// Changes size of rectangle collider.
		// Returns false if collider not found or it is not a rectangle
		bool ChangeColliderSize(const std::string& colliderName, VectorI newSize);

		// Changes size of circle collider.
		// Returns false if collider not found or it is not a circle
		bool ChangeColliderSize(const std::string& colliderName, float newRadius);

		// Writes collision data in XML format below the parent node
		void WriteXML(CHapiXMLNode* parentNode) const;

		// Reads collision data in XML format below the past node
		// Note: removes existing data
		bool ReadXML(const CHapiXML& xml, CHapiXMLNode* parentNode);

		// Add a local space collision rectangle of type
		// Name is optional. If not provided it is auto generated e.g. shape1, shape2...
		// For ease it is returned
		std::string Add(RectangleI rct, EColliderType type, std::string name = std::string());

		// Add a local space collision circle of type
		// Name is optional. If not provided it is auto generated e.g. shape1, shape2...
		// For ease it is returned
		std::string Add(Circle circle, EColliderType type, std::string name = std::string());

		// Remove a collider (slow)
		bool Delete(const std::string &colliderName);

		// Axis aligned bounding rect
		const RectangleI& GetBoundingAABR(const Transform& transform) const;

		// Oriented bounding rect
		RectangleOrientedF GetBoundingOBR(const Transform& transform) const;

		// Bounding circle
		Circle GetBoundingCircle(const Transform& transform) const;

		// The one collision function. How it behaves is controlled by the passed in settings
		// Information about the collision is returned in collisionInfo
		bool CheckCollision(const CollisionSettings &settings, const ColliderGroup& other,CollisionInfo& collisionInfo) const;

		// Retrieves the centre point of the marker collider type, given the transform
		// Returns false if marker not found
		bool GetMarkerCentre(const std::string& name, const Transform& transform, VectorF &centre) const;

		// Determines if a local space point is inside a certain type of collider and returns name of first found
		// If none found returns empty string
		std::string PointInsideLocalSpace(const VectorF &pnt, EColliderType type) const;

		// Determines if a local space point is inside any collider and returns name of first found
		// If none found returns empty string
		std::string PointInsideLocalSpace(const VectorF &pnt) const;

		// Determines if a local space point is inside a certain shape of collider and returns name of first found
		// If none found returns empty string
		std::string PointInsideLocalSpace(const VectorF &pnt, EColliderShape shape) const;

		// As above but returns the smallest collider the point is inside
		// This helps with user picking in the editor
		// If none found returns empty string
		std::string PointInsideSmallestLocalSpace(const VectorF &pnt) const;

		/*
			Note on these debug render functions
			For perfromance reasons you cannot directly ask a collider to draw itself, it must be done via one
			of these functions of the group (so caching refreshed)
		*/

		// Renders one collider by name
		void RenderCollider(std::shared_ptr<Surface> &renderSurface, const Transform& transform, const Colour255& col, const std::string& colliderName) const;

		// Useful for debug purposes, render the collider shape type to Surface using transform and colour
		void RenderRects(std::shared_ptr<Surface> &renderSurface, const Transform& transform, EColliderType type, const Colour255& col = Colour255::WHITE) const;
		void RenderCircles(std::shared_ptr<Surface> &renderSurface, const Transform& transform, EColliderType type, const Colour255& col = Colour255::WHITE) const;

		// As above but for all types, with colours provided for each type else default to white
		void RenderRects(std::shared_ptr<Surface> &renderSurface, const Transform& transform, const Colour255& boundsColour = Colour255::WHITE,
			const Colour255& oneOfColour = Colour255::WHITE, const Colour255& markerColou= Colour255::RED) const;

		// As above but for all types, with colours provided for each type else default to white
		void RenderCircles(std::shared_ptr<Surface> &renderSurface, const Transform& transform, const Colour255& boundsColour = Colour255::WHITE,
			const Colour255& oneOfColour = Colour255::WHITE, const Colour255& markerColou = Colour255::RED) const;

		void ClearCache() { m_cache.Clear(); }
	};
}

