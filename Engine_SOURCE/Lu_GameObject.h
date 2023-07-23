#pragma once
#include "Lu_Entity.h"
#include "Lu_Component.h"
#include "Lu_Script.h"

namespace Lu
{
	class GameObject : public Entity
	{
	public:
		enum class eState
		{
			Active,
			Paused,
			Dead,
		};

	public:		
		GameObject();
		virtual ~GameObject();

	private:
		eState					m_State;
		std::vector<Component*> m_Components;
		std::vector<Script*>	m_Scripts;

	public:
		void SetState(eState _State)
		{ 
			m_State = _State;
		}

	public:
		eState GetState()
		{ 
			return m_State; 
		}

	public:
		virtual void Initialize() {};
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

	public:
		template <typename T>
		T* GetComponent()
		{
			T* component;
			for (Component* comp : m_Components)
			{
				component = dynamic_cast<T*>(comp);
				if (component != nullptr)
					return component;
			}

			for (Script* script : m_Scripts)
			{
				component = dynamic_cast<T*>(script);
				if (component != nullptr)
					return component;
			}

			return nullptr;
		}

		template <typename T>
		const std::vector<T*> GetComponents()
		{
			std::vector<T*> comps;

			T* component;
			for (Component* comp : m_Components)
			{
				component = dynamic_cast<T*>(comp);
				if (component != nullptr)
					comps.push_back(component);
			}

			for (Script* script : m_Scripts)
			{
				component = dynamic_cast<T*>(script);
				if (component != nullptr)
					comps.push_back(component);
			}

			return comps;
		}

		template <typename T>
		T* AddComponent()
		{
			T* comp = new T();

			Component* buff
				= dynamic_cast<Component*>(comp);
			Script* script
				= dynamic_cast<Script*>(buff);

			if (buff == nullptr)
				return nullptr;

			if (script == nullptr)
				m_Components.push_back(buff);
			else
				m_Scripts.push_back(script);

			comp->SetOwner(this);
			comp->Initialize();

			return comp;
		}
	};
}