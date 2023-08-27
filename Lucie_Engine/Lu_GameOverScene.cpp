#include "Lu_GameOverScene.h"
#include "Lu_Renderer.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_Object.h"
#include "Lu_AudioSource.h"
#include "Lu_AudioListener.h"

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

			pGameOver->AddComponent<AudioListener>();
			AudioSource* BGM = pGameOver->AddComponent<AudioSource>();
			BGM->SetClip(Resources::Load<AudioClip>(L"GameOverBGM", L"..\\Resources\\Sound\\BGM\\GameOverBGM.ogg"));
			SetBGM(BGM);
		}
	}

	void GameOverScene::Update()
	{
		Scene::Update();

		// 다시하기 : 
		if (Input::GetKeyUp(eKeyCode::ENTER))
		{
			SceneManager::LoadScene(L"PlayScene");
		}

		// 방으로 돌아가기
		//if (Input::GetKeyUp(eKeyCode::SPACE))
		//{
		//	SceneManager::LoadScene(L"LobbyScene");
		//}
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
	}

	void GameOverScene::OnExit()
	{
		Scene::OnExit();
	}
}