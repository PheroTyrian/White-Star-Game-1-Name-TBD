#pragma once

namespace HAPISPACE {

	// Currently available coponents
	enum class EComponentType
	{
		eTransform = 0,
		eCollider,
		// animator TODO
		// rigid body TODO
		eNumTypes
	};

	class Sprite;

	// Base component class
	class IComponent
	{
	private:
		Sprite &m_owner;
	protected:

	public:
		IComponent(Sprite &owner) noexcept : m_owner(owner) {}
		virtual ~IComponent() {}

		// Every component has an owner
		Sprite& GetOwner() const { return m_owner; }

		// Virtuals
		virtual EComponentType GetType() const = 0;
	};
}
