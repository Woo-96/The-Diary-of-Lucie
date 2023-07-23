#include "Lu_NextFloorScene.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"

#include "Lu_Input.h"

namespace Lu
{
	NextFloorScene::NextFloorScene()
	{
		SetName(L"NextFloorSceneScript");
	}

	NextFloorScene::~NextFloorScene()
	{
	}

	void NextFloorScene::Initialize()
	{
		StageScene::Initialize();

		// Back
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 900.f), eLayerType::BackGround);
			pObject->SetName(L"NextFloor_BG");

			Transform* pTransform = pObject->GetComponent<Transform>();
			// 원본 2배
			pTransform->SetScale(Vector3(2016.f, 1344.f, 100.f));

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			// 같은 이미지라서 돌려씀
			pMeshRender->SetMaterial(Resources::Find<Material>(L"WeaponChoiceBG_Mtrl"));
		}
	}

	void NextFloorScene::Update()
	{
		StageScene::Update();

		// 임시
		if (Input::GetKeyUp(eKeyCode::ENTER))
		{
			SceneManager::LoadScene(L"BossWayScene");
		}
	}

	void NextFloorScene::LateUpdate()
	{
		StageScene::LateUpdate();
	}

	void NextFloorScene::Render()
	{
		StageScene::Render();
	}

	void NextFloorScene::Destroy()
	{
		StageScene::Destroy();
	}

	void NextFloorScene::OnEnter()
	{
		StageScene::OnEnter();
	}

	void NextFloorScene::OnExit()
	{

	}
}