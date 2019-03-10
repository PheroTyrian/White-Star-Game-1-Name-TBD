#pragma once

#include "IComponent.h"
#include "CoreTypes\Transform.h"

namespace HAPISPACE {

	// A transfor component used by Sprite
	class ComponentTransform :
		public IComponent
	{
		private:
			Transform m_transform;

			// For ray generation:
			VectorF m_lastPosition;
		public:
			ComponentTransform(Sprite &owner) noexcept : IComponent(owner) {}
			EComponentType GetType() const override final { return EComponentType::eTransform; }

			// Allows reading of transform values but not writing.
			// To change values call one of the fns below
			const Transform& GetTransform() const { return m_transform; }

			void SetTransform(Transform newTrans) { m_transform = newTrans; }

			void SetRotation(float radians) { m_transform.rotation = radians; }

			// Sets new position and remembers old
			void SetPosition(VectorF pos) { m_lastPosition = m_transform.position; m_transform.position = (VectorF)pos; }

			void SetScaling(const VectorF &scale) { m_transform.scale = scale; }

			// Set origin to centre of current frame rectangle
			void SetOriginToCentreOfFrame();

			// Sets the origin used for positioning and rotation
			void SetOrigin(const VectorF& newOrigin) { m_transform.origin = newOrigin; }

			void Translate(const VectorF &trans) { m_lastPosition = m_transform.position; m_transform.Translate(trans); }

			void Rotate(float radians) { m_transform.Rotate(radians); }

			// Gets
			float GetRotation() const { return m_transform.rotation; }
			const VectorF& GetScale() const { return m_transform.scale; }
			const VectorF& GetPosition() const { return m_transform.position; }
			const VectorF& GetOrigin() const { return m_transform.origin; }
			bool IsRotated() const { return m_transform.IsRotated(); }
			bool IsScaled() const { return m_transform.IsScaled();}

			// Can be used to determine collision points etc.
			LineF GetLineFromLastPosition() const { return LineF(m_lastPosition, GetPosition()); }
	};
}
