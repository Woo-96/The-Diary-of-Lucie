#include "Lu_Nomal1Scene.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"

#include "Lu_Input.h"

namespace Lu
{
	Nomal1Scene::Nomal1Scene()
	{
		SetName(L"Nomal1SceneScript");
	}

	Nomal1Scene::~Nomal1Scene()
	{
	}

	void Nomal1Scene::Initialize()
	{
		StageScene::Initialize();

		// Back
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 900.f), eLayerType::BackGround);
			pObject->SetName(L"Nomal1_BG");

			Transform* pTransform = pObject->GetComponent<Transform>();
			// 원본 2배
			pTransform->SetScale(Vector3(2016.f, 1536.f, 100.f));

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Nomal1BG_Mtrl"));
		}
	}

	void Nomal1Scene::Update()
	{
		StageScene::Update();

		// 임시
		if (Input::GetKeyUp(eKeyCode::SPACE))
		{
			SceneManager::LoadScene(L"Nomal2Scene");
		}
	}

	void Nomal1Scene::LateUpdate()
	{
		StageScene::LateUpdate();
	}

	void Nomal1Scene::Render()
	{
		StageScene::Render();
	}

	void Nomal1Scene::Destroy()
	{
		StageScene::Destroy();
	}

	void Nomal1Scene::OnEnter()
	{
		StageScene::OnEnter();
	}

	void Nomal1Scene::OnExit()
	{

	}
}