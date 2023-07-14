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

	}

	TitleScene::~TitleScene()
	{

	}

	void TitleScene::Initialize()
	{
		Scene::Initialize();

		// TitleBG
		{
			GameObject* pTitleBG = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 900.f), eLayerType::Default);
			pTitleBG->SetName(L"TitleBG");

			Transform* pTransform = pTitleBG->GetComponent<Transform>();
			pTransform->SetScale(Vector3(1440.f, 810.f, 100.f));

			MeshRenderer* pMeshRender = pTitleBG->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"TitleBGMtrl"));
		}

		// 루시
		{
			GameObject* pLucie = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 700.f), eLayerType::Default);
			pLucie->SetName(L"Lucie");

			Transform* pTransform = pLucie->GetComponent<Transform>();
			pTransform->SetScale(Vector3(1440.f, 810.f, 100.f));

			MeshRenderer* pMeshRender = pLucie->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"LucieMtrl"));
		}

		// 마리
		{
			GameObject* pMary = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 800.f), eLayerType::Default);
			pMary->SetName(L"Mary");

			Transform* pTransform = pMary->GetComponent<Transform>();
			pTransform->SetScale(Vector3(1440.f, 810.f, 100.f));

			MeshRenderer* pMeshRender = pMary->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"MaryMtrl"));
		}

		// 아이오네
		{
			GameObject* pIone = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 800.f), eLayerType::Default);
			pIone->SetName(L"Ione");

			Transform* pTransform = pIone->GetComponent<Transform>();
			pTransform->SetScale(Vector3(1440.f, 810.f, 100.f));

			MeshRenderer* pMeshRender = pIone->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"IoneMtrl"));
		}

		// Title 문구
		{
			GameObject* pTitle = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 800.f), eLayerType::Default);
			pTitle->SetName(L"Title");

			Transform* pTransform = pTitle->GetComponent<Transform>();
			pTransform->SetScale(Vector3(1440.f, 810.f, 100.f));

			MeshRenderer* pMeshRender = pTitle->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"TitleMtrl"));
		}
	}

	void TitleScene::Update()
	{
		Scene::Update();

		// 새 게임 : 튜토리얼
		if (Input::GetKeyUp(eKeyCode::SPACE))
		{
			SceneManager::LoadScene(L"TutorialScene");
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