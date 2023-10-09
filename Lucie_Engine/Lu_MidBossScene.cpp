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
#include "Lu_SoundManager.h"
#include "Lu_PortalScript.h"
#include "Lu_LabelScript.h"
#include "Lu_ChestScript.h"
#include "Lu_ImmovableScript.h"
#include "Lu_PlayerScript.h"

namespace Lu
{
	MidBossScene::MidBossScene()
		: m_Boss(nullptr)
		, m_DramaFX(nullptr)
		, m_ChestFX(nullptr)
		, m_bBossAlive(true)
		, m_bCameraMove(false)
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
		}

		// KingSlime
		{
			GameObject* pPlayer = SceneManager::FindPlayer();
			PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();

			CameraScript* pMainCamScript = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
			pMainCamScript->SetWorldResolution(Vector2(2208.f - 400.f, 1920.f - 200.f));
			pMainCamScript->SetTarget(pPlayer);


			// Boss
			m_Boss = object::Instantiate<GameObject>(Vector3(0.f, 200.f, 500.f), Vector3(720.f, 720.f, 100.f), eLayerType::Monster);
			m_Boss->SetName(L"KingSlime");

			MeshRenderer* pMeshRender = m_Boss->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"SlimeAnimation_Mtrl"));

			Collider2D* pCollider = m_Boss->AddComponent<Collider2D>();
			pCollider->SetCenter(Vector2(0.f, -200.f));
			pCollider->SetSize(Vector2(0.5f, 0.4f));

			Animator* pAnimator = m_Boss->AddComponent<Animator>();
			KingSlimeScript* pKingSlimeScript = m_Boss->AddComponent<KingSlimeScript>();
			pKingSlimeScript->SetTarget(pPlayerScript);
		}

		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(-870.f, -95.f, 500.f), Vector3(20.f, 180.f, 100.f), eLayerType::Portal);
			pObject->SetName(L"Portal");

			Collider2D* pCollider = pObject->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);

			PortalScript* pPortal = pObject->AddComponent<PortalScript>();
			pPortal->SetCurSceneName(L"MidBossScene");
			pPortal->SetNextSceneName(L"Nomal2Scene");
			pPortal->Initialize();
		}

		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, -820.f, 500.f), Vector3(240.f, 20.f, 100.f), eLayerType::Portal);
			pObject->SetName(L"Portal2");

			Collider2D* pCollider = pObject->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);

			PortalScript* pPortal = pObject->AddComponent<PortalScript>();
			pPortal->SetCurSceneName(L"MidBossScene");
			pPortal->SetNextSceneName(L"NextFloorScene");
			pPortal->Initialize();
		}
	}

	void MidBossScene::Update()
	{
		StageScene::Update();

		if (m_bCameraMove)
		{
			m_Time += (float)Time::DeltaTime();
			if (m_Time >= 3.f)
			{
				GameObject* pPlayer = SceneManager::FindPlayer();
				PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();
				pPlayerScript->SetAction(false);

				CameraScript* pMainCam = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
				pMainCam->SetTarget(pPlayer);
				pMainCam->SetOffset(Vector2(0.f, 0.f));
				renderer::mainCamera->SetScale(1.f);

				object::Destroy(m_DramaFX);
				m_DramaFX = nullptr;
				m_bCameraMove = false;
				m_Time = 0.f;
			}
			else
			{
				float fScale = renderer::mainCamera->GetScale();
				fScale += (float)Time::DeltaTime() / 10.f;
				renderer::mainCamera->SetScale(fScale);
			}
		}

		if (m_bBossAlive)
		{
			if (!IsInBattle())
			{
				m_bBossAlive = false;
				CreateBossReward();

				AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetBGM();
				pBGM->Stop();

				AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
				pSFX->SetClip(Resources::Load<AudioClip>(L"BossDeadSFX", L"..\\Resources\\Sound\\SFX\\Monster\\BossDeadSFX.ogg"));
				pSFX->Play();
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

		if (IsInBattle())
		{
			// 카메라 설정
			m_bCameraMove = true;

			GameObject* pPlayer = SceneManager::FindPlayer();
			PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();
			pPlayerScript->SetAction(true);
			pPlayerScript->ChangeState(StateScript::eState::Idle);

			CameraScript* pMainCam = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
			pMainCam->SetTarget(m_Boss);
			pMainCam->SetOffset(Vector2(0.f, -200.f));

			AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetBGM();
			pBGM->Stop();

			pBGM->SetClip(Resources::Find<AudioClip>(L"MidBossBGM"));
			pBGM->Play();
			pBGM->SetLoop(true);
			pBGM->SetVolume(0.3f);

			// 보스 이름 UI : 원본 크기 1.5배
			GameObject* pBossName = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 50.f), Vector3(1440.f, 810.f, 100.f), eLayerType::UI);
			pBossName->SetName(L"MidBoss_Name");

			MeshRenderer* pMeshRender = pBossName->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"MidBossName_Mtrl"));

			LabelScript* pLabel = pBossName->AddComponent<LabelScript>();
			pLabel->SetMeshRender(pMeshRender);
			pLabel->SetDuration(3.f);

			// 드라마 FX
			m_DramaFX = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 100.f), Vector3(1440.f, 810.f, 100.f), eLayerType::UI);
			m_DramaFX->SetName(L"DramaFX");

			pMeshRender = m_DramaFX->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Drama_Mtrl"));

			// 드라마 재질은 트랜스퍼런트인데 UI가 안가려지는 이상한 버그가 있어서 재질 2개로 생성
			pMeshRender = m_DramaFX->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"SkillBG_Mtrl"));
		}
	}

	void MidBossScene::OnExit()
	{
		StageScene::OnExit();

		AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetBGM();
		pBGM->Stop();
	}

	void MidBossScene::CreateBossReward()
	{
		// 상자 등장 효과
		m_ChestFX = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 500.f), Vector3(288.f, 288.f, 200.f), eLayerType::FX);
		m_ChestFX->SetName(L"ChestOpenFX1");
		MeshRenderer* pMeshRender = m_ChestFX->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		pMeshRender->SetMaterial(Resources::Find<Material>(L"ChestOpenFX1_Mtrl"));
		Animator* pAnimator = m_ChestFX->AddComponent<Animator>();
		pAnimator->Create(L"ChestOpenFX1", Resources::Load<Texture>(L"ChestFX1_Tex", L"..\\Resources\\Texture\\Map\\Stage\\ChestFX1.png")
			, Vector2::Zero, Vector2(192.f, 192.f), 5, Vector2(192.f, 192.f));
		pAnimator->CompleteEvent(L"ChestOpenFX1") = std::bind(&MidBossScene::ChestOpenFXComplete, this);
		pAnimator->PlayAnimation(L"ChestOpenFX1", true);

		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"ChestSFX", L"..\\Resources\\Sound\\SFX\\ChestSFX.ogg"));
		pSFX->Play();
	}

	void MidBossScene::ChestOpenFXComplete()
	{
		object::Destroy(m_ChestFX);
		m_ChestFX = nullptr;

		GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 70.f, 500.f), Vector3(144.f, 144.f, 200.f), eLayerType::Item);
		pObject->SetName(L"Chest");
		MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		pMeshRender->SetMaterial(Resources::Find<Material>(L"Chest_Mtrl"));
		Collider2D* pCollider = pObject->AddComponent<Collider2D>();
		pCollider->SetSize(Vector2(0.4f, 0.4f));
		pCollider->SetCenter(Vector2(0.f, -30.f));
		pObject->AddComponent<Animator>();
		pObject->AddComponent<ChestScript>();
		pObject->AddComponent<ImmovableScript>();
	}
}