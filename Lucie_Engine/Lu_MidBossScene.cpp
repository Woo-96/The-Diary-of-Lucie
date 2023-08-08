#include "Lu_MidBossScene.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_Collider2D.h"
#include "Lu_KingSlimeScript.h"

#include "Lu_Input.h"

namespace Lu
{
	MidBossScene::MidBossScene()
	{
		SetName(L"MidBossSceneScript");
	}

	MidBossScene::~MidBossScene()
	{
	}

	void MidBossScene::Initialize()
	{
		StageScene::Initialize();

		// Back : 크기 원본 2배
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 900.f), Vector3(2208.f, 1920.f, 100.f), eLayerType::BackGround);
			pObject->SetName(L"MidBoss_BG");

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"MidBossBG_Mtrl"));
		}

		// UI : 크기 원본 1.5배
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 350.f, 100.f), Vector3(759.f, 48.f, 100.f), eLayerType::UI);
			pObject->SetName(L"MidBoss_HP_Frame");

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"MidBossHPFrame_Mtrl"));
		}

		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 350.f, 100.f), Vector3(720.f, 30.f, 100.f), eLayerType::UI);
			pObject->SetName(L"MidBoss_HP");

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"BossHP_Mtrl"));
		}

		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 0.f), Vector3(1440.f, 810.f, 100.f), eLayerType::UI);
			pObject->SetName(L"MidBoss_Name");

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"MidBossName_Mtrl"));
		}

		// KingSlime
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 200.f, 500.f), Vector3(720.f, 720.f, 100.f), eLayerType::Monster);
			pObject->SetName(L"KingSlime");

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"KingSlimeAnimation_Mtrl"));

			Collider2D* pCollider = pObject->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);
			pCollider->SetCenter(Vector2(0.f, -200.f));
			pCollider->SetSize(Vector2(0.5f, 0.4f));

			Animator* pAnimator = pObject->AddComponent<Animator>();
			KingSlimeScript* pKingSlimeScript = pObject->AddComponent<KingSlimeScript>();
			//pKingSlimeScript->SetTarget(pPlayerScript);
		}
	}

	void MidBossScene::Update()
	{
		StageScene::Update();

		// 임시
		if (Input::GetKeyUp(eKeyCode::ENTER))
		{
			SceneManager::LoadScene(L"NextFloorScene");
		}
	}

	void MidBossScene::LateUpdate()
	{
		StageScene::LateUpdate();
	}

	void MidBossScene::Render()
	{
		StageScene::Render();
	}

	void MidBossScene::Destroy()
	{
		StageScene::Destroy();
	}

	void MidBossScene::OnEnter()
	{
		StageScene::OnEnter();
	}

	void MidBossScene::OnExit()
	{

	}
}