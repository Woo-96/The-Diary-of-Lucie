#pragma once
#include "Lu_Scene.h"

namespace Lu
{
	class SceneManager
	{
	private:
		static Scene*							m_ActiveScene;
		static std::map<std::wstring, Scene*>	m_Scenes;

		static std::vector<GameObject*>			m_DontDestroyObjects;

	public:
		static Scene* GetActiveScene() 
		{ 
			return m_ActiveScene; 
		}

	public:
		static void Initialize();
		static void Update();
		static void LateUpdate();
		static void Render();
		static void Destroy();
		static void Release();

	public:
		static Scene* LoadScene(std::wstring _Name);

		// 씬 전환 시에 객체를 파괴하지 않도록 설정하는 함수
		static void DontDestroyOnLoad(GameObject* _GameObject)
		{
			m_DontDestroyObjects.push_back(_GameObject);
		}

		static void DontUseOnLoad(std::wstring _Name);
		static void DontUseOnLoad(eLayerType _LayerType);

		static GameObject* FindPlayer();

	public:
		template <typename T>
		static bool CreateScene(std::wstring _Name)
		{
			T* scene = new T();

			std::map<std::wstring, Scene*>::iterator iter
				= m_Scenes.find(_Name);

			if (iter != m_Scenes.end())
				return false;

			m_Scenes.insert(std::make_pair(_Name, scene));
			m_ActiveScene = scene;
			scene->Initialize();
			return true;
		}
	};
}