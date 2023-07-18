#pragma once
#include "..\Engine_SOURCE\Lu_SceneManager.h"
#include "Lu_TitleScene.h"
#include "Lu_TutorialScene.h"
#include "Lu_LobbyScene.h"
#include "Lu_WeaponChoiceScene.h"
#include "Lu_DiceScene.h"
#include "Lu_StoreScene.h"
#include "Lu_Nomal1Scene.h"
#include "Lu_Nomal2Scene.h"
#include "Lu_MidBossScene.h"
#include "Lu_NextFloorScene.h"
#include "Lu_BossWayScene.h"
#include "Lu_BossScene.h"

#include "Lu_GameOverScene.h"

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
		SceneManager::CreateScene<TitleScene>(L"TitleScene");
		//SceneManager::CreateScene<TutorialScene>(L"TutorialScene");
		SceneManager::CreateScene<LobbyScene>(L"LobbyScene");

		SceneManager::CreateScene<WeaponChoiceScene>(L"WeaponChoiceScene");
		SceneManager::CreateScene<DiceScene>(L"DiceScene");
		SceneManager::CreateScene<StoreScene>(L"StoreScene");
		SceneManager::CreateScene<Nomal1Scene>(L"Nomal1Scene");
		SceneManager::CreateScene<Nomal2Scene>(L"Nomal2Scene");
		SceneManager::CreateScene<MidBossScene>(L"MidBossScene");
		SceneManager::CreateScene<NextFloorScene>(L"NextFloorScene");
		SceneManager::CreateScene<BossWayScene>(L"BossWayScene");
		SceneManager::CreateScene<BossScene>(L"BossScene");

		SceneManager::CreateScene<GameOverScene>(L"GameOverScene");

		SceneManager::LoadScene(L"TitleScene");

		// 수업 진도용
		//SceneManager::CreateScene<PlayScene>(L"PlayScene");
	}
}