#include "Lu_DiceScene.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"

#include "Lu_Input.h"

namespace Lu
{
	DiceScene::DiceScene()
	{
		SetName(L"DiceSceneScript");
	}

	DiceScene::~DiceScene()
	{
	}

	void DiceScene::Initialize()
	{
		StageScene::Initialize();

		// Back : 크기 원본 2배
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 900.f), Vector3(2016.f, 1344.f, 100.f), eLayerType::BackGround);
			pObject->SetName(L"Dice_BG");

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"DiceBG_Mtrl"));
		}
	}

	void DiceScene::Update()
	{
		StageScene::Update();

		// 임시
		if (Input::GetKeyUp(eKeyCode::ENTER))
		{
			SceneManager::LoadScene(L"StoreScene");
		}
	}

	void DiceScene::LateUpdate()
	{
		StageScene::LateUpdate();
	}

	void DiceScene::Render()
	{
		StageScene::Render();
	}

	void DiceScene::Destroy()
	{
		StageScene::Destroy();
	}

	void DiceScene::OnEnter()
	{
		StageScene::OnEnter();
	}

	void DiceScene::OnExit()
	{
		StageScene::OnExit();
	}
}