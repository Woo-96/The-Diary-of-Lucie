#include "Lu_SceneManager.h"
#include "Lu_GameObject.h"

namespace Lu
{
	Scene* SceneManager::m_ActiveScene = nullptr;
	std::map<std::wstring, Scene*> SceneManager::m_Scenes;
	std::vector<GameObject*> SceneManager::m_DontDestroyObjects;

	void SceneManager::Initialize()
	{

	}

	void SceneManager::Update()
	{
		m_ActiveScene->Update();
	}

	void SceneManager::LateUpdate()
	{
		m_ActiveScene->LateUpdate();
	}

	void SceneManager::Render()
	{
		m_ActiveScene->Render();
	}

	void SceneManager::Destroy()
	{
		m_ActiveScene->Destroy();
	}

	void SceneManager::Release()
	{
		for (auto& iter : m_Scenes)
		{
			delete iter.second;
			iter.second = nullptr;
		}
	}

	Scene* SceneManager::LoadScene(std::wstring _Name)
	{
		std::map<std::wstring, Scene*>::iterator iter = m_Scenes.find(_Name);

		if (iter == m_Scenes.end())
			return nullptr;

		// 이전 씬에서 DontDestroyOnLoad로 설정한 객체를 새 씬으로 이동
		for (size_t i = 0; i < m_DontDestroyObjects.size(); ++i)
		{
			GameObject* dontDestroyObject = m_DontDestroyObjects[i];

			// 현재 씬에서 객체 제거
			if (m_ActiveScene)
			{
				m_ActiveScene->RemoveGameObject((eLayerType)dontDestroyObject->GetLayerIndex(), dontDestroyObject);
			}

			// 새 씬에서 해당 객체 찾기
			GameObject* foundObject = iter->second->FindGameObject((eLayerType)dontDestroyObject->GetLayerIndex(), dontDestroyObject);

			// 객체가 이미 새로운 씬에 있는지 확인
			if (!foundObject)
			{
				// 새로운 씬에 객체 추가
				iter->second->AddGameObject((eLayerType)dontDestroyObject->GetLayerIndex(), dontDestroyObject);
			}
		}

		m_ActiveScene->OnExit();
		m_ActiveScene = iter->second;
		m_ActiveScene->OnEnter();

		return iter->second;
	}


	void SceneManager::DontUseOnLoad(std::wstring objectName)
	{
		// m_DontDestroyObjects를 반복문으로 돌며 동일한 이름의 객체를 찾는다
		for (auto& object : m_DontDestroyObjects)
		{
			// 객체의 이름을 얻어온다
			std::wstring name = object->GetName();

			// 동일한 이름의 객체를 찾았을 경우
			if (name == objectName)
			{
				// 호출한 씬에서 제외
				if (m_ActiveScene)
				{
					m_ActiveScene->RemoveGameObject((eLayerType)object->GetLayerIndex(), object);
				}
			}
		}
	}

	void SceneManager::DontUseOnLoad(eLayerType _LayerType)
	{
		for (auto& object : m_DontDestroyObjects)
		{
			int iLayerType = object->GetLayerIndex();

			if ((int)_LayerType == iLayerType)
			{
				if (m_ActiveScene)
				{
					m_ActiveScene->RemoveGameObject((eLayerType)iLayerType, object);
				}
			}
		}
	}

	GameObject* SceneManager::FindPlayer()
	{
		for (auto& object : m_DontDestroyObjects)
		{
			std::wstring name = object->GetName();

			if (L"Player" == name)
			{
				return object;
				break;
			}
		}

		return nullptr;
	}
}