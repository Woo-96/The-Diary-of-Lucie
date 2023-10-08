#include "Lu_BossWayScene.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_SoundManager.h"
#include "Lu_AudioSource.h"
#include "Lu_CameraScript.h"
#include "Lu_Renderer.h"
#include "Lu_PortalScript.h"
#include "Lu_ImmovableScript.h"

#include "Lu_Input.h"

namespace Lu
{
	BossWayScene::BossWayScene()
	{
		SetName(L"BossWaySceneScript");
	}

	BossWayScene::~BossWayScene()
	{
	}

	void BossWayScene::Initialize()
	{
		Scene::Initialize();

		// Back : 크기 원본 2배
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 900.f), Vector3(2016.f, 1920.f, 100.f), eLayerType::BackGround);
			pObject->SetName(L"BossWay_BG");

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"BossWayBG_Mtrl"));
		}

		{
			GameObject* pObj = object::Instantiate<GameObject>(Vector3(30.f, -200.f, 500.f), Vector3(360.f, 200.f, 100.f), eLayerType::Immovable);
			pObj->SetName(L"RootOfTree1");
			pObj->AddComponent<Collider2D>();
			pObj->AddComponent<ImmovableScript>();
		}

		{
			GameObject* pObj = object::Instantiate<GameObject>(Vector3(20.f, 70.f, 500.f), Vector3(150.f, 300.f, 100.f), eLayerType::Immovable);
			pObj->SetName(L"RootOfTree2");
			pObj->AddComponent<Collider2D>();
			pObj->AddComponent<ImmovableScript>();
		}

		{
			GameObject* pObj = object::Instantiate<GameObject>(Vector3(150.f, 50.f, 500.f), Vector3(100.f, 150.f, 100.f), eLayerType::Immovable);
			pObj->SetName(L"RootOfTree3");
			pObj->AddComponent<Collider2D>();
			pObj->AddComponent<ImmovableScript>();
		}

		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 650.f, 500.f), Vector3(100.f, 100.f, 100.f), eLayerType::Portal);
			pObject->SetName(L"Portal");

			pObject->AddComponent<Collider2D>();

			PortalScript* pPortal = pObject->AddComponent<PortalScript>();
			pPortal->SetCurSceneName(L"BossWayScene");
			pPortal->SetNextSceneName(L"BossScene");
			pPortal->Initialize();
		}

		// 카메라 설정
		GameObject* pPlayer = SceneManager::FindPlayer();
		CameraScript* pMainCamScript = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
		pMainCamScript->SetWorldResolution(Vector2(2016.f, 1920.f));
		pMainCamScript->SetTarget(pPlayer);
	}

	void BossWayScene::Update()
	{
		StageScene::Update();

		// 임시
		if (Input::GetKeyUp(eKeyCode::ENTER))
		{
			SceneManager::LoadScene(L"BossScene");
		}
	}

	void BossWayScene::LateUpdate()
	{
		StageScene::LateUpdate();
	}

	void BossWayScene::Render()
	{
		StageScene::Render();
	}

	void BossWayScene::Destroy()
	{
		StageScene::Destroy();
	}

	void BossWayScene::OnEnter()
	{
		StageScene::OnEnter();

		AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetBGM();
		pBGM->SetClip(Resources::Find<AudioClip>(L"BossWayBGM"));
		pBGM->Play();
		pBGM->SetLoop(true);
		pBGM->SetVolume(0.3f);

		SceneManager::DontUseOnLoad(eLayerType::UI);
	}

	void BossWayScene::OnExit()
	{
		Scene::OnExit();

		AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetBGM();
		pBGM->Stop();
	}
}