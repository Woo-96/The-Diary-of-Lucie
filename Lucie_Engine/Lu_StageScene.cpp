#include "Lu_StageScene.h"
#include "Lu_Renderer.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_Object.h"
#include "Lu_Input.h"
#include "Lu_PlayerScript.h"
#include "Lu_NumberScript.h"
#include "Lu_HeartScript.h"
#include "Lu_ManaScript.h"
#include "Lu_ProgressBarScript.h"

#include "Lu_QuickItemScript.h"
#include "Lu_WeaponSlotScript.h"
#include "Lu_InventoryScript.h"

namespace Lu
{
	StageScene::StageScene()
	{
		SetName(L"StageSceneScript");
	}

	StageScene::~StageScene()
	{

	}

	void StageScene::Initialize()
	{
		Scene::Initialize();
	}

	void StageScene::Update()
	{
		Scene::Update();
	}

	void StageScene::LateUpdate()
	{
		Scene::LateUpdate();
	}

	void StageScene::Render()
	{
		Scene::Render();
	}

	void StageScene::Destroy()
	{
		Scene::Destroy();
	}

	void StageScene::OnEnter()
	{
		Scene::OnEnter();
	}

	void StageScene::OnExit()
	{
		Scene::OnExit();
	}

	void StageScene::CreateUI()
	{
		GameObject* pObject;
		MeshRenderer* pMeshRender;
		GameObject* pPlayer = SceneManager::FindPlayer();
		PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();

		// 크기 원본 1.5배
		{
			pObject = object::Instantiate<GameObject>(Vector3(0.f, -300.f, 150.f), Vector3(345.f, 135.f, 100.f), eLayerType::UI);
			pObject->SetName(L"UI_Layout");
			SceneManager::DontDestroyOnLoad(pObject);
			pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Layout_Mtrl"));
		}

		{
			pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 100.f), eLayerType::UI);
			pObject->SetName(L"UI_HeartMgr");
			SceneManager::DontDestroyOnLoad(pObject);
			HeartScript* pHPScript = pObject->AddComponent<HeartScript>();
			pPlayerScript->SetUI(PlayerScript::eUI::HP, (UIScript*)pHPScript);
			pHPScript->SetMaxHP(pPlayerScript->GetPlayerInfo().MaxHP);
			pHPScript->SetHeart(pPlayerScript->GetPlayerInfo().CurHP);
		}

		{
			pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 100.f), eLayerType::UI);
			pObject->SetName(L"UI_ManaMgr");
			SceneManager::DontDestroyOnLoad(pObject);
			ManaScript* pMPScript = pObject->AddComponent<ManaScript>();
			pPlayerScript->SetUI(PlayerScript::eUI::MP, (UIScript*)pMPScript);
			pMPScript->SetMaxMP(pPlayerScript->GetPlayerInfo().MaxMP);
			pMPScript->SetMana(pPlayerScript->GetPlayerInfo().CurMP);
		}

		{
			pObject = object::Instantiate<GameObject>(Vector3(15.f, -315.f, 100.f), Vector3(190.5f, 10.5f, 100.f), eLayerType::UI);
			pObject->SetName(L"UI_EXP");
			SceneManager::DontDestroyOnLoad(pObject);
			pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"EXP_Meter_Mtrl"));
			ProgressBarScript* pProgressBarScript = pObject->AddComponent<ProgressBarScript>();
			pProgressBarScript->SetMeshRender(pMeshRender);
			pProgressBarScript->SetMaxValue(pPlayerScript->GetPlayerInfo().MaxEXP);
			pProgressBarScript->SetCurValue(pPlayerScript->GetPlayerInfo().CurEXP);
			pPlayerScript->SetUI(PlayerScript::eUI::EXP, (UIScript*)pProgressBarScript);
		}

		{
			pObject = object::Instantiate<GameObject>(Vector3(-7.f, -336.5f, 100.f), Vector3(249.f, 21.f, 100.f), eLayerType::UI);
			pObject->SetName(L"UI_TP");
			SceneManager::DontDestroyOnLoad(pObject);
			pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"TP_Meter_Mtrl"));
			ProgressBarScript* pProgressBarScript = pObject->AddComponent<ProgressBarScript>();
			pProgressBarScript->SetMeshRender(pMeshRender);
			pProgressBarScript->SetMaxValue(pPlayerScript->GetPlayerInfo().MaxTP);
			pProgressBarScript->SetCurValue(pPlayerScript->GetPlayerInfo().CurTP);
			pPlayerScript->SetUI(PlayerScript::eUI::TP, (UIScript*)pProgressBarScript);
		}

		{
			GameObject* pInven = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 100.f), eLayerType::UI);
			pInven->SetName(L"UI_Inventory");
			SceneManager::DontDestroyOnLoad(pInven);
			InventoryScript* pInvenScript = pInven->AddComponent<InventoryScript>();
			pPlayerScript->SetUI(PlayerScript::eUI::Inventory, (UIScript*)pInvenScript);


			GameObject* pWeapon = object::Instantiate<GameObject>(Vector3(640.f, -330.f, 100.f), Vector3(105.f, 102.f, 100.f), eLayerType::UI);
			pWeapon->SetName(L"UI_WeaponSlot");
			SceneManager::DontDestroyOnLoad(pWeapon);
			pMeshRender = pWeapon->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"WeaponSlot_Mtrl"));
			WeaponSlotScript* pWeaponSlot = pWeapon->AddComponent<WeaponSlotScript>();
			pWeaponSlot->SetInventory(pInvenScript);
			pPlayerScript->SetUI(PlayerScript::eUI::WeaponSlot, (UIScript*)pWeaponSlot);

			GameObject* pQuick = object::Instantiate<GameObject>(Vector3(540.f, -345.f, 100.f), Vector3(66.f, 66.f, 100.f), eLayerType::UI);
			pQuick->SetName(L"UI_QuickItem");
			SceneManager::DontDestroyOnLoad(pQuick);
			pMeshRender = pQuick->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"QuickItem_Mtrl"));
			QuickItemScript* pQuickSlot = pQuick->AddComponent<QuickItemScript>();
			pQuickSlot->SetInventory(pInvenScript);
			pPlayerScript->SetUI(PlayerScript::eUI::QuickItem, (UIScript*)pQuickSlot);

			pInvenScript->SetWeaponSlot(pWeaponSlot);
			pInvenScript->SetQuickSlot(pQuickSlot);
		}

		{
			pObject = object::Instantiate<GameObject>(Vector3(685.f, 355.f, 100.f), Vector3(21.f, 28.5f, 100.f), eLayerType::UI);
			pObject->SetName(L"UI_Gold_G");
			SceneManager::DontDestroyOnLoad(pObject);
			pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Gold_A_Mtrl"));
		}

		// 원본 사이즈 1.5배 (y축 조금 작은거같아서 약간 늘림)
		{
			pObject = object::Instantiate<GameObject>(Vector3(662.f, 354.f, 100.f), Vector3(21.f, 30.f, 100.f), eLayerType::UI);
			pObject->SetName(L"UI_Gold_Num");
			SceneManager::DontDestroyOnLoad(pObject);

			pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Gold_B_Mtrl"));
		
			NumberScript* pNum = pObject->AddComponent<NumberScript>();
			pNum->SetTexResolution(Vector2(140.f, 20.f));
			pNum->SetSliceSize(Vector2(14.f, 20.f));
			
			Vector2 arrLT[10];
			for (int i = 0; i < 10; ++i)
			{
				arrLT[i] = Vector2(14.f * i, 0.f);
			}
			pNum->SetNumbersLT(arrLT, 10);
			pNum->SetCurNumber(0);

			pPlayerScript->SetUI(PlayerScript::eUI::Gold, (UIScript*)pNum);
		}

		{
			pObject = object::Instantiate<GameObject>(Vector3(-113.f, -313.f, 100.f), Vector3(48.f, 28.5f, 100.f), eLayerType::UI);
			pObject->SetName(L"UI_Level");
			SceneManager::DontDestroyOnLoad(pObject);

			pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"LvNumber_Mtrl"));

			NumberScript* pNum = pObject->AddComponent<NumberScript>();
			pNum->SetTexResolution(Vector2(320.f, 19.f));
			pNum->SetSliceSize(Vector2(32.f, 19.f));

			Vector2 arrLT[10];
			for (int i = 0; i < 10; ++i)
			{
				arrLT[i] = Vector2(32.f * i, 0.f);
			}
			pNum->SetNumbersLT(arrLT, 10);
			pNum->SetCurNumber(1);

			pPlayerScript->SetUI(PlayerScript::eUI::Level, (UIScript*)pNum);
		}
	}	
}