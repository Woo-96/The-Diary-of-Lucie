#include "Lu_MidBossScene.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_Collider2D.h"
#include "Lu_KingSlimeScript.h"
#include "Lu_Time.h"
#include "Lu_Object.h"
#include "Lu_CameraScript.h"
#include "Lu_Renderer.h"
#include "Lu_AudioSource.h"
#include "Lu_AudioListener.h"

#include "Lu_Input.h"

namespace Lu
{
	MidBossScene::MidBossScene()
		: m_BossName(nullptr)
		, m_Time(0.f)
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
		
			pObject->AddComponent<AudioListener>();
			AudioSource* BGM = pObject->AddComponent<AudioSource>();
			BGM->SetClip(Resources::Load<AudioClip>(L"MidBossBGM", L"..\\Resources\\Sound\\BGM\\MidBossBGM.ogg"));
			SetBGM(BGM);
		}

		// UI : 크기 원본 1.5배
		{
			m_BossName = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 0.f), Vector3(1440.f, 810.f, 100.f), eLayerType::UI);
			m_BossName->SetName(L"MidBoss_Name");

			MeshRenderer* pMeshRender = m_BossName->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"MidBossName_Mtrl"));
		}

		// Player & KingSlime
		{
			GameObject* pPlayer = object::Instantiate<GameObject>(Vector3(600.f, -150.f, 500.f), Vector3(200.f, 200.f, 100.f), eLayerType::Player);
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
			PlayerScript* pPlayerScript = pPlayer->AddComponent<PlayerScript>();

			CameraScript* pMainCamScript = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
			pMainCamScript->SetWorldResolution(Vector2(2208.f, 1920.f));
			pMainCamScript->SetTarget(pPlayer);


			// Boss
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 200.f, 500.f), Vector3(720.f, 720.f, 100.f), eLayerType::Monster);
			pObject->SetName(L"KingSlime");

			pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"SlimeAnimation_Mtrl"));

			pCollider = pObject->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);
			pCollider->SetCenter(Vector2(0.f, -200.f));
			pCollider->SetSize(Vector2(0.5f, 0.4f));

			pAnimator = pObject->AddComponent<Animator>();
			KingSlimeScript* pKingSlimeScript = pObject->AddComponent<KingSlimeScript>();
			pKingSlimeScript->SetTarget(pPlayerScript);
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

		// 임시, 이름이 서서히 나타났다가 서서히 사라져야하는 것으로 알고있음..
		if (nullptr != m_BossName)
		{
			m_Time += (float)Time::DeltaTime();
			if (3.f <= m_Time)
			{
				m_Time = 0.f;
				object::Destroy(m_BossName);
				m_BossName = nullptr;
			}
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
		StageScene::OnExit();
	}
}