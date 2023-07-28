#include "Lu_StageScene.h"
#include "Lu_Renderer.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_Object.h"
#include "Lu_Input.h"
#include "Lu_Layout.h"
#include "Lu_HP.h"
#include "Lu_MP.h"
#include "Lu_EXP.h"
#include "Lu_TP.h"
#include "Lu_QuickItem.h"
#include "Lu_WeaponSlot.h"
#include "Lu_Inventory.h"
#include "Lu_NumberScript.h"

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

		CreateHUD();
		//CreateInventory();
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

	}

	void StageScene::CreateHUD()
	{
		GameObject* pObject;
		Transform* pTransform;
		MeshRenderer* pMeshRender;

		{
			pObject = object::Instantiate<GameObject>(Vector3(0.f, -300.f, 150.f), eLayerType::UI);
			pObject->SetName(L"UI_Layout");

			pTransform = pObject->GetComponent<Transform>();
			// 원본 사이즈 1.5배
			pTransform->SetScale(Vector3(345.f, 135.f, 100.f));

			pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Layout_Mtrl"));

			pObject->AddComponent<Layout>();
		}

		{
			pObject = object::Instantiate<GameObject>(Vector3(-100.f, -280.f, 100.f), eLayerType::UI);
			pObject->SetName(L"UI_HPIcon");

			pTransform = pObject->GetComponent<Transform>();
			// 원본 사이즈 1.5배
			pTransform->SetScale(Vector3(72.f, 33.f, 100.f));

			pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"HP_Icon_Mtrl"));

			pObject->AddComponent<HP>();
		}

		{
			pObject = object::Instantiate<GameObject>(Vector3(85.f, -280.f, 100.f), eLayerType::UI);
			pObject->SetName(L"UI_MPIcon");

			pTransform = pObject->GetComponent<Transform>();
			// 원본 사이즈 1.5배
			pTransform->SetScale(Vector3(72.f, 33.f, 100.f));

			pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"MP_Icon_Mtrl"));

			pObject->AddComponent<MP>();
		}

		{
			pObject = object::Instantiate<GameObject>(Vector3(15.f, -315.f, 100.f), eLayerType::UI);
			pObject->SetName(L"UI_EXP");

			pTransform = pObject->GetComponent<Transform>();
			// 원본 사이즈 1.5배
			pTransform->SetScale(Vector3(190.5f, 10.5f, 100.f));

			pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"EXP_Meter_Mtrl"));

			pObject->AddComponent<EXP>();
		}

		{
			pObject = object::Instantiate<GameObject>(Vector3(-7.f, -336.5f, 100.f), eLayerType::UI);
			pObject->SetName(L"UI_TP");

			pTransform = pObject->GetComponent<Transform>();
			// 원본 사이즈 1.5배
			pTransform->SetScale(Vector3(249.f, 21.f, 100.f));

			pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"TP_Meter_Mtrl"));

			pObject->AddComponent<TP>();
		}

		{
			pObject = object::Instantiate<GameObject>(Vector3(640.f, -330.f, 100.f), eLayerType::UI);
			pObject->SetName(L"UI_WeaponSlot");

			pTransform = pObject->GetComponent<Transform>();
			// 원본 사이즈 1.5배
			pTransform->SetScale(Vector3(105.f, 102.f, 100.f));

			pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"weaponSlotA_Mtrl"));

			pObject->AddComponent<WeaponSlot>();
		}

		{
			pObject = object::Instantiate<GameObject>(Vector3(540.f, -345.f, 100.f), eLayerType::UI);
			pObject->SetName(L"UI_QuickItem");

			pTransform = pObject->GetComponent<Transform>();
			// 원본 사이즈 1.5배
			pTransform->SetScale(Vector3(66.f, 66.f, 100.f));

			pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"quickItem_Mtrl"));

			pObject->AddComponent<QuickItem>();
		}

		{
			pObject = object::Instantiate<GameObject>(Vector3(685.f, 355.f, 100.f), eLayerType::UI);
			pObject->SetName(L"Gold_G");

			pTransform = pObject->GetComponent<Transform>();
			// 원본 사이즈 1.5배
			pTransform->SetScale(Vector3(21.f, 28.5f, 100.f));

			pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Gold_A_Mtrl"));
		}

		{
			pObject = object::Instantiate<GameObject>(Vector3(662.f, 354.f, 100.f), eLayerType::UI);
			pObject->SetName(L"Gold_Num");

			pTransform = pObject->GetComponent<Transform>();
			// 원본 사이즈 1.5배 (y축 조금 작은거같아서 약간 늘림)
			pTransform->SetScale(Vector3(21.f, 30.f, 100.f));

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
			pNum->SetCurNumber(3);
		}

		{
			pObject = object::Instantiate<GameObject>(Vector3(-113.f, -313.f, 100.f), eLayerType::UI);
			pObject->SetName(L"Level");

			pTransform = pObject->GetComponent<Transform>();
			// 원본 사이즈 1.5배
			pTransform->SetScale(Vector3(48.f, 28.5f, 100.f));

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
		}
	}
	void StageScene::CreateInventory()
	{
		GameObject* pObject;
		Transform* pTransform;
		MeshRenderer* pMeshRender;

		{
			pObject = object::Instantiate<GameObject>(Vector3(516.f, -80.f, 0.f), eLayerType::UI);
			pObject->SetName(L"Inventory_Layout");

			pTransform = pObject->GetComponent<Transform>();
			// 원본 사이즈 1.5배
			pTransform->SetScale(Vector3(360.f, 444.f, 100.f));

			pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"InvenBG_Mtrl"));

			pObject->AddComponent<Inventory>();
		}
	}
}