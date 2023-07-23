#include "Lu_MidBossScene.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"

#include "Lu_Input.h"

namespace Lu
{
	MidBossScene::MidBossScene()
	{
		SetName(L"MidBossSceneScript");
	}

	MidBossScene::~MidBossScene()
	{
	}

	void MidBossScene::Initialize()
	{
		StageScene::Initialize();

		// Back
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 900.f), eLayerType::BackGround);
			pObject->SetName(L"MidBoss_BG");

			Transform* pTransform = pObject->GetComponent<Transform>();
			// 원본 2배
			pTransform->SetScale(Vector3(2208.f, 1920.f, 100.f));

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"MidBossBG_Mtrl"));
		}

		// UI
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 350.f, 100.f), eLayerType::UI);
			pObject->SetName(L"MidBoss_HP_Frame");

			Transform* pTransform = pObject->GetComponent<Transform>();
			// 원본 1.5배
			pTransform->SetScale(Vector3(759.f, 48.f, 100.f));

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"MidBossHPFrame_Mtrl"));
		}

		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 350.f, 100.f), eLayerType::UI);
			pObject->SetName(L"MidBoss_HP");

			Transform* pTransform = pObject->GetComponent<Transform>();
			pTransform->SetScale(Vector3(720.f, 30.f, 100.f));

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"BossHP_Mtrl"));
		}

		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 0.f), eLayerType::UI);
			pObject->SetName(L"MidBoss_Name");

			Transform* pTransform = pObject->GetComponent<Transform>();
			pTransform->SetScale(Vector3(1440.f, 810.f, 100.f));

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"MidBossName_Mtrl"));
		}
	}

	void MidBossScene::Update()
	{
		StageScene::Update();

		// 임시
		if (Input::GetKeyUp(eKeyCode::ENTER))
		{
			SceneManager::LoadScene(L"NextFloorScene");
		}
	}

	void MidBossScene::LateUpdate()
	{
		StageScene::LateUpdate();
	}

	void MidBossScene::Render()
	{
		StageScene::Render();
	}

	void MidBossScene::Destroy()
	{
		StageScene::Destroy();
	}

	void MidBossScene::OnEnter()
	{
		StageScene::OnEnter();
	}

	void MidBossScene::OnExit()
	{

	}
}