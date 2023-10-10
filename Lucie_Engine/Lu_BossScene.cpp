#include "Lu_BossScene.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_SoundManager.h"
#include "Lu_AudioSource.h"
#include "Lu_CameraScript.h"
#include "Lu_Renderer.h"
#include "Lu_Input.h"
#include "Lu_EntScript.h"
#include "Lu_TriggerScript.h"
#include "Lu_Time.h"
#include "Lu_PlayerScript.h"
#include "Lu_LabelScript.h"

namespace Lu
{
	BossScene::BossScene()
		: m_Boss(nullptr)
		, m_DramaFX(nullptr)
		, m_bBossWakeUp(false)
		, m_bFirst(false)
		, m_Time(0.f)
		, m_CameraScale(0.f)
	{
		SetName(L"BossSceneScript");
	}

	BossScene::~BossScene()
	{
	}

	void BossScene::Initialize()
	{
		StageScene::Initialize();

		// Back크기 원본 2배
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 900.f), Vector3(2688.f, 2400.f, 100.f), eLayerType::BackGround); 
			pObject->SetName(L"Boss_BG");

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"BossBG_Mtrl"));
		}

		{
			// 카메라 설정
			GameObject* pPlayer = SceneManager::FindPlayer();
			CameraScript* pMainCamScript = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
			pMainCamScript->SetWorldResolution(Vector2(2688.f, 2400.f));
			pMainCamScript->SetTarget(pPlayer);

			// Boss
			m_Boss = object::Instantiate<GameObject>(Vector3(-50.f, 500.f, 500.f), Vector3(644.f, 700.f, 100.f), eLayerType::Monster);
			m_Boss->SetName(L"Ent");

			MeshRenderer* pMeshRender = m_Boss->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"EntAnimation_Mtrl"));

			Collider2D* pCollider = m_Boss->AddComponent<Collider2D>();
			pCollider->SetCenter(Vector2(0.f, -300.f));
			pCollider->SetSize(Vector2(0.4f, 0.1f));

			Animator* pAnimator = m_Boss->AddComponent<Animator>();

			EntScript* pEntScript = m_Boss->AddComponent<EntScript>();
			pEntScript->SetTarget(pPlayer->GetComponent<PlayerScript>());
		}

		// 보스를 깨우는 트리거
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, -300.f, 900.f), Vector3(2000.f, 10.f, 50.f), eLayerType::Ground);
			pObject->SetName(L"Boss_Trigger");

			pObject->AddComponent<Collider2D>();
			TriggerScript* pTrigger = pObject->AddComponent<TriggerScript>();
			pTrigger->AddDelegate(this, (TriggerScript::Scene_DELEGATE)&BossScene::BossWakeUp);
		}
	}

	void BossScene::Update()
	{
		StageScene::Update();

		if (m_bBossWakeUp)
		{
			m_Time += (float)Time::DeltaTime();

			if (m_Time >= 2.f && m_Time < 2.1f)
			{
				m_Boss->GetComponent<EntScript>()->WakeUp();
			}
			else if (m_Time >= 4.0f && m_Time < 4.1f)
			{
				if (!m_bFirst)
				{
					// 보스 소리 지름
					m_Boss->GetComponent<EntScript>()->Howling();

					// 카메라 쉐이킹
					renderer::mainCamera->GetOwner()->GetComponent<CameraScript>()->RequestCameraShaking(10.f, 1.5f);

					m_bFirst = true;
				}
			}
			else if (m_Time >= 5.5f)
			{
				if (m_bFirst)
				{
					// 드라마 FX 제거
					object::Destroy(m_DramaFX);
					m_DramaFX = nullptr;

					// 보스 이름 UI : 원본 크기 1.5배
					GameObject* pBossName = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 50.f), Vector3(1440.f, 810.f, 100.f), eLayerType::UI);
					pBossName->SetName(L"Boss_Name");

					MeshRenderer* pMeshRender = pBossName->AddComponent<MeshRenderer>();
					pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
					pMeshRender->SetMaterial(Resources::Find<Material>(L"BossName_Mtrl"));

					LabelScript* pLabel = pBossName->AddComponent<LabelScript>();
					pLabel->SetMeshRender(pMeshRender);
					pLabel->SetDuration(3.f);

					// 플레이어 정상화
					GameObject* pPlayer = SceneManager::FindPlayer();
					PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();
					pPlayerScript->SetAction(false);

					// 카메라 타겟 및 타겟 이동 변경
					CameraScript* pMainCam = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
					pMainCam->SetTarget(pPlayer);

					// BGM 재생
					AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetBGM();
					pBGM->SetClip(Resources::Find<AudioClip>(L"BossBGM"));
					pBGM->Play();
					pBGM->SetLoop(true);
					pBGM->SetVolume(0.3f);

					m_CameraScale = 1.2f;
					m_bFirst = false;
				}

				// 카메라 배율 원상 복구
				m_CameraScale -= (float)Time::DeltaTime();
				if (m_CameraScale <= 1.f)
				{
					Camera* pCamera = renderer::mainCamera;
					pCamera->SetScale(1.f);
					CameraScript* pMainCam = pCamera->GetOwner()->GetComponent<CameraScript>();
					pMainCam->SetTargetMove(true);
					pMainCam->SetOffset(Vector2(0.f, 200.f));

					m_bBossWakeUp = false;
					m_Time = 0.f;
				}
				else
				{
					renderer::mainCamera->SetScale(m_CameraScale);
				}
			}
		}

		if (Input::GetKeyUp(eKeyCode::ENTER))
		{
			SceneManager::LoadScene(L"EndingScene");
		}	
	}

	void BossScene::LateUpdate()
	{
		StageScene::LateUpdate();
	}

	void BossScene::Render()
	{
		StageScene::Render();
	}

	void BossScene::Destroy()
	{
		StageScene::Destroy();
	}

	void BossScene::OnEnter()
	{
		StageScene::OnEnter();

		CameraScript* pMainCam = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
		renderer::mainCamera->SetScale(1.2f);
	}

	void BossScene::OnExit()
	{
		StageScene::OnExit();

		AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetBGM();
		pBGM->Stop();
	}

	void BossScene::BossWakeUp()
	{
		m_bBossWakeUp = true;

		AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetBGM();
		pBGM->Stop();

		// 플레이어 멈춤
		GameObject* pPlayer = SceneManager::FindPlayer();
		PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();
		pPlayerScript->SetAction(true);
		pPlayerScript->ChangeState(StateScript::eState::Idle);

		// 카메라 타겟 및 타겟 이동 변경
		CameraScript* pMainCam = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
		pMainCam->SetTargetMove(false);
		pMainCam->SetTarget(m_Boss);
		pMainCam->SetOffset(Vector2(0.f, -150.f));
		pMainCam->SetSpeed(1.f);

		// 드라마 FX
		m_DramaFX = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 10.f), Vector3(1440.f, 810.f, 100.f), eLayerType::UI);
		m_DramaFX->SetName(L"DramaFX");

		MeshRenderer* pMeshRender = m_DramaFX->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		pMeshRender->SetMaterial(Resources::Find<Material>(L"Drama_Mtrl"));

		pMeshRender = m_DramaFX->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		pMeshRender->SetMaterial(Resources::Find<Material>(L"SkillBG_Mtrl"));
	}
}