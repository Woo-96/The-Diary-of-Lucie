#include "Lu_TitleScene.h"
#include "Lu_Renderer.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_Object.h"
#include "Lu_Input.h"

namespace Lu
{
	TitleScene::TitleScene()
	{
		SetName(L"TitleSceneScript");
	}

	TitleScene::~TitleScene()
	{

	}

	void TitleScene::Initialize()
	{
		Scene::Initialize();

		// TitleBG
		{
			GameObject* pTitleBG = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 900.f), eLayerType::BackGround);
			pTitleBG->SetName(L"Title_BG");

			Transform* pTransform = pTitleBG->GetComponent<Transform>();
			// 원본 1.5배
			pTransform->SetScale(Vector3(1440.f, 810.f, 100.f));

			MeshRenderer* pMeshRender = pTitleBG->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"TitleBG_Mtrl"));
		}

		// 루시
		{
			GameObject* pLucie = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 700.f), eLayerType::UI);
			pLucie->SetName(L"Lucie");

			Transform* pTransform = pLucie->GetComponent<Transform>();
			pTransform->SetScale(Vector3(1440.f, 810.f, 100.f));

			MeshRenderer* pMeshRender = pLucie->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Lucie_Mtrl"));
		}

		// 마리
		{
			GameObject* pMary = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 800.f), eLayerType::UI);
			pMary->SetName(L"Mary");

			Transform* pTransform = pMary->GetComponent<Transform>();
			pTransform->SetScale(Vector3(1440.f, 810.f, 100.f));

			MeshRenderer* pMeshRender = pMary->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Mary_Mtrl"));
		}

		// 아이오네
		{
			GameObject* pIone = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 800.f), eLayerType::UI);
			pIone->SetName(L"Ione");

			Transform* pTransform = pIone->GetComponent<Transform>();
			pTransform->SetScale(Vector3(1440.f, 810.f, 100.f));

			MeshRenderer* pMeshRender = pIone->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Ione_Mtrl"));
		}

		// Title 문구
		{
			GameObject* pTitle = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 800.f), eLayerType::UI);
			pTitle->SetName(L"Title");

			Transform* pTransform = pTitle->GetComponent<Transform>();
			pTransform->SetScale(Vector3(1440.f, 810.f, 100.f));

			MeshRenderer* pMeshRender = pTitle->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Title_Mtrl"));
		}

		// UI
		{

		}
	}

	void TitleScene::Update()
	{
		Scene::Update();

		// 새 게임 : 
		if (Input::GetKeyUp(eKeyCode::SPACE))
		{
			SceneManager::LoadScene(L"LobbyScene");
		}

		// 종료 : 게임 종료
	}

	void TitleScene::LateUpdate()
	{
		Scene::LateUpdate();
	}

	void TitleScene::Render()
	{
		Scene::Render();
	}

	void TitleScene::Destroy()
	{
		Scene::Destroy();
	}

	void TitleScene::OnEnter()
	{
		Scene::OnEnter();
	}

	void TitleScene::OnExit()
	{

	}
}