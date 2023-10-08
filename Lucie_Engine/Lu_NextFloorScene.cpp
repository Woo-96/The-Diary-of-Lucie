#include "Lu_NextFloorScene.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_CameraScript.h"
#include "Lu_Renderer.h"
#include "Lu_PortalScript.h"
#include "Lu_MagicCircleScript.h"
#include "Lu_WoodScript.h"
#include "Lu_PlayerScript.h"

namespace Lu
{
	NextFloorScene::NextFloorScene()
	{
		SetName(L"NextFloorSceneScript");
	}

	NextFloorScene::~NextFloorScene()
	{
	}

	void NextFloorScene::Initialize()
	{
		StageScene::Initialize();

		// Back : 크기 원본 2배
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 900.f), Vector3(2016.f, 1344.f, 100.f), eLayerType::BackGround);
			pObject->SetName(L"NextFloor_BG");

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			// 같은 이미지라서 돌려씀
			pMeshRender->SetMaterial(Resources::Find<Material>(L"WeaponChoiceBG_Mtrl"));
		}

		// 포탈
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 550.f, 500.f), Vector3(200.f, 20.f, 100.f), eLayerType::Portal);
			pObject->SetName(L"Portal");

			pObject->AddComponent<Collider2D>();

			PortalScript* pPortal = pObject->AddComponent<PortalScript>();
			pPortal->SetCurSceneName(L"NextFloorScene");
			pPortal->SetNextSceneName(L"MidBossScene");
			pPortal->Initialize();
		}

		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 300.f), Vector3(288.f, 288.f, 100.f), eLayerType::Portal);
			pObject->SetName(L"MagicCircle_Portal");

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"PortalAnimation_Mtrl"));
			
			Collider2D* pCollider = pObject->AddComponent<Collider2D>();
			pCollider->SetSize(Vector2(0.5f, 0.2f));
			pCollider->SetCenter(Vector2(0.f, -90.f));

			pObject->AddComponent<Animator>();
			pObject->AddComponent<MagicCircleScript>();
		}

		// 카메라 설정
		GameObject* pPlayer = SceneManager::FindPlayer();
		CameraScript* pMainCamScript = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
		pMainCamScript->SetWorldResolution(Vector2(2016.f - 400.f, 1344.f - 200.f));
		pMainCamScript->SetTarget(pPlayer);

		// 바닥 재질
		GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, -40.f, 900.f), Vector3(840.f, 620.f, 500.f), eLayerType::Ground);
		pObject->SetName(L"Wood");
		pObject->AddComponent<Collider2D>();
		pObject->AddComponent<WoodScript>()->SetPlayer(pPlayer->GetComponent<PlayerScript>());
	}

	void NextFloorScene::Update()
	{
		StageScene::Update();
	}

	void NextFloorScene::LateUpdate()
	{
		StageScene::LateUpdate();
	}

	void NextFloorScene::Render()
	{
		StageScene::Render();
	}

	void NextFloorScene::Destroy()
	{
		StageScene::Destroy();
	}

	void NextFloorScene::OnEnter()
	{
		StageScene::OnEnter();
	}

	void NextFloorScene::OnExit()
	{
		StageScene::OnExit();
	}
}