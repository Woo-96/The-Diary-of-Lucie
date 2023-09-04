#include "Lu_Nomal1Scene.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_Animator.h"
#include "Lu_PlayerScript.h"
#include "Lu_SnabyScript.h"
#include "Lu_CameraScript.h"
#include "Lu_Renderer.h"
#include "Lu_ImmovableScript.h"

#include "Lu_Input.h"

namespace Lu
{
	Nomal1Scene::Nomal1Scene()
	{
		SetName(L"Nomal1SceneScript");
	}

	Nomal1Scene::~Nomal1Scene()
	{
	}

	void Nomal1Scene::Initialize()
	{
		StageScene::Initialize();

		// Back : 크기 원본 2배
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 900.f), Vector3(2016.f, 1536.f, 100.f), eLayerType::BackGround);
			pObject->SetName(L"Nomal1_BG");

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Nomal1BG_Mtrl"));
		}

		// Monster : 크기 원본 2배
		{
			GameObject* pPlayer = SceneManager::FindPlayer();
			PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();

			CameraScript* pMainCamScript = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
			pMainCamScript->SetWorldResolution(Vector2(1008.f + 440.f, 1056.f * 1.5f - 600.f));
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

		// 맵 가운데 나무
		{
			GameObject* pObj = object::Instantiate<GameObject>(Vector3(20.f, 50.f, 500.f), Vector3(80.f, 180.f, 100.f), eLayerType::Immovable);
			pObj->SetName(L"RootOfTree_Big");

			Collider2D* pCollider = pObj->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);

			pObj->AddComponent<ImmovableScript>();
		}
		{
			GameObject* pObj = object::Instantiate<GameObject>(Vector3(75.f, -30.f, 500.f), Vector3(80.f, 80.f, 100.f), eLayerType::Immovable);
			pObj->SetName(L"RootOfTree_Small");

			Collider2D* pCollider = pObj->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);

			pObj->AddComponent<ImmovableScript>();
		}
	}

	void Nomal1Scene::Update()
	{
		StageScene::Update();

		// 임시
		if (Input::GetKeyUp(eKeyCode::ENTER))
		{
			SceneManager::LoadScene(L"Nomal2Scene");
		}
	}

	void Nomal1Scene::LateUpdate()
	{
		StageScene::LateUpdate();
	}

	void Nomal1Scene::Render()
	{
		StageScene::Render();
	}

	void Nomal1Scene::Destroy()
	{
		StageScene::Destroy();
	}

	void Nomal1Scene::OnEnter()
	{
		StageScene::OnEnter();
	}

	void Nomal1Scene::OnExit()
	{
		StageScene::OnExit();
	}
}