#include "Lu_SceneManager.h"

namespace Lu
{
	Scene* SceneManager::m_ActiveScene = nullptr;
	std::map<std::wstring, Scene*> SceneManager::m_Scenes;

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
		
		m_ActiveScene->OnExit();
		m_ActiveScene = iter->second;
		m_ActiveScene->OnEnter();

		return iter->second;
	}
}