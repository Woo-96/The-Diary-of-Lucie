#include "Lu_TutorialScene.h"
#include "Lu_Renderer.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_Object.h"
#include "Lu_Input.h"

namespace Lu
{
	TutorialScene::TutorialScene()
	{

	}

	TutorialScene::~TutorialScene()
	{

	}

	void TutorialScene::Initialize()
	{
		Scene::Initialize();

		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 900.f), eLayerType::Default);
			pObject->SetName(L"Tutorial");

			Transform* pTransform = pObject->GetComponent<Transform>();
			pTransform->SetScale(Vector3(2160.f, 1800.f, 100.f));

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Tutorial_Mtrl"));
		}
	}

	void TutorialScene::Update()
	{
		Scene::Update();

		if (Input::GetKeyUp(eKeyCode::SPACE))
		{
			SceneManager::LoadScene(L"LobbyScene");
		}
	}

	void TutorialScene::LateUpdate()
	{
		Scene::LateUpdate();
	}

	void TutorialScene::Render()
	{
		Scene::Render();
	}

	void TutorialScene::Destroy()
	{
		Scene::Destroy();
	}

	void TutorialScene::OnEnter()
	{
		Scene::OnEnter();
	}

	void TutorialScene::OnExit()
	{

	}
}