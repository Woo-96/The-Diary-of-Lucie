#include "Lu_StoreScene.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"

#include "Lu_Input.h"

namespace Lu
{
	StoreScene::StoreScene()
	{
		SetName(L"StoreSceneScript");
	}

	StoreScene::~StoreScene()
	{
	}

	void StoreScene::Initialize()
	{
		Scene::Initialize();

		// Back : 크기 원본 2배
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 900.f), Vector3(1920.f, 1152.f, 100.f), eLayerType::BackGround);
			pObject->SetName(L"Store_BG");

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"StoreBG_Mtrl"));
		}
	}

	void StoreScene::Update()
	{
		StageScene::Update();

		// 임시
		if (Input::GetKeyUp(eKeyCode::ENTER))
		{
			SceneManager::LoadScene(L"Nomal1Scene");
		}
	}

	void StoreScene::LateUpdate()
	{
		StageScene::LateUpdate();
	}

	void StoreScene::Render()
	{
		StageScene::Render();
	}

	void StoreScene::Destroy()
	{
		StageScene::Destroy();
	}

	void StoreScene::OnEnter()
	{
		StageScene::OnEnter();
	}

	void StoreScene::OnExit()
	{
		StageScene::OnExit();
	}
}