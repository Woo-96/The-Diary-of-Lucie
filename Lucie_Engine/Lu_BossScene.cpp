#include "Lu_BossScene.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"

#include "Lu_Input.h"

namespace Lu
{
	BossScene::BossScene()
	{
		SetName(L"BossSceneScript");
	}

	BossScene::~BossScene()
	{
	}

	void BossScene::Initialize()
	{
		StageScene::Initialize();

		// Back
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 900.f), eLayerType::BackGround);
			pObject->SetName(L"Boss_BG");

			Transform* pTransform = pObject->GetComponent<Transform>();
			// 원본 2배
			pTransform->SetScale(Vector3(2688.f, 2400.f, 100.f));

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"BossBG_Mtrl"));
		}
	}

	void BossScene::Update()
	{
		StageScene::Update();

		// 임시
		if (Input::GetKeyUp(eKeyCode::SPACE))
		{
			SceneManager::LoadScene(L"GameOverScene");
		}
	}

	void BossScene::LateUpdate()
	{
		StageScene::LateUpdate();
	}

	void BossScene::Render()
	{
		StageScene::Render();
	}

	void BossScene::Destroy()
	{
		StageScene::Destroy();
	}

	void BossScene::OnEnter()
	{
		StageScene::OnEnter();
	}

	void BossScene::OnExit()
	{

	}
}