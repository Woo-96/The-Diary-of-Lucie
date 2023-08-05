#include "Lu_LobbyScene.h"
#include "Lu_Renderer.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_Object.h"
#include "Lu_Input.h"
#include "Lu_Animator.h"
#include "Lu_PlayerScript.h"
#include "Lu_CameraScript.h"
#include "Lu_ImmovableScript.h"
#include "Lu_CollisionManager.h"

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


			Animator* pAnimator = pPlayer->AddComponent<Animator>();
			pPlayer->AddComponent<PlayerScript>();

			//CameraScript* pMainCamScript = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
			//pMainCamScript->SetTarget(pPlayer);
			//pMainCamScript->SetWorldResolution(Vector2(1008.f + 440.f, 1056.f * 1.5f - 600.f));
		}

		// 책상
		{
			GameObject* pObj = object::Instantiate<GameObject>(Vector3(3.f, -80.f, 500.f), Vector3(220.f, 120.f, 100.f), eLayerType::Immovable);
			pObj->SetName(L"Desk");

			Collider2D* pCollider = pObj->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);

			pObj->AddComponent<ImmovableScript>();
		}

		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Immovable, true);
	}

	void LobbyScene::Update()
	{
		Scene::Update();

		// 오브젝트로 입장해야함
		if (Input::GetKeyUp(eKeyCode::ENTER))
		{
			SceneManager::LoadScene(L"WeaponChoiceScene");
		}
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
	}

	void LobbyScene::OnExit()
	{

	}
}