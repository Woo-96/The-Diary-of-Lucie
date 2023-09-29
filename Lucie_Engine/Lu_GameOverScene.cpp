#include "Lu_GameOverScene.h"
#include "Lu_Renderer.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_Object.h"
#include "Lu_AudioSource.h"
#include "Lu_SoundManager.h"
#include "Lu_PlayerScript.h"

#include "Lu_Input.h"

namespace Lu
{
	GameOverScene::GameOverScene()
	{

	}

	GameOverScene::~GameOverScene()
	{

	}

	void GameOverScene::Initialize()
	{
		Scene::Initialize();

		// Back : 크기 원본 1.5배
		{
			GameObject* pGameOver = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 900.f), Vector3(1440.f, 810.f, 100.f), eLayerType::Default);
			pGameOver->SetName(L"GameOver");

			MeshRenderer* pMeshRender = pGameOver->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"GameOver_Mtrl"));
		}
	}

	void GameOverScene::Update()
	{
		Scene::Update();

		// 다시하기 : WeaponChioceScene
		if (Input::GetKeyUp(eKeyCode::ENTER))
		{
			GameObject* pPlayer = SceneManager::FindPlayer();
			PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();
			pPlayerScript->ResetPlayerInfo();
			pPlayerScript->GetOwner()->GetComponent<Transform>()->SetPosition(Vector3(0, 0.f, 500.f));
			pPlayerScript->MagicPortalMove();

			AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
			pSFX->SetClip(Resources::Load<AudioClip>(L"floorStart", L"..\\Resources\\Sound\\SFX\\floorStart.ogg"));
			pSFX->Play();
			SceneManager::LoadScene(L"WeaponChoiceScene");

			AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetBGM();
			pBGM->Stop();

			pBGM->SetClip(Resources::Find<AudioClip>(L"ForestBGM"));
			pBGM->Play();
			pBGM->SetLoop(true);
			pBGM->SetVolume(0.3f);
		}

		// 방으로 돌아가기
		if (Input::GetKeyUp(eKeyCode::SPACE))
		{
			GameObject* pPlayer = SceneManager::FindPlayer();
			PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();
			pPlayerScript->ResetPlayerInfo();
			pPlayerScript->GetOwner()->GetComponent<Transform>()->SetPosition(Vector3(400.f, 50.f, 500.f));
			
			AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetBGM();
			pBGM->Stop();
			SceneManager::LoadScene(L"LobbyScene");
		}
	}

	void GameOverScene::LateUpdate()
	{
		Scene::LateUpdate();
	}

	void GameOverScene::Render()
	{
		Scene::Render();
	}

	void GameOverScene::Destroy()
	{
		Scene::Destroy();
	}

	void GameOverScene::OnEnter()
	{
		Scene::OnEnter();

		AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetBGM();
		pBGM->Stop();

		pBGM->SetClip(Resources::Find<AudioClip>(L"GameOverBGM"));
		pBGM->Play();
		pBGM->SetVolume(0.3f);

		SceneManager::DontUseOnLoad(eLayerType::Player);
		SceneManager::DontUseOnLoad(eLayerType::UI);
	}

	void GameOverScene::OnExit()
	{
		Scene::OnExit();

		AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetBGM();
		if(nullptr != pBGM->GetClip())
			pBGM->Stop();
	}
}