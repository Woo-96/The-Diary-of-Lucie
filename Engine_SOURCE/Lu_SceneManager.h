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
		static void Release();

	public:
		static Scene* LoadScene(std::wstring _Name);
	};
}