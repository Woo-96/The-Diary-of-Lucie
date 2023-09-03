#include "Lu_Nomal2Scene.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_PlayerScript.h"
#include "Lu_CameraScript.h"
#include "Lu_Renderer.h"
#include "Lu_SlimeScript.h"
#include "Lu_AudioListener.h"
#include "Lu_AudioSource.h"

#include "Lu_Input.h"

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
		StageScene::Initialize();

		// Back : 크기 원본 2배
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 900.f), Vector3(2016.f, 1344.f, 100.f), eLayerType::BackGround);
			pObject->SetName(L"Nomal2_BG");

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Nomal2BG_Mtrl"));
		
			pObject->AddComponent<AudioListener>();
			AudioSource* BGM = pObject->AddComponent<AudioSource>();
			BGM->SetClip(Resources::Find<AudioClip>(L"ForestBGM"));
			SetBGM(BGM);
			SetContinuousPlay(true);
		}

		// Player : 크기 원본 2배
		{
			//GameObject* pPlayer = object::Instantiate<GameObject>(Vector3(0.f, 400.f, 500.f), Vector3(200.f, 200.f, 100.f), eLayerType::Player);
			//pPlayer->SetName(L"Player");

			//MeshRenderer* pMeshRender = pPlayer->AddComponent<MeshRenderer>();
			//pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			//pMeshRender->SetMaterial(Resources::Find<Material>(L"PlayerAnimation_Mtrl"));

			//// 피격 판정용 충돌체
			//Collider2D* pCollider = pPlayer->AddComponent<Collider2D>();
			//pCollider->SetName(L"ImmovableCollider");
			//pCollider->SetType(eColliderType::Rect);
			//pCollider->SetCenter(Vector2(2.f, 3.f));
			//pCollider->SetSize(Vector2(0.2f, 0.42f));

			//pCollider = pPlayer->AddComponent<Collider2D>();
			//pCollider->SetName(L"HitCollider");
			//pCollider->SetType(eColliderType::Rect);
			//pCollider->SetCenter(Vector2(2.f, -29.f));
			//pCollider->SetSize(Vector2(0.1f, 0.1f));

			//pPlayer->AddComponent<Animator>();
			//PlayerScript* pPlayerScript = pPlayer->AddComponent<PlayerScript>();

			GameObject* pPlayer = SceneManager::FindPlayer();
			PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();

			CameraScript* pMainCamScript = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
			pMainCamScript->SetWorldResolution(Vector2(1008.f + 440.f, 1056.f * 1.5f - 600.f));
			pMainCamScript->SetTarget(pPlayer);


			// Monster : 크기 원본 2배
			GameObject* pMonster = object::Instantiate<GameObject>(Vector3(-40.f, 100.f, 500.f), Vector3(180.f, 180.f, 100.f), eLayerType::Monster);
			pMonster->SetName(L"Slime");

			MeshRenderer* pMeshRender = pMonster->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"SlimeAnimation_Mtrl"));

			Collider2D* pCollider = pMonster->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);
			pCollider->SetCenter(Vector2(0.f, -60.f));
			pCollider->SetSize(Vector2(0.4f, 0.3f));

			pMonster->AddComponent<Animator>();
			SlimeScript* pSlimeScript = pMonster->AddComponent<SlimeScript>();
			pSlimeScript->SetTarget(pPlayerScript);


			pMonster = object::Instantiate<GameObject>(Vector3(20.f, -150.f, 500.f), Vector3(180.f, 180.f, 100.f), eLayerType::Monster);
			pMonster->SetName(L"Slime2");

			pMeshRender = pMonster->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"SlimeAnimation_Mtrl"));

			pCollider = pMonster->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);
			pCollider->SetCenter(Vector2(0.f, -60.f));
			pCollider->SetSize(Vector2(0.4f, 0.3f));

			pMonster->AddComponent<Animator>();
			pSlimeScript = pMonster->AddComponent<SlimeScript>();
			pSlimeScript->SetTarget(pPlayerScript);
		}
	}

	void Nomal2Scene::Update()
	{
		StageScene::Update();

		// 임시
		if (Input::GetKeyUp(eKeyCode::ENTER))
		{
			SceneManager::LoadScene(L"MidBossScene");
		}
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
		SetContinuousPlay(false);
		StageScene::OnExit();
	}
}