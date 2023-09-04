#include "Lu_WeaponChoiceScene.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_PlayerScript.h"
#include "Lu_AudioSource.h"
#include "Lu_SoundManager.h"

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
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 500.f), Vector3(384.f, 384.f, 100.f), eLayerType::BackGround);
			pObject->SetName(L"MagicCircle");

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"MagicCircleAnimation_Mtrl"));

			Animator* pAnimator = pObject->AddComponent<Animator>();
			std::shared_ptr<Texture> pAtlas
				= Resources::Load<Texture>(L"MagicCircle_Tex", L"..\\Resources\\Texture\\Map\\Stage\\magic_circle.png");

			pAnimator->Create(L"MagicCircle", pAtlas, Vector2(0.f, 0.f), Vector2(192.f, 192.f), 2, Vector2(192.f, 192.f));
			pAnimator->PlayAnimation(L"MagicCircle", false);
	
			GameObject* pObject2 = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 500.f), Vector3(434.f, 434.f, 100.f), eLayerType::BackGround);
			pObject2->SetName(L"MagicCircle2");

			MeshRenderer* pMeshRender2 = pObject2->AddComponent<MeshRenderer>();
			pMeshRender2->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender2->SetMaterial(Resources::Find<Material>(L"MagicCircle2Animation_Mtrl"));

			Animator* pAnimator2 = pObject2->AddComponent<Animator>();
			pAnimator2->Create(L"MagicCircle", pAtlas, Vector2(0.f, 0.f), Vector2(192.f, 192.f), 2, Vector2(192.f, 192.f), Vector2::Zero, 0.4f);
			pAnimator2->PlayAnimation(L"MagicCircle", true);
		}
	}

	void WeaponChoiceScene::Update()
	{
		StageScene::Update();

		// 임시
		if (Input::GetKeyUp(eKeyCode::ENTER))
		{
			SceneManager::LoadScene(L"Nomal1Scene");
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

		AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetBGM();
		pBGM->SetClip(Resources::Find<AudioClip>(L"ForestBGM"));
		pBGM->Play();
		pBGM->SetVolume(0.3f);

		GameObject* pPlayer = SceneManager::FindPlayer();
		pPlayer->GetComponent<Transform>()->SetPosition(Vector3(0.f, 0.f, 500.f));
		pPlayer->GetComponent<PlayerScript>()->SetDir(eDir::Down);
	}

	void WeaponChoiceScene::OnExit()
	{
		StageScene::OnExit();
	}
}