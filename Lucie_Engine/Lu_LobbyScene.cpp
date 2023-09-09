#include "Lu_LobbyScene.h"
#include "Lu_Renderer.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_Object.h"
#include "Lu_Animator.h"
#include "Lu_PlayerScript.h"
#include "Lu_CameraScript.h"
#include "Lu_ImmovableScript.h"
#include "Lu_AudioClip.h"
#include "Lu_AudioSource.h"
#include "Lu_DiaryScript.h"
#include "Lu_SoundManager.h"

namespace Lu
{
	LobbyScene::LobbyScene()
	{
		SetName(L"LobbySceneScript");
	}

	LobbyScene::~LobbyScene()
	{

	}

	void LobbyScene::Initialize()
	{
		Scene::Initialize();

		// BG : 크기 원본 2.5배
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, -95.f, 900.f), Vector3(2520.f, 2640.f, 100.f), eLayerType::BackGround);
			pObject->SetName(L"Lobby_BG");

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"LobbyBG_Mtrl"));
		}

		// Player : 크기 원본 2배
		{
			GameObject* pPlayer = object::Instantiate<GameObject>(Vector3(400.f, 50.f, 500.f), Vector3(200.f, 200.f, 100.f), eLayerType::Player);
			pPlayer->SetName(L"Player");
			SceneManager::DontDestroyOnLoad(pPlayer);

			MeshRenderer* pMeshRender = pPlayer->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"PlayerAnimation_Mtrl"));

			// 피격 판정용 충돌체
			Collider2D* pCollider = pPlayer->AddComponent<Collider2D>();
			pCollider->SetName(L"ImmovableCollider");
			pCollider->SetType(eColliderType::Rect);
			pCollider->SetCenter(Vector2(2.f, 3.f));
			pCollider->SetSize(Vector2(0.2f, 0.42f));

			pCollider = pPlayer->AddComponent<Collider2D>();
			pCollider->SetName(L"HitCollider");
			pCollider->SetType(eColliderType::Rect);
			pCollider->SetCenter(Vector2(2.f, -29.f));
			pCollider->SetSize(Vector2(0.1f, 0.1f));

			pPlayer->AddComponent<Animator>();
			pPlayer->AddComponent<PlayerScript>();

			CameraScript* pMainCamScript = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
			pMainCamScript->SetWorldResolution(Vector2(1008.f + 440.f, 1056.f * 1.5f - 600.f));
			pMainCamScript->SetTarget(pPlayer);
		}

		// 벽
		{
			GameObject* pObj = object::Instantiate<GameObject>(Vector3(3.f, -80.f, 500.f), Vector3(220.f, 120.f, 100.f), eLayerType::Immovable);
			pObj->SetName(L"Desk");

			Collider2D* pCollider = pObj->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);
			pObj->AddComponent<ImmovableScript>();
		}
		{
			GameObject* pObj = object::Instantiate<GameObject>(Vector3(0.f, 350.f, 500.f), Vector3(1000.f, 300.f, 100.f), eLayerType::Immovable);
			pObj->SetName(L"Wall_Top");

			Collider2D* pCollider = pObj->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);
			pObj->AddComponent<ImmovableScript>();
		}

		{
			GameObject* pObj = object::Instantiate<GameObject>(Vector3(400.f, 150.f, 500.f), Vector3(150.f, 70.f, 100.f), eLayerType::Immovable);
			pObj->SetName(L"Closet");

			Collider2D* pCollider = pObj->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);
			pObj->AddComponent<ImmovableScript>();
		}

		{
			GameObject* pObj = object::Instantiate<GameObject>(Vector3(-492.f, -115.f, 500.f), Vector3(20.f, 630.f, 100.f), eLayerType::Immovable);
			pObj->SetName(L"Wall_Left");

			Collider2D* pCollider = pObj->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);
			pObj->AddComponent<ImmovableScript>();
		}

		{
			GameObject* pObj = object::Instantiate<GameObject>(Vector3(492.f, -115.f, 500.f), Vector3(20.f, 630.f, 100.f), eLayerType::Immovable);
			pObj->SetName(L"Wall_Right");

			Collider2D* pCollider = pObj->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);
			pObj->AddComponent<ImmovableScript>();
		}

		{
			GameObject* pObj = object::Instantiate<GameObject>(Vector3(0.f, -435.f, 500.f), Vector3(1000.f, 20.f, 100.f), eLayerType::Immovable);
			pObj->SetName(L"Wall_Bottom");

			Collider2D* pCollider = pObj->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);
			pObj->AddComponent<ImmovableScript>();
		}

		// 다이어리(포탈)
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, -110.f, 500.f), Vector3(96.f, 72.f, 100.f), eLayerType::Portal);
			pObject->SetName(L"Diary");

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"DiaryAnimation_Mtrl"));

			Collider2D* pCollider = pObject->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);
			pCollider->SetCenter(Vector2(0.f, -15.f));
			pCollider->SetSize(Vector2(0.35f, 0.57f));

			pObject->AddComponent<Animator>();
			pObject->AddComponent<DiaryScript>();
		}
	}

	void LobbyScene::Update()
	{
		Scene::Update();
	}

	void LobbyScene::LateUpdate()
	{
		Scene::LateUpdate();
	}

	void LobbyScene::Render()
	{
		Scene::Render();
	}

	void LobbyScene::Destroy()
	{
		Scene::Destroy();
	}

	void LobbyScene::OnEnter()
	{
		Scene::OnEnter();

		SceneManager::DontUseOnLoad(eLayerType::UI);

		AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetBGM();
		pBGM->SetClip(Resources::Find<AudioClip>(L"LobbyBGM"));
		pBGM->Play();
		pBGM->SetVolume(0.3f);
	}

	void LobbyScene::OnExit()
	{
		Scene::OnExit();

		AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetBGM();
		pBGM->Stop();

		// 로비로 다시 못가니까 여기서 켜줌
		pBGM->SetClip(Resources::Find<AudioClip>(L"ForestBGM"));
		pBGM->Play();
		pBGM->SetVolume(0.3f);
	}
}