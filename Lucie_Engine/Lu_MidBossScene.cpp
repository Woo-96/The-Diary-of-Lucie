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

namespace Lu
{
	MidBossScene::MidBossScene()
		: m_bBossAlive(true)
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

		if (m_bBossAlive)
		{
			AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetBGM();
			pBGM->Stop();

			pBGM->SetClip(Resources::Find<AudioClip>(L"MidBossBGM"));
			pBGM->Play();
			pBGM->SetVolume(0.3f);

			// 보스 이름 UI : 원본 크기 1.5배
			{
				GameObject* pBossName = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 0.f), Vector3(1440.f, 810.f, 100.f), eLayerType::UI);
				pBossName->SetName(L"MidBoss_Name");

				MeshRenderer* pMeshRender = pBossName->AddComponent<MeshRenderer>();
				pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
				pMeshRender->SetMaterial(Resources::Find<Material>(L"MidBossName_Mtrl"));

				LabelScript* pLabel = pBossName->AddComponent<LabelScript>();
				pLabel->SetMeshRender(pMeshRender);
				pLabel->SetDuration(3.f);
			}
		}
	}

	void MidBossScene::OnExit()
	{
		StageScene::OnExit();

		AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetBGM();
		pBGM->Stop();
	}
}