#include "Lu_Nomal1Scene.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_Animator.h"
#include "Lu_CollisionManager.h"
#include "Lu_PlayerScript.h"
#include "Lu_SnabyScript.h"
#include "Lu_CameraScript.h"
#include "Lu_Renderer.h"

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

		// Player : 크기 원본 2배
		{
			GameObject* pPlayer = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 500.f), Vector3(200.f, 200.f, 100.f), eLayerType::Player); 
			pPlayer->SetName(L"Player");

			MeshRenderer* pMeshRender = pPlayer->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"SpriteAnimaionMaterial"));

			Collider2D* pCollider = pPlayer->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);
			pCollider->SetCenter(Vector2(0.f, -30.f));
			pCollider->SetSize(Vector2(0.1f, 0.1f));

			Animator* pAnimator = pPlayer->AddComponent<Animator>();
			pPlayer->AddComponent<PlayerScript>();

			CameraScript* pMainCamScript = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
			pMainCamScript->SetTarget(pPlayer);
			pMainCamScript->SetWorldResolution(Vector2(2016.f, 1536.f));
		}

		// Monster : 크기 원본 2배
		{
			GameObject* pMonster = object::Instantiate<GameObject>(Vector3(150.f, -150.f, 500.f), Vector3(96.f, 96.f, 100.f), eLayerType::Monster);
			pMonster->SetName(L"Snaby");

			MeshRenderer* pMeshRender = pMonster->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"SpriteAnimaionMaterial"));

			Collider2D* pCollider = pMonster->AddComponent<Collider2D>();
			pCollider->SetCenter(Vector2(3.f, -10.f));
			pCollider->SetSize(Vector2(0.3f, 0.7f));

			Animator* pAnimator = pMonster->AddComponent<Animator>();
			pMonster->AddComponent<SnabyScript>();
		}

		{
			GameObject* pMonster = object::Instantiate<GameObject>(Vector3(-150.f, -150.f, 500.f), Vector3(96.f, 96.f, 100.f), eLayerType::Monster);
			pMonster->SetName(L"Snaby2");

			MeshRenderer* pMeshRender = pMonster->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"SpriteAnimaionMaterial"));

			Collider2D* pCollider = pMonster->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);
			pCollider->SetCenter(Vector2(3.f, -10.f));
			pCollider->SetSize(Vector2(0.3f, 0.7f));

			Animator* pAnimator = pMonster->AddComponent<Animator>();
			pMonster->AddComponent<SnabyScript>();
		}

		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Monster, true);	// 나중에 몬스터 투사체 구현하고 바꿔야함
		CollisionManager::SetLayer(eLayerType::PlayerProjectile, eLayerType::Monster, true);
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

	}
}