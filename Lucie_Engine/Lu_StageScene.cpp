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

namespace Lu
{
	StageScene::StageScene()
	{

	}

	StageScene::~StageScene()
	{

	}

	void StageScene::Initialize()
	{
		Scene::Initialize();

		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 900.f), eLayerType::Default);
			pObject->SetName(L"Stage");

			Transform* pTransform = pObject->GetComponent<Transform>();
			pTransform->SetScale(Vector3(1512.f, 1008.f, 100.f));

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Stage_Mtrl"));
		}

		CreateHUD();
		CreateInventory();
	}

	void StageScene::Update()
	{
		Scene::Update();

		if (Input::GetKeyUp(eKeyCode::SPACE))
		{
			SceneManager::LoadScene(L"GameOverScene");
		}
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
			pObject = object::Instantiate<GameObject>(Vector3(-7.f, -347.f, 100.f), eLayerType::UI);
			pObject->SetName(L"UI_TP");

			pTransform = pObject->GetComponent<Transform>();
			// 원본 사이즈 1.5배
			pTransform->SetScale(Vector3(249.f, 42.f, 100.f));

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