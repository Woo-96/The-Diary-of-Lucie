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
		}

		// UI : 크기 원본 1.5배
		{
			m_BossName = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 0.f), Vector3(1440.f, 810.f, 100.f), eLayerType::UI);
			m_BossName->SetName(L"MidBoss_Name");

			MeshRenderer* pMeshRender = m_BossName->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"MidBossName_Mtrl"));
		}

		// KingSlime
		{
			GameObject* pPlayer = SceneManager::FindPlayer();
			PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();

			CameraScript* pMainCamScript = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
			pMainCamScript->SetWorldResolution(Vector2(2208.f - 400.f, 1920.f - 200.f));
			pMainCamScript->SetTarget(pPlayer);


			// Boss
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 200.f, 500.f), Vector3(720.f, 720.f, 100.f), eLayerType::Monster);
			pObject->SetName(L"KingSlime");

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"SlimeAnimation_Mtrl"));

			Collider2D* pCollider = pObject->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);
			pCollider->SetCenter(Vector2(0.f, -200.f));
			pCollider->SetSize(Vector2(0.5f, 0.4f));

			Animator* pAnimator = pObject->AddComponent<Animator>();
			KingSlimeScript* pKingSlimeScript = pObject->AddComponent<KingSlimeScript>();
			pKingSlimeScript->SetTarget(pPlayerScript);
		}

		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(870.f, -110.f, 500.f), Vector3(20.f, 180.f, 100.f), eLayerType::Portal);
			pObject->SetName(L"Portal");

			Collider2D* pCollider = pObject->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);

			PortalScript* pPortal = pObject->AddComponent<PortalScript>();
			pPortal->SetCurSceneName(L"MidBossScene");
			pPortal->SetNextSceneName(L"Nomal2Scene");
			pPortal->Initialize();
		}
	}

	void MidBossScene::Update()
	{
		StageScene::Update();

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

		// 임시임. 구조 바꿔야함
		AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetBGM();
		pBGM->Stop();

		//AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetBGM();
		pBGM->SetClip(Resources::Find<AudioClip>(L"MidBossBGM"));
		pBGM->Play();
		pBGM->SetVolume(0.3f);
	}

	void MidBossScene::OnExit()
	{
		StageScene::OnExit();

		AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetBGM();
		pBGM->Stop();
	}
}