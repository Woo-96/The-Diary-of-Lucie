#include "Lu_GameObject.h"
#include "Lu_Renderer.h"
#include "Lu_GraphicDevice_Dx11.h"
#include "Lu_Transform.h"

namespace Lu
{
	GameObject::GameObject()
		: m_State(eState::Active)
		, m_LayerIdx(-1)
		, m_bActive(true)
	{
		AddComponent<Transform>();
	}

	GameObject::~GameObject()
	{
		for (Component* comp : m_Components)
		{
			if (comp == nullptr)
				continue;

			delete comp;
			comp = nullptr;
		}

		for (Script* script : m_Scripts)
		{
			if (script == nullptr)
				continue;

			delete script;
			script = nullptr;
		}
	}

	void GameObject::Update()
	{
		if (!m_bActive)
			return;

		for (Component* comp : m_Components)
		{
			comp->Update();
		}

		for (Script* script : m_Scripts)
		{
			script->Update();
		}
	}

	void GameObject::LateUpdate()
	{
		if (!m_bActive)
			return;

		for (Component* comp : m_Components)
		{
			comp->LateUpdate();
		}

		for (Script* script : m_Scripts)
		{
			script->LateUpdate();
		}
	}

	void GameObject::Render()
	{
		if (!m_bActive)
			return;

		for (Component* comp : m_Components)
		{
			comp->Render();
		}

		for (Script* script : m_Scripts)
		{
			script->Render();
		}
	}
}