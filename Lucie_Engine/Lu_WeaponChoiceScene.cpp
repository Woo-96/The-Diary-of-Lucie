#include "Lu_WeaponChoiceScene.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_PlayerScript.h"
#include "Lu_AudioSource.h"
#include "Lu_SoundManager.h"
#include "Lu_CameraScript.h"
#include "Lu_Renderer.h"
#include "Lu_PortalScript.h"

namespace Lu
{
	WeaponChoiceScene::WeaponChoiceScene()
		: m_arrWeapon{}
	{
		SetName(L"WeaponChoiceSceneScript");
	}

	WeaponChoiceScene::~WeaponChoiceScene()
	{
	}

	void WeaponChoiceScene::Initialize()
	{
		StageScene::Initialize();
		CreateHUD();

		// Back : 크기 원본 2배
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 900.f), Vector3(2016.f, 1344.f, 100.f), eLayerType::BackGround);
			pObject->SetName(L"WeaponChoice_BG");

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"WeaponChoiceBG_Mtrl"));
		}

		// 매직서클
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 500.f), Vector3(288.f, 288.f, 100.f), eLayerType::BackGround);
			pObject->SetName(L"MagicCircle");

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"MagicCircleAnimation_Mtrl"));

			Animator* pAnimator = pObject->AddComponent<Animator>();
			std::shared_ptr<Texture> pAtlas
				= Resources::Load<Texture>(L"MagicCircle_Tex", L"..\\Resources\\Texture\\Map\\Stage\\magic_circle.png");

			pAnimator->Create(L"MagicCircle", pAtlas, Vector2(0.f, 0.f), Vector2(192.f, 192.f), 2, Vector2(192.f, 192.f));
			pAnimator->PlayAnimation(L"MagicCircle", false);
	
			GameObject* pObject2 = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 500.f), Vector3(384.f, 384.f, 100.f), eLayerType::BackGround);
			pObject2->SetName(L"MagicCircle2");

			MeshRenderer* pMeshRender2 = pObject2->AddComponent<MeshRenderer>();
			pMeshRender2->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender2->SetMaterial(Resources::Find<Material>(L"MagicCircle2Animation_Mtrl"));

			Animator* pAnimator2 = pObject2->AddComponent<Animator>();
			pAnimator2->Create(L"MagicCircle", pAtlas, Vector2(0.f, 0.f), Vector2(192.f, 192.f), 2, Vector2(192.f, 192.f), Vector2::Zero, 0.4f);
			pAnimator2->PlayAnimation(L"MagicCircle", true);
		}

		// 포탈
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 550.f, 500.f), Vector3(200.f, 20.f, 100.f), eLayerType::Portal);
			pObject->SetName(L"Portal");

			pObject->AddComponent<Collider2D>();

			PortalScript* pPortal = pObject->AddComponent<PortalScript>();
			pPortal->SetCurSceneName(L"WeaponChoiceScene");
			pPortal->SetNextSceneName(L"Nomal1Scene");
			pPortal->Initialize();
		}

		// Weapon
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(-200.f, 150.f, 500.f), Vector3(48.f, 48.f, 100.f), eLayerType::Item);
			pObject->SetName(L"Wand");

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Wand_Mtrl"));

			pObject->AddComponent<Collider2D>();

			m_arrWeapon[(int)eWeaponType::Wand] = pObject->AddComponent<WeaponScript>();
			m_arrWeapon[(int)eWeaponType::Wand]->SetTransform(pObject->GetComponent<Transform>());
			m_arrWeapon[(int)eWeaponType::Wand]->SetItemState(ItemScript::eItemState::Drop);
			m_arrWeapon[(int)eWeaponType::Wand]->SetWeaponType(eWeaponType::Wand);
		}

		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 150.f, 500.f), Vector3(48.f, 48.f, 100.f), eLayerType::Item);
			pObject->SetName(L"Bow");

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Bow_Mtrl"));

			pObject->AddComponent<Collider2D>();

			m_arrWeapon[(int)eWeaponType::Bow] = pObject->AddComponent<WeaponScript>();
			m_arrWeapon[(int)eWeaponType::Bow]->SetTransform(pObject->GetComponent<Transform>());
			m_arrWeapon[(int)eWeaponType::Bow]->SetItemState(ItemScript::eItemState::Drop);
			m_arrWeapon[(int)eWeaponType::Bow]->SetWeaponType(eWeaponType::Bow);
		}

		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(200.f, 150.f, 500.f), Vector3(48.f, 48.f, 100.f), eLayerType::Item);
			pObject->SetName(L"Sword");

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Sword_Mtrl"));

			pObject->AddComponent<Collider2D>();

			m_arrWeapon[(int)eWeaponType::Sword] = pObject->AddComponent<WeaponScript>();
			m_arrWeapon[(int)eWeaponType::Sword]->SetTransform(pObject->GetComponent<Transform>());
			m_arrWeapon[(int)eWeaponType::Sword]->SetItemState(ItemScript::eItemState::Drop);
			m_arrWeapon[(int)eWeaponType::Sword]->SetWeaponType(eWeaponType::Sword);
		}

		GameObject* pPlayer = SceneManager::FindPlayer();
		CameraScript* pMainCamScript = renderer::mainCamera->GetOwner()->GetComponent<CameraScript>();
		pMainCamScript->SetWorldResolution(Vector2(2016.f - 400.f, 1344.f - 200.f));
		pMainCamScript->SetTarget(pPlayer);
	}

	void WeaponChoiceScene::Update()
	{
		StageScene::Update();
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
		StageScene::OnExit();
	}

	void WeaponChoiceScene::PlayerWeaponGet(eWeaponType _Type)
	{
		for (int i = 0; i < (int)eWeaponType::None; ++i)
		{
			if (i == (int)_Type)
				continue;

			if (m_arrWeapon[i])
			{
				object::Destroy(m_arrWeapon[i]->GetOwner());
			}
		}
	}
}