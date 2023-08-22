#pragma once
#include "Lu_Entity.h"
#include "Lu_Component.h"
#include "Lu_Script.h"
#include "Lu_Animator.h"

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

		int                     m_LayerIdx;

	public:
		void SetState(eState _State)
		{ 
			m_State = _State;
		}

		void SetLayerIndex(int _Layer)
		{
			m_LayerIdx = _Layer;
		}

	public:
		eState GetState()
		{ 
			return m_State; 
		}

		bool IsDead() const
		{
			return eState::Dead == m_State;
		}

		int GetLayerIndex() const
		{
			return m_LayerIdx;
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

		template <typename T>
		bool DeleteComponent()
		{
			bool bResult = false;

			{
				auto iter = m_Components.begin();
				auto iterEnd = m_Components.end();

				for (; iter != iterEnd; ++iter)
				{
					T* component = dynamic_cast<T*>(*iter);
					if (component != nullptr)
					{
						delete component;
						m_Components.erase(iter);
						bResult = true;
						break;
					}
				}
			}

			if (!bResult)
			{
				auto iter = m_Scripts.begin();
				auto iterEnd = m_Scripts.end();

				for (; iter != iterEnd; ++iter)
				{
					T* component = dynamic_cast<T*>(*iter);
					if (component != nullptr)
					{
						delete component;
						m_Scripts.erase(iter);
						bResult = true;
						break;
					}
				}
			}

			return bResult;
		}
	};
}