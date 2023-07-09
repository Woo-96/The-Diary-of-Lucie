#include "Lu_Layer.h"

namespace Lu
{
	Layer::Layer()
	{

	}

	Layer::~Layer()
	{
		for (GameObject* gameObj : m_GameObjects)
		{
			if (gameObj == nullptr)
				continue;

			delete gameObj;
			gameObj = nullptr;
		}
	}
	
	void Layer::Initialize()
	{
	
	}
	
	void Layer::Update()
	{
		for (GameObject* gameObj : m_GameObjects)
		{
			gameObj->Update();
		}
	}
	
	void Layer::LateUpdate()
	{
		for (GameObject* gameObj : m_GameObjects)
		{
			gameObj->LateUpdate();
		}
	}
	
	void Layer::Render()
	{
		for (GameObject* gameObj : m_GameObjects)
		{
			gameObj->Render();
		}
	}

	void Layer::AddGameObject(GameObject* _GameObj)
	{
		m_GameObjects.push_back(_GameObj);
	}
}
