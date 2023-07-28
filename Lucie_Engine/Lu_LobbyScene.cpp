#include "Lu_LobbyScene.h"
#include "Lu_Renderer.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_Object.h"
#include "Lu_Input.h"
#include "Lu_Animator.h"
#include "Lu_PlayerScript.h"

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

		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, -95.f, 900.f), eLayerType::BackGround);
			pObject->SetName(L"Lobby_BG");

			Transform* pTransform = pObject->GetComponent<Transform>();
			// 원본 2.5배
			pTransform->SetScale(Vector3(2520.f, 2640.f, 100.f));

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"LobbyBG_Mtrl"));
		}

		{
			GameObject* pPlayer = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 500.f), eLayerType::Player);
			pPlayer->SetName(L"Player");

			Transform* pTransform = pPlayer->GetComponent<Transform>();
			pTransform->SetScale(Vector3(200.f, 200.f, 100.f)); // 원본 2배

			MeshRenderer* pMeshRender = pPlayer->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"SpriteAnimaionMaterial"));

			Collider2D* pCollider = pPlayer->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);
			pCollider->SetCenter(Vector2(0.f, -30.f));
			pCollider->SetSize(Vector2(0.1f, 0.1f));

			Animator* pAnimator = pPlayer->AddComponent<Animator>();
			pPlayer->AddComponent<PlayerScript>();
		}
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