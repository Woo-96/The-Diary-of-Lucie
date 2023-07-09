#include "Lu_GameObject.h"
#include "Lu_Renderer.h"
#include "Lu_GraphicDevice_Dx11.h"
#include "Lu_Transform.h"

namespace Lu
{
	GameObject::GameObject()
		: m_State(eState::Active)
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

	void GameObject::Initialize()
	{

	}

	void GameObject::Update()
	{
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