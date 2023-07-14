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
			if (gameObj->GetState()
				!= GameObject::eState::Active)
				continue;

			gameObj->Update();
		}
	}
	
	void Layer::LateUpdate()
	{
		for (GameObject* gameObj : m_GameObjects)
		{
			if (gameObj->GetState()
				!= GameObject::eState::Active)
				continue;

			gameObj->LateUpdate();
		}
	}
	
	void Layer::Render()
	{
		for (GameObject* gameObj : m_GameObjects)
		{
			if (gameObj->GetState()
				!= GameObject::eState::Active)
				continue;

			gameObj->Render();
		}
	}

	void Layer::Destory()
	{
		// dead ������Ʈ ��Ƶα�
		std::set<GameObject*> deleteGameObj = {};
		for (GameObject* gameObj : m_GameObjects)
		{
			if (gameObj->GetState()
				== GameObject::eState::Dead)
				deleteGameObj.insert(gameObj);
		}

		// daed ������Ʈ ���ܽ�Ű�� layer����
		typedef std::vector<GameObject*>::iterator GameObjectIter;
		for (GameObjectIter iter = m_GameObjects.begin()
			; iter != m_GameObjects.end(); )
		{
			std::set<GameObject*>::iterator deleteIter
				= deleteGameObj.find(*(iter));

			if (deleteIter != deleteGameObj.end())
			{
				iter = m_GameObjects.erase(iter);
				continue;
			}

			iter++;
		}

		//�޸� ����
		for (GameObject* gameObj : deleteGameObj)
		{
			delete gameObj;
			gameObj = nullptr;
		}
	}

	void Layer::AddGameObject(GameObject* _GameObj)
	{
		m_GameObjects.push_back(_GameObj);
	}
}
