#include "Lu_TitleScene.h"
#include "Lu_Renderer.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_Object.h"
#include "Lu_Input.h"
#include "Lu_ParticleSystem.h"

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

		// TitleBG : ũ�� ���� 1.5��
		{
			GameObject* pTitleBG = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 900.f), Vector3(1440.f, 810.f, 100.f), eLayerType::BackGround);
			pTitleBG->SetName(L"Title_BG");

			MeshRenderer* pMeshRender = pTitleBG->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"TitleBG_Mtrl"));
		}

		// ���
		{
			GameObject* pLucie = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 700.f), Vector3(1440.f, 810.f, 100.f), eLayerType::UI);
			pLucie->SetName(L"Lucie");

			MeshRenderer* pMeshRender = pLucie->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Lucie_Mtrl"));
		}

		// ����
		{
			GameObject* pMary = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 800.f), Vector3(1440.f, 810.f, 100.f), eLayerType::UI);
			pMary->SetName(L"Mary");

			MeshRenderer* pMeshRender = pMary->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Mary_Mtrl"));
		}

		// ���̿���
		{
			GameObject* pIone = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 800.f), Vector3(1440.f, 810.f, 100.f), eLayerType::UI);
			pIone->SetName(L"Ione");

			MeshRenderer* pMeshRender = pIone->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Ione_Mtrl"));
		}

		// Title ����
		{
			GameObject* pTitle = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 800.f), Vector3(1440.f, 810.f, 100.f), eLayerType::UI);
			pTitle->SetName(L"Title");

			MeshRenderer* pMeshRender = pTitle->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Title_Mtrl"));
		}

		// UI
		{
			//GameObject* pParticle = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 900.f), Vector3(500, 500.f, 500.f), eLayerType::Default);
			//pParticle->SetName(L"ParticleTest");

			//ParticleSystem* pParticleSystem = pParticle->AddComponent<ParticleSystem>();
		}
	}

	void TitleScene::Update()
	{
		Scene::Update();

		// �� ���� : 
		if (Input::GetKeyUp(eKeyCode::ENTER))
		{
			SceneManager::LoadScene(L"LobbyScene");
		}

		// ���� : ���� ����
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