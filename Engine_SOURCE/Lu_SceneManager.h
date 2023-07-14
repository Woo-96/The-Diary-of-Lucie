#pragma once
#include "Lu_Scene.h"

namespace Lu
{
	class SceneManager
	{
	private:
		static Scene*							m_ActiveScene;
		static std::map<std::wstring, Scene*>	m_Scenes;

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