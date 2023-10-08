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

namespace Lu
{
	BossScene::BossScene()
		: m_Boss(nullptr)
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
			m_Boss = object::Instantiate<GameObject>(Vector3(-50.f, 600.f, 500.f), Vector3(483.f, 525.f, 100.f), eLayerType::Monster);
			m_Boss->SetName(L"Ent");

			MeshRenderer* pMeshRender = m_Boss->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"EntAnimation_Mtrl"));

			Collider2D* pCollider = m_Boss->AddComponent<Collider2D>();
			pCollider->SetCenter(Vector2(0.f, -200.f));
			pCollider->SetSize(Vector2(0.5f, 0.4f));

			m_Boss->AddComponent<Animator>();

			EntScript* pEntScript = m_Boss->AddComponent<EntScript>();
			pEntScript->SetTarget(pPlayer->GetComponent<PlayerScript>());
		}
	}

	void BossScene::Update()
	{
		StageScene::Update();

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

		AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetBGM();
		pBGM->SetClip(Resources::Find<AudioClip>(L"BossBGM"));
		pBGM->Play();
		pBGM->SetLoop(true);
		pBGM->SetVolume(0.3f);
	}

	void BossScene::OnExit()
	{
		StageScene::OnExit();

		AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetBGM();
		pBGM->Stop();
	}
}