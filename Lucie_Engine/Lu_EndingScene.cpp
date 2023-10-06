#include "Lu_EndingScene.h"
#include "Lu_Resources.h"
#include "Lu_SoundManager.h"
#include "Lu_AudioSource.h"
#include "Lu_SceneManager.h"
#include "Lu_Input.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"

namespace Lu
{
	EndingScene::EndingScene()
	{
		SetName(L"TitleSceneScript");
	}

	EndingScene::~EndingScene()
	{
	}

	void EndingScene::Initialize()
	{
		Scene::Initialize();

		// TitleBG : 크기 원본 1.5배
		{
			GameObject* pTitleBG = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 900.f), Vector3(1440.f, 810.f, 100.f), eLayerType::BackGround);
			pTitleBG->SetName(L"Ending_BG");

			MeshRenderer* pMeshRender = pTitleBG->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Ending_Mtrl"));
		}
	}

	void EndingScene::Update()
	{
		Scene::Update();

		if (Input::GetKeyUp(eKeyCode::ENTER))
		{
			SceneManager::LoadScene(L"TitleScene");
		}
	}

	void EndingScene::LateUpdate()
	{
		Scene::LateUpdate();
	}

	void EndingScene::Render()
	{
		Scene::Render();
	}

	void EndingScene::Destroy()
	{
		Scene::Destroy();
	}

	void EndingScene::OnEnter()
	{
		AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetBGM();
		pBGM->Stop();

		pBGM->SetClip(Resources::Find<AudioClip>(L"GameOverBGM"));
		pBGM->Play();
		pBGM->SetLoop(true);
		pBGM->SetVolume(0.3f);

		SceneManager::DontUseOnLoad(eLayerType::Player);
		SceneManager::DontUseOnLoad(eLayerType::UI);
	}

	void EndingScene::OnExit()
	{
	}
}