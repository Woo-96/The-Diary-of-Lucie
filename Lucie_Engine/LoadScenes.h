#pragma once
//#include "Lu_SceneManager.h"
#include "..\Engine_SOURCE\Lu_SceneManager.h"
#include "Lu_PlayScene.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\x64\\Debug\\Lucie_Engine.lib")
#else
#pragma comment(lib, "..\\x64\\Release\\Lucie_Engine.lib")
#endif

namespace Lu
{
	void InitializeScenes()
	{
		SceneManager::CreateScene<PlayScene>(L"PlayScene");
	}
}