#include "Lu_WeaponChoiceScene.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"

#include "Lu_Input.h"

namespace Lu
{
	WeaponChoiceScene::WeaponChoiceScene()
	{
		SetName(L"WeaponChoiceSceneScript");
	}

	WeaponChoiceScene::~WeaponChoiceScene()
	{
	}

	void WeaponChoiceScene::Initialize()
	{
		StageScene::Initialize();

		// Back
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 900.f), eLayerType::BackGround);
			pObject->SetName(L"WeaponChoice_BG");

			Transform* pTransform = pObject->GetComponent<Transform>();
			// 원본 2배
			pTransform->SetScale(Vector3(2016.f, 1344.f, 100.f));

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"WeaponChoiceBG_Mtrl"));
		}
	}

	void WeaponChoiceScene::Update()
	{
		StageScene::Update();

		// 임시
		if (Input::GetKeyUp(eKeyCode::SPACE))
		{
			SceneManager::LoadScene(L"DiceScene");
		}
	}

	void WeaponChoiceScene::LateUpdate()
	{
		StageScene::LateUpdate();
	}

	void WeaponChoiceScene::Render()
	{
		StageScene::Render();
	}

	void WeaponChoiceScene::Destroy()
	{
		StageScene::Destroy();
	}

	void WeaponChoiceScene::OnEnter()
	{
		StageScene::OnEnter();
	}

	void WeaponChoiceScene::OnExit()
	{
		
	}
}