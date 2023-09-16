#include "Lu_DiceScene.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_PlayerScript.h"
#include "Lu_CameraScript.h"
#include "Lu_Renderer.h"
#include "Lu_PortalScript.h"
#include "Lu_StatueScript.h"

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

		// 카메라 월드 제한 설정
		{
			GameObject* pPlayer = SceneManager::FindPlayer();
			PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();

			CameraScript* pMainCamScript = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
			pMainCamScript->SetWorldResolution(Vector2(2016.f - 200.f, 1344.f));
			pMainCamScript->SetTarget(pPlayer);
		}

		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(880.f, -270.f, 500.f), Vector3(20.f, 200.f, 100.f), eLayerType::Portal);
			pObject->SetName(L"Portal1");

			pObject->AddComponent<Collider2D>();

			PortalScript* pPortal = pObject->AddComponent<PortalScript>();
			pPortal->SetCurSceneName(L"DiceScene");
			pPortal->SetNextSceneName(L"Nomal1Scene");
			pPortal->Initialize();
		}

		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 650.f, 500.f), Vector3(250.f, 20.f, 100.f), eLayerType::Portal);
			pObject->SetName(L"Portal2");

			pObject->AddComponent<Collider2D>();

			PortalScript* pPortal = pObject->AddComponent<PortalScript>();
			pPortal->SetCurSceneName(L"DiceScene");
			pPortal->SetNextSceneName(L"Nomal2Scene");
			pPortal->Initialize();
		}

		// 석상
		{
			// Red Statue
			GameObject* pRed = object::Instantiate<GameObject>(Vector3(-120.f, 0.f, 500.f), Vector3(240.f, 240.f, 100.f), eLayerType::Statue);
			pRed->SetName(L"RedStatue");
			MeshRenderer* pRedMR = pRed->AddComponent<MeshRenderer>();
			pRedMR->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pRedMR->SetMaterial(Resources::Find<Material>(L"StatueAnimation_Mtrl"));
			Animator* pRedAnimator = pRed->AddComponent<Animator>();
			Collider2D* pRedCollider = pRed->AddComponent<Collider2D>();
			pRedCollider->SetSize(Vector2(0.9f, 0.8f));
			pRedCollider->SetCenter(Vector2(0.f, -20.f));
			StatueScript* pRedStatue = pRed->AddComponent<StatueScript>();
			pRedStatue->SetStatueColor(StatueScript::eStatueColor::Red);

			// Blue Statue
			GameObject* pBlue = object::Instantiate<GameObject>(Vector3(120.f, 0.f, 500.f), Vector3(240.f, 240.f, 100.f), eLayerType::Statue);
			pBlue->SetName(L"BlueStatue");
			MeshRenderer* pBlueMR = pBlue->AddComponent<MeshRenderer>();
			pBlueMR->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pBlueMR->SetMaterial(Resources::Find<Material>(L"StatueAnimation_Mtrl"));
			Animator* pBlueAnimator = pBlue->AddComponent<Animator>();
			Collider2D* pBlueCollider = pBlue->AddComponent<Collider2D>();
			pBlueCollider->SetSize(Vector2(0.9f, 0.8f));
			pBlueCollider->SetCenter(Vector2(0.f, -20.f));
			StatueScript* pBlueStatue = pBlue->AddComponent<StatueScript>();
			pBlueStatue->SetStatueColor(StatueScript::eStatueColor::Blue);

			// 서로 알도록
			pRedStatue->SetAnotherStatue(pBlueStatue);
			pBlueStatue->SetAnotherStatue(pRedStatue);

			// Angel 일지 Devil 일지 랜덤
			int iRand = std::rand() % 2;
			if (iRand == 0)
				pRedStatue->SetStatueType(StatueScript::eStatueType::Angel);
			else
				pRedStatue->SetStatueType(StatueScript::eStatueType::Devil);

			iRand = std::rand() % 2;
			if(iRand == 0)
				pBlueStatue->SetStatueType(StatueScript::eStatueType::Angel);
			else
				pBlueStatue->SetStatueType(StatueScript::eStatueType::Devil);

			// 애니메이션 생성
			pRedStatue->CreateStatueAnimation(pRedAnimator);
			pBlueStatue->CreateStatueAnimation(pBlueAnimator);
		}
	}

	void DiceScene::Update()
	{
		StageScene::Update();
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