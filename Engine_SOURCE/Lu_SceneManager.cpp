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

		// ���� ������ DontDestroyOnLoad�� ������ ��ü�� �� ������ �̵�
		for (size_t i = 0; i < m_DontDestroyObjects.size(); ++i)
		{
			GameObject* dontDestroyObject = m_DontDestroyObjects[i];

			// ���� ������ ��ü ����
			if (m_ActiveScene)
			{
				m_ActiveScene->RemoveGameObject((eLayerType)dontDestroyObject->GetLayerIndex(), dontDestroyObject);
			}

			// �� ������ �ش� ��ü ã��
			GameObject* foundObject = iter->second->FindGameObject((eLayerType)dontDestroyObject->GetLayerIndex(), dontDestroyObject);

			// ��ü�� �̹� ���ο� ���� �ִ��� Ȯ��
			if (!foundObject)
			{
				// ���ο� ���� ��ü �߰�
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
		// m_DontDestroyObjects�� �ݺ������� ���� ������ �̸��� ��ü�� ã�´�
		for (auto& object : m_DontDestroyObjects)
		{
			// ��ü�� �̸��� ���´�
			std::wstring name = object->GetName();

			// ������ �̸��� ��ü�� ã���� ���
			if (name == objectName)
			{
				// ȣ���� ������ ����
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