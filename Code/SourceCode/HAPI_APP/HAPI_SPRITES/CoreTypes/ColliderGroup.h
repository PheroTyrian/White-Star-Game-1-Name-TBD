////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	ColliderGroup.h
//
// summary:	Declares the collider group class
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Collider.h"

namespace HAPISPACE {

	/// <summary>	For optimisation uses results are cached in this pod. </summary>
	struct CachedResults
	{
		/// <summary>	True to dirty. </summary>
		bool dirty{ true };
		/// <summary>	The transform. </summary>
		Transform transform;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Shortcuts to bounding shape Note: every collider is guaranteed to have 1 bounding rect and 1
		/// bounding circle (and only 1)
		/// Other's must be of a different type e.g. one of.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleCollider *boundingRect{ nullptr };
		/// <summary>	The bounding circle. </summary>
		CircleCollider* boundingCircle{ nullptr };

		/// <summary>	Avoids constant recalculation for quick outs: </summary>
		RectangleI boundingRectAABR;

		/// <summary>	Clear cache. </summary>
		void Clear() { dirty = true; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Determine if the cache 'hit' or missed. </summary>
		///
		/// <param name="trans">	The transform. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool CacheHit(const Transform &trans) const {
			if (dirty) return false;
			return (transform == trans);
		}
	};

	/// <summary>	A collection of collider shapes. </summary>
	class ColliderGroup
	{
	private:
		// Each collider can contain a number of shapes to allow for composite shapes
		// All defined in local space i.e. with 0,0 top left of frame

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Could later be a map with name as key but doubt there will be enough to cause lookup
		/// slowdowns.
		/// </summary>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::vector<std::shared_ptr<Collider>> m_colliders;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Optimisation. </summary>
		///
		/// <value>	The m cache. </value>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		mutable CachedResults m_cache;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Carries out collision tests between shapes of the specified type. </summary>
		///
		/// <param name="settings">			Options for controlling the operation. </param>
		/// <param name="other">			The other. </param>
		/// <param name="typeToCheck">  	The type to check. </param>
		/// <param name="allMustPass">  	True to all must pass. </param>
		/// <param name="collisionInfo">	[in,out] Information describing the collision. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool CheckCollisionsForType(const CollisionSettings &settings, const ColliderGroup& other,
			EColliderType typeToCheck, bool allMustPass,
			CollisionInfo& collisionInfo) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Calculates the collision information. </summary>
		///
		/// <param name="settings">			Options for controlling the operation. </param>
		/// <param name="thisCollider"> 	this collider. </param>
		/// <param name="otherCollider">	The other collider. </param>
		/// <param name="collisionInfo">	[in,out] Information describing the collision. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool CalculateCollisionInfo(const CollisionSettings &settings, const std::shared_ptr<Collider>& thisCollider, 
			const std::shared_ptr<Collider>& otherCollider,
			CollisionInfo& collisionInfo) const;

		/// <summary>	A sprite sheet. </summary>
		friend class SpriteSheet;
		/// <summary>	Form for viewing the sprite editor toolbox. </summary>
		friend class SpriteEditorToolboxWindow;
		/// <summary>	Form for viewing the sprite editor copy. </summary>
		friend class SpriteEditorCopyWindow;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Mainly intended for editor use. Prefer calling these rather than accessing the collider
		/// directly.
		/// </summary>
		///
		/// <param name="name">	The name. </param>
		/// <param name="rect">	[in,out] The rectangle. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GetRectangle(const std::string& name, RectangleI& rect) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a circle. </summary>
		///
		/// <param name="name">  	The name. </param>
		/// <param name="circle">	[in,out] The circle. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GetCircle(const std::string& name, Circle& circle) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Changes. </summary>
		///
		/// <param name="name">	The name. </param>
		/// <param name="rct"> 	The rectangle. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool Change(const std::string& name, RectangleI rct);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Changes. </summary>
		///
		/// <param name="name">  	The name. </param>
		/// <param name="circle">	The circle. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool Change(const std::string& name, Circle circle);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Change type. </summary>
		///
		/// <param name="name">	The name. </param>
		/// <param name="type">	The type. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool ChangeType(const std::string& name, EColliderType type);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Change name. </summary>
		///
		/// <param name="name">   	The name. </param>
		/// <param name="newName">	Name of the new. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool ChangeName(const std::string& name, std::string newName);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Translates. </summary>
		///
		/// <param name="name"> 	The name. </param>
		/// <param name="trans">	The transform. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool Translate(const std::string& name, const VectorI& trans);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adds a copy of collider. </summary>
		///
		/// <param name="collider">	The collider. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void AddCopyOfCollider(const std::shared_ptr<Collider>& collider);
		//void AddBase(Collider* col) { m_colliders.push_back(std::make_shared<Collider>(col)); }

		/// <summary>	Number of automatic names. </summary>
		unsigned int m_autoNameCount{ 1 };

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Check name is unique, if not (or if empty) make a unique one. </summary>
		///
		/// <param name="root">	The root. </param>
		///
		/// <returns>	The automatic name. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::string GetAutoName(std::string root);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Transform all collider shapes. </summary>
		///
		/// <param name="thisTransform">	this transform. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void TransformShapes(const Transform &thisTransform) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Check collider collision. </summary>
		///
		/// <param name="settings">			Options for controlling the operation. </param>
		/// <param name="thisCollider"> 	this collider. </param>
		/// <param name="otherCollider">	The other collider. </param>
		/// <param name="collisionInfo">	[in,out] Information describing the collision. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool CheckColliderCollision(const CollisionSettings &settings, const std::shared_ptr<Collider>& thisCollider, const std::shared_ptr<Collider>& otherCollider, CollisionInfo& collisionInfo) const;
	public:
		// Must be constructed with at least a rect in local space as bounding colliders

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="rect">	The rectangle. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
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

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Construct from both a rectangle collider and a circle collider. </summary>
		///
		/// <param name="rect">  	The rectangle. </param>
		/// <param name="circle">	The circle. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		ColliderGroup(RectangleI rect, Circle circle)
		{
			Add(rect, EColliderType::eBounding, "BoundingRect");
			Add(circle, EColliderType::eBounding, "BoundingCircle");
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Copy constructor. </summary>
		///
		/// <param name="other">	The other. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		ColliderGroup(const ColliderGroup& other);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Move constructor. </summary>
		///
		/// <param name="other">	The other. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		ColliderGroup(const ColliderGroup&& other) noexcept;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Assignment operator. </summary>
		///
		/// <param name="other">	The other. </param>
		///
		/// <returns>	A shallow copy of this object. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		ColliderGroup& operator=(const ColliderGroup& other);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Allows bounding colliders to be changed after creation , perhaps set to auto or restored.
		/// </summary>
		///
		/// <param name="rect">	The rectangle. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ChangeBoundingColliders(RectangleI rect);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Potentially slow. </summary>
		///
		/// <returns>	all collider names. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::vector<std::string> GetAllColliderNames() const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets collider names by type. </summary>
		///
		/// <param name="type">	The type. </param>
		///
		/// <returns>	The collider names by type. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::vector<std::string> GetColliderNamesByType(EColliderType type) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Get RW access to a collider by name, returns null if does not exist Note: every collider
		/// group has a "BoundingRect" and "BoundingCircle".
		/// </summary>
		///
		/// <param name="name">	The name. </param>
		///
		/// <returns>	The collider by name. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::shared_ptr<Collider> GetColliderByName(const std::string& name) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns a vector filled with all rectangle colliders. </summary>
		///
		/// <returns>	Null if it fails, else all rectangles. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::vector<RectangleCollider*> GetAllRectangles() const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Fills a vector with all circle colliders. </summary>
		///
		/// <returns>	Null if it fails, else all circles. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::vector<CircleCollider*> GetAllCircles() const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// For rectangle this is TL for circle it is centre Returns 0,0 if collider not found. Check log.
		/// </summary>
		///
		/// <param name="colliderName">	Name of the collider. </param>
		///
		/// <returns>	The collider position. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		VectorI GetColliderPosition(const std::string& colliderName) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns 0,0 if collider not found or not rectangle. Check log. </summary>
		///
		/// <param name="colliderName">	Name of the collider. </param>
		///
		/// <returns>	The rectangle collider size. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		VectorI GetRectangleColliderSize(const std::string& colliderName) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns 0 if collider not found or not circle. Check log. </summary>
		///
		/// <param name="colliderName">	Name of the collider. </param>
		///
		/// <returns>	The circle collider radius. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		float GetCircleColliderRadius(const std::string& colliderName) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Changes position of collider. For rectangles TL is changed. For circles, centre. Remember
		/// colliders are defined in frame space. Returns false if collider not found.
		/// </summary>
		///
		/// <param name="colliderName">	Name of the collider. </param>
		/// <param name="newPos">	   	The new position. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool ChangeColliderPosition(const std::string& colliderName, VectorI newPos);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Changes size of rectangle collider. Returns false if collider not found or it is not a
		/// rectangle.
		/// </summary>
		///
		/// <param name="colliderName">	Name of the collider. </param>
		/// <param name="newSize">	   	Size of the new. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool ChangeColliderSize(const std::string& colliderName, VectorI newSize);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Changes size of circle collider. Returns false if collider not found or it is not a circle.
		/// </summary>
		///
		/// <param name="colliderName">	Name of the collider. </param>
		/// <param name="newRadius">   	The new radius. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool ChangeColliderSize(const std::string& colliderName, float newRadius);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Writes collision data in XML format below the parent node. </summary>
		///
		/// <param name="parentNode">	[in,out] If non-null, the parent node. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void WriteXML(CHapiXMLNode* parentNode) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Reads collision data in XML format below the past node Note: removes existing data.
		/// </summary>
		///
		/// <param name="xml">		 	The XML. </param>
		/// <param name="parentNode">	[in,out] If non-null, the parent node. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool ReadXML(const CHapiXML& xml, CHapiXMLNode* parentNode);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Add a local space collision rectangle of type Name is optional. If not provided it is auto
		/// generated e.g. shape1, shape2... For ease it is returned.
		/// </summary>
		///
		/// <param name="rct"> 	The rectangle. </param>
		/// <param name="type">	The type. </param>
		/// <param name="name">	(Optional) The name. </param>
		///
		/// <returns>	A std::string. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::string Add(RectangleI rct, EColliderType type, std::string name = std::string());

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Add a local space collision circle of type Name is optional. If not provided it is auto
		/// generated e.g. shape1, shape2... For ease it is returned.
		/// </summary>
		///
		/// <param name="circle">	The circle. </param>
		/// <param name="type">  	The type. </param>
		/// <param name="name">  	(Optional) The name. </param>
		///
		/// <returns>	A std::string. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::string Add(Circle circle, EColliderType type, std::string name = std::string());

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Remove a collider (slow) </summary>
		///
		/// <param name="colliderName">	The collider name to delete. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool Delete(const std::string &colliderName);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Axis aligned bounding rect. </summary>
		///
		/// <param name="transform">	The transform. </param>
		///
		/// <returns>	The bounding a line break. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const RectangleI& GetBoundingAABR(const Transform& transform) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Oriented bounding rect. </summary>
		///
		/// <param name="transform">	The transform. </param>
		///
		/// <returns>	The bounding obr. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleOrientedF GetBoundingOBR(const Transform& transform) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Bounding circle. </summary>
		///
		/// <param name="transform">	The transform. </param>
		///
		/// <returns>	The bounding circle. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Circle GetBoundingCircle(const Transform& transform) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// The one collision function. How it behaves is controlled by the passed in settings
		/// Information about the collision is returned in collisionInfo.
		/// </summary>
		///
		/// <param name="settings">			Options for controlling the operation. </param>
		/// <param name="other">			The other. </param>
		/// <param name="collisionInfo">	[in,out] Information describing the collision. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool CheckCollision(const CollisionSettings &settings, const ColliderGroup& other,CollisionInfo& collisionInfo) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Retrieves the centre point of the marker collider type, given the transform Returns false if
		/// marker not found.
		/// </summary>
		///
		/// <param name="name">			The name. </param>
		/// <param name="transform">	The transform. </param>
		/// <param name="centre">   	[in,out] The centre. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool GetMarkerCentre(const std::string& name, const Transform& transform, VectorF &centre) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Determines if a local space point is inside a certain type of collider and returns name of
		/// first found If none found returns empty string.
		/// </summary>
		///
		/// <param name="pnt"> 	The point. </param>
		/// <param name="type">	The type. </param>
		///
		/// <returns>	A std::string. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::string PointInsideLocalSpace(const VectorF &pnt, EColliderType type) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Determines if a local space point is inside any collider and returns name of first found If
		/// none found returns empty string.
		/// </summary>
		///
		/// <param name="pnt">	The point. </param>
		///
		/// <returns>	A std::string. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::string PointInsideLocalSpace(const VectorF &pnt) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Determines if a local space point is inside a certain shape of collider and returns name of
		/// first found If none found returns empty string.
		/// </summary>
		///
		/// <param name="pnt">  	The point. </param>
		/// <param name="shape">	The shape. </param>
		///
		/// <returns>	A std::string. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::string PointInsideLocalSpace(const VectorF &pnt, EColliderShape shape) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// As above but returns the smallest collider the point is inside This helps with user picking
		/// in the editor If none found returns empty string.
		/// </summary>
		///
		/// <param name="pnt">	The point. </param>
		///
		/// <returns>	A std::string. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::string PointInsideSmallestLocalSpace(const VectorF &pnt) const;

		/*
			Note on these debug render functions
			For perfromance reasons you cannot directly ask a collider to draw itself, it must be done via one
			of these functions of the group (so caching refreshed)
		*/

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Renders one collider by name. </summary>
		///
		/// <param name="renderSurface">	[in,out] The render surface. </param>
		/// <param name="transform">		The transform. </param>
		/// <param name="col">				The col. </param>
		/// <param name="colliderName"> 	Name of the collider. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void RenderCollider(std::shared_ptr<Surface> &renderSurface, const Transform& transform, const Colour255& col, const std::string& colliderName) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Useful for debug purposes, render the collider shape type to Surface using transform and
		/// colour.
		/// </summary>
		///
		/// <param name="renderSurface">	[in,out] The render surface. </param>
		/// <param name="transform">		The transform. </param>
		/// <param name="type">				The type. </param>
		/// <param name="col">				(Optional) The col. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void RenderRects(std::shared_ptr<Surface> &renderSurface, const Transform& transform, EColliderType type, const Colour255& col = Colour255::WHITE) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Renders the circles. </summary>
		///
		/// <param name="renderSurface">	[in,out] The render surface. </param>
		/// <param name="transform">		The transform. </param>
		/// <param name="type">				The type. </param>
		/// <param name="col">				(Optional) The col. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void RenderCircles(std::shared_ptr<Surface> &renderSurface, const Transform& transform, EColliderType type, const Colour255& col = Colour255::WHITE) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// As above but for all types, with colours provided for each type else default to white.
		/// </summary>
		///
		/// <param name="renderSurface">	[in,out] The render surface. </param>
		/// <param name="transform">		The transform. </param>
		/// <param name="boundsColour"> 	(Optional) The bounds colour. </param>
		/// <param name="oneOfColour">  	(Optional) The one of colour. </param>
		/// <param name="markerColou">  	(Optional) The marker colou. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void RenderRects(std::shared_ptr<Surface> &renderSurface, const Transform& transform, const Colour255& boundsColour = Colour255::WHITE,
			const Colour255& oneOfColour = Colour255::WHITE, const Colour255& markerColou= Colour255::RED) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// As above but for all types, with colours provided for each type else default to white.
		/// </summary>
		///
		/// <param name="renderSurface">	[in,out] The render surface. </param>
		/// <param name="transform">		The transform. </param>
		/// <param name="boundsColour"> 	(Optional) The bounds colour. </param>
		/// <param name="oneOfColour">  	(Optional) The one of colour. </param>
		/// <param name="markerColou">  	(Optional) The marker colou. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void RenderCircles(std::shared_ptr<Surface> &renderSurface, const Transform& transform, const Colour255& boundsColour = Colour255::WHITE,
			const Colour255& oneOfColour = Colour255::WHITE, const Colour255& markerColou = Colour255::RED) const;

		/// <summary>	Clears the cache. </summary>
		void ClearCache() { m_cache.Clear(); }
	};
}

