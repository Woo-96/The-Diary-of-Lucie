#include "Lu_BossWayScene.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_SoundManager.h"
#include "Lu_AudioSource.h"

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