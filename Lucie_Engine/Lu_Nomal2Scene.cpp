#include "Lu_Nomal2Scene.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_PlayerScript.h"
#include "Lu_CameraScript.h"
#include "Lu_Renderer.h"
#include "Lu_SnabyScript.h"
#include "Lu_PortalScript.h"

namespace Lu
{
	Nomal2Scene::Nomal2Scene()
	{
		SetName(L"Nomal2SceneScript");
	}

	Nomal2Scene::~Nomal2Scene()
	{
	}

	void Nomal2Scene::Initialize()
	{
		Scene::Initialize();

		// Back : 크기 원본 2배
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 900.f), Vector3(2016.f, 1344.f, 100.f), eLayerType::BackGround);
			pObject->SetName(L"Nomal2_BG");

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Nomal2BG_Mtrl"));
		}


		// Monster : 크기 원본 2배
		{
			GameObject* pPlayer = SceneManager::FindPlayer();
			PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();

			CameraScript* pMainCamScript = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
			pMainCamScript->SetWorldResolution(Vector2(2016.f - 400.f, 1344.f - 100.f));
			pMainCamScript->SetTarget(pPlayer);


			GameObject* pMonster = object::Instantiate<GameObject>(Vector3(-200.f, 200.f, 500.f), Vector3(96.f, 96.f, 100.f), eLayerType::Monster);
			pMonster->SetName(L"Snaby");

			MeshRenderer* pMeshRender = pMonster->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"SnabyAnimation_Mtrl"));

			Collider2D* pCollider = pMonster->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);
			pCollider->SetCenter(Vector2(3.f, -7.f));
			pCollider->SetSize(Vector2(0.3f, 0.8f));

			pMonster->AddComponent<Animator>();
			SnabyScript* pSnabyScript = pMonster->AddComponent<SnabyScript>();
			pSnabyScript->SetTarget(pPlayerScript);


			pMonster = object::Instantiate<GameObject>(Vector3(250.f, -250.f, 500.f), Vector3(96.f, 96.f, 100.f), eLayerType::Monster);
			pMonster->SetName(L"Snaby2");

			pMeshRender = pMonster->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"SnabyAnimation_Mtrl"));

			pCollider = pMonster->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);
			pCollider->SetCenter(Vector2(3.f, -7.f));
			pCollider->SetSize(Vector2(0.3f, 0.8f));

			pMonster->AddComponent<Animator>();
			pSnabyScript = pMonster->AddComponent<SnabyScript>();
			pSnabyScript->SetTarget(pPlayerScript);
		}

		// 포탈
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, -600.f, 500.f), Vector3(250.f, 20.f, 100.f), eLayerType::Portal);
			pObject->SetName(L"Portal1");

			Collider2D* pCollider = pObject->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);

			PortalScript* pPortal = pObject->AddComponent<PortalScript>();
			pPortal->SetCurSceneName(L"Nomal2Scene");
			pPortal->SetNextSceneName(L"Nomal1Scene");
			pPortal->Initialize();
		}

		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(-780.f, -60.f, 500.f), Vector3(20.f, 200.f, 100.f), eLayerType::Portal);
			pObject->SetName(L"Portal2");

			Collider2D* pCollider = pObject->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);

			PortalScript* pPortal = pObject->AddComponent<PortalScript>();
			pPortal->SetCurSceneName(L"Nomal2Scene");
			pPortal->SetNextSceneName(L"MidBossScene");
			pPortal->Initialize();
		}
	}

	void Nomal2Scene::Update()
	{
		StageScene::Update();
	}

	void Nomal2Scene::LateUpdate()
	{
		StageScene::LateUpdate();
	}

	void Nomal2Scene::Render()
	{
		StageScene::Render();
	}

	void Nomal2Scene::Destroy()
	{
		StageScene::Destroy();
	}

	void Nomal2Scene::OnEnter()
	{
		StageScene::OnEnter();
	}

	void Nomal2Scene::OnExit()
	{
		StageScene::OnExit();
	}
}