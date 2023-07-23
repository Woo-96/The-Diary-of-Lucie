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

		// UI
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 350.f, 100.f), eLayerType::UI);
			pObject->SetName(L"Boss_HP_Frame");

			Transform* pTransform = pObject->GetComponent<Transform>();
			// 원본 1.5배
			pTransform->SetScale(Vector3(759.f, 48.f, 100.f));

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"BossHPFrame_Mtrl"));
		}

		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 350.f, 100.f), eLayerType::UI);
			pObject->SetName(L"Boss_HP");

			Transform* pTransform = pObject->GetComponent<Transform>();
			pTransform->SetScale(Vector3(720.f, 30.f, 100.f));

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"BossHP_Mtrl"));
		}

		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 0.f), eLayerType::UI);
			pObject->SetName(L"Boss_Name");

			Transform* pTransform = pObject->GetComponent<Transform>();
			pTransform->SetScale(Vector3(1440.f, 810.f, 100.f));

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"BossName_Mtrl"));
		}
	}

	void BossScene::Update()
	{
		StageScene::Update();

		// 임시
		if (Input::GetKeyUp(eKeyCode::ENTER))
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