#pragma once
#include "Lu_Entity.h"

namespace Lu
{
	using namespace Lu::enums;
	using namespace Lu::math;

	class GameObject;
	class Component : public Entity
	{
	public:
		Component(eComponentType _Type);
		virtual ~Component();

	private:
		const eComponentType	m_Type;
		GameObject*				m_Owner;

	public:
		void SetOwner(GameObject* _Owner)
		{
			m_Owner = _Owner;
		}

	public:
		GameObject* GetOwner()
		{
			return m_Owner;
		}

	public:
		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();
	};
}