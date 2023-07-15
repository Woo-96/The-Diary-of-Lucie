#include "Lu_LobbyScene.h"
#include "Lu_Renderer.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_Object.h"
#include "Lu_Input.h"

namespace Lu
{
	LobbyScene::LobbyScene()
	{

	}

	LobbyScene::~LobbyScene()
	{

	}

	void LobbyScene::Initialize()
	{
		Scene::Initialize();

		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 900.f), eLayerType::Default);
			pObject->SetName(L"Room");

			Transform* pTransform = pObject->GetComponent<Transform>();
			pTransform->SetScale(Vector3(1512.f, 1584.f, 100.f));

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Room_Mtrl"));
		}
	}

	void LobbyScene::Update()
	{
		Scene::Update();

		if (Input::GetKeyUp(eKeyCode::SPACE))
		{
			SceneManager::LoadScene(L"StageScene");
		}
	}

	void LobbyScene::LateUpdate()
	{
		Scene::LateUpdate();
	}

	void LobbyScene::Render()
	{
		Scene::Render();
	}

	void LobbyScene::Destroy()
	{
		Scene::Destroy();
	}

	void LobbyScene::OnEnter()
	{
		Scene::OnEnter();
	}

	void LobbyScene::OnExit()
	{

	}
}