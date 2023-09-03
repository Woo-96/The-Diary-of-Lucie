#include "Lu_Layer.h"

namespace Lu
{
	Layer::Layer()
	{

	}

	Layer::~Layer()
	{
		//for (GameObject* gameObj : m_GameObjects)
		//{
		//	if (gameObj == nullptr)
		//		continue;

		//	delete gameObj;
		//	gameObj = nullptr;
		//}

		for (int i = 0; i < m_GameObjects.size(); ++i)
		{
			GameObject* gameObj = m_GameObjects[i];
			if (gameObj == nullptr)
				continue;

			delete gameObj;
			m_GameObjects[i] = nullptr;
		}

	}
	
	void Layer::Initialize()
	{

	}
	
	void Layer::Update()
	{
		//for (GameObject* gameObj : m_GameObjects)
		//{
		//	if (gameObj->GetState()
		//		!= GameObject::eState::Active)
		//		continue;

		//	gameObj->Update();
		//}

		for (int i = 0; i < m_GameObjects.size(); ++i)
		{
			GameObject* gameObj = m_GameObjects[i];

			if (gameObj->GetState() != GameObject::eState::Active)
				continue;

			gameObj->Update();
		}
	}
	
	void Layer::LateUpdate()
	{
		//for (GameObject* gameObj : m_GameObjects)
		//{
		//	if (gameObj->GetState()
		//		!= GameObject::eState::Active)
		//		continue;

		//	gameObj->LateUpdate();
		//}

		for (int i = 0; i < m_GameObjects.size(); ++i)
		{
			GameObject* gameObj = m_GameObjects[i];

			if (gameObj->GetState() != GameObject::eState::Active)
				continue;

			gameObj->LateUpdate();
		}
	}
	
	void Layer::Render()
	{
		//for (GameObject* gameObj : m_GameObjects)
		//{
		//	if (gameObj->GetState()
		//		!= GameObject::eState::Active)
		//		continue;

		//	gameObj->Render();
		//}

		for (int i = 0; i < m_GameObjects.size(); ++i)
		{
			GameObject* gameObj = m_GameObjects[i];

			if (gameObj->GetState() != GameObject::eState::Active)
				continue;

			gameObj->Render();
		}
	}

	void Layer::Destory()
	{
		// dead 오브젝트 모아두기
		std::set<GameObject*> deleteGameObj = {};
		for (int i = 0; i < m_GameObjects.size(); ++i)
		{
			GameObject* gameObj = m_GameObjects[i];

			if (gameObj->GetState() == GameObject::eState::Dead)
				deleteGameObj.insert(gameObj);
		}

		// daed 오브젝트 제외시키기 layer에서
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

		//메모리 해제
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

	void Layer::RemoveGameObject(GameObject* _GameObj)
	{
		for (auto iter = m_GameObjects.begin(); iter != m_GameObjects.end(); ++iter)
		{
			if (*iter == _GameObj)
			{
				m_GameObjects.erase(iter);
				break; // 객체를 찾고 나면 반복문 종료
			}
		}
	}
}
