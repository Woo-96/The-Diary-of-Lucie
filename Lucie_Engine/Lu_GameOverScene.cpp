#include "Lu_GameOverScene.h"
#include "Lu_Renderer.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_Object.h"
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

		{
			GameObject* pGameOver = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 900.f), eLayerType::Default);
			pGameOver->SetName(L"GameOver");

			Transform* pTransform = pGameOver->GetComponent<Transform>();
			// 원본 1.5배
			pTransform->SetScale(Vector3(1440.f, 810.f, 100.f));

			MeshRenderer* pMeshRender = pGameOver->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"GameOver_Mtrl"));
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

	}
}