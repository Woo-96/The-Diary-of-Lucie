#include "Lu_Nomal2Scene.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"

#include "Lu_Input.h"

namespace Lu
{
	Nomal2Scene::Nomal2Scene()
	{
		SetName(L"Nomal2SceneScript");
	}

	Nomal2Scene::~Nomal2Scene()
	{
	}

	void Nomal2Scene::Initialize()
	{
		StageScene::Initialize();

		// Back : 크기 원본 2배
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 900.f), Vector3(2016.f, 1344.f, 100.f), eLayerType::BackGround);
			pObject->SetName(L"Nomal2_BG");

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Nomal2BG_Mtrl"));
		}
	}

	void Nomal2Scene::Update()
	{
		StageScene::Update();

		// 임시
		if (Input::GetKeyUp(eKeyCode::ENTER))
		{
			SceneManager::LoadScene(L"MidBossScene");
		}
	}

	void Nomal2Scene::LateUpdate()
	{
		StageScene::LateUpdate();
	}

	void Nomal2Scene::Render()
	{
		StageScene::Render();
	}

	void Nomal2Scene::Destroy()
	{
		StageScene::Destroy();
	}

	void Nomal2Scene::OnEnter()
	{
		StageScene::OnEnter();
	}

	void Nomal2Scene::OnExit()
	{

	}
}