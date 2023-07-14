#pragma once
#include "..\Engine_SOURCE\Lu_SceneManager.h"
#include "Lu_TitleScene.h"
#include "Lu_TutorialScene.h"
#include "Lu_LobbyScene.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\x64\\Debug\\Lucie_Engine.lib")
#else
#pragma comment(lib, "..\\x64\\Release\\Lucie_Engine.lib")
#endif

namespace Lu
{
	void InitializeScenes()
	{
		SceneManager::CreateScene<TitleScene>(L"TitleScene");
		SceneManager::CreateScene<TutorialScene>(L"TutorialScene");
		SceneManager::CreateScene<LobbyScene>(L"LobbyScene");

		SceneManager::LoadScene(L"TitleScene");
	}
}