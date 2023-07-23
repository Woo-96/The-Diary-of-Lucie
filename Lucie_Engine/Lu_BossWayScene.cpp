#include "Lu_BossWayScene.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"

#include "Lu_Input.h"

namespace Lu
{
	BossWayScene::BossWayScene()
	{
		SetName(L"BossWaySceneScript");
	}

	BossWayScene::~BossWayScene()
	{
	}

	void BossWayScene::Initialize()
	{
		Scene::Initialize();

		// Back
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 900.f), eLayerType::BackGround);
			pObject->SetName(L"BossWay_BG");

			Transform* pTransform = pObject->GetComponent<Transform>();
			// 원본 2배
			pTransform->SetScale(Vector3(2016.f, 1920.f, 100.f));

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"BossWayBG_Mtrl"));
		}
	}

	void BossWayScene::Update()
	{
		StageScene::Update();

		// 임시
		if (Input::GetKeyUp(eKeyCode::ENTER))
		{
			SceneManager::LoadScene(L"BossScene");
		}
	}

	void BossWayScene::LateUpdate()
	{
		StageScene::LateUpdate();
	}

	void BossWayScene::Render()
	{
		StageScene::Render();
	}

	void BossWayScene::Destroy()
	{
		StageScene::Destroy();
	}

	void BossWayScene::OnEnter()
	{
		StageScene::OnEnter();
	}

	void BossWayScene::OnExit()
	{

	}
}