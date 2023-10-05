#include "Lu_TitleScene.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_Object.h"
#include "Lu_Input.h"
#include "Lu_AudioListener.h"
#include "Lu_AudioSource.h"
#include "Lu_MouseScript.h"
#include "Lu_SoundManager.h"
#include "Lu_CollisionManager.h"
#include "Lu_LabelScript.h"

namespace Lu
{
	TitleScene::TitleScene()
	{
		SetName(L"TitleSceneScript");
	}

	TitleScene::~TitleScene()
	{

	}

	void TitleScene::Initialize()
	{
		Scene::Initialize();

		// TitleBG : ũ�� ���� 1.5��
		{
			GameObject* pTitleBG = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 900.f), Vector3(1440.f, 810.f, 100.f), eLayerType::BackGround);
			pTitleBG->SetName(L"Title_BG");

			MeshRenderer* pMeshRender = pTitleBG->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"TitleBG_Mtrl"));
		}

		// ���� �Ŵ���
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 0.f), Vector3(0.f, 0.f, 0.f), eLayerType::Default);
			pObject->SetName(L"SoundManager");
			SceneManager::DontDestroyOnLoad(pObject);

			SoundManager* pSoundMgr = pObject->AddComponent<SoundManager>();
			pSoundMgr->SetListener(pObject->AddComponent<AudioListener>());
			pSoundMgr->SetBGM(pObject->AddComponent<AudioSource>());
			pSoundMgr->SetSFX(pObject->AddComponent<AudioSource>());
		}

		// Mouse
		{
			GameObject* pMouse = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 10.f), Vector3(48.f, 48.f, 100.f), eLayerType::Mouse);
			pMouse->SetName(L"Mouse");
			SceneManager::DontDestroyOnLoad(pMouse);

			MeshRenderer* pMeshRender = pMouse->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Mouse_Mtrl"));

			pMouse->AddComponent<MouseScript>();
		}

		// ���
		{
			GameObject* pLucie = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 700.f), Vector3(1440.f, 810.f, 100.f), eLayerType::UI);
			pLucie->SetName(L"Lucie");

			MeshRenderer* pMeshRender = pLucie->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Lucie_Mtrl"));
		}

		// ����
		{
			GameObject* pMary = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 800.f), Vector3(1440.f, 810.f, 100.f), eLayerType::UI);
			pMary->SetName(L"Mary");

			MeshRenderer* pMeshRender = pMary->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Mary_Mtrl"));
		}

		// ���̿���
		{
			GameObject* pIone = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 800.f), Vector3(1440.f, 810.f, 100.f), eLayerType::UI);
			pIone->SetName(L"Ione");

			MeshRenderer* pMeshRender = pIone->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Ione_Mtrl"));
		}

		// Title ����
		{
			GameObject* pTitle = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 800.f), Vector3(1440.f, 810.f, 100.f), eLayerType::UI);
			pTitle->SetName(L"Title");

			MeshRenderer* pMeshRender = pTitle->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Title_Mtrl"));
		}

		// Press any key
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, -150.f, 100.f), Vector3(960.f, 540.f, 100.f), eLayerType::UI);
			pObject->SetName(L"PressKey");

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"PressKey_Mtrl"));

			LabelScript* pLabel = pObject->AddComponent<LabelScript>();
			pLabel->SetMeshRender(pMeshRender);
			pLabel->SetBlink(true);
		}

		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Item, true);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Portal, true);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Ground, true);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Immovable, true);
		CollisionManager::SetLayer(eLayerType::Player, eLayerType::MonsterProjectile, true);
		CollisionManager::SetLayer(eLayerType::Monster, eLayerType::Immovable, true);
		CollisionManager::SetLayer(eLayerType::PlayerProjectile, eLayerType::Statue, true);
		CollisionManager::SetLayer(eLayerType::PlayerProjectile, eLayerType::Monster, true);
		CollisionManager::SetLayer(eLayerType::PlayerProjectile, eLayerType::Immovable, true);
	}

	void TitleScene::Update()
	{
		Scene::Update();

		// �ƹ� Ű�� ������ ���� ������ �̵�
		for (int i = 1; i <= 255; i++)
		{
			if (GetAsyncKeyState(i) & 0x8001)
			{
				SceneManager::LoadScene(L"LobbyScene");
				break;  // �ƹ� Ű�� �������Ƿ� ���� ����
			}
		}
	}

	void TitleScene::LateUpdate()
	{
		Scene::LateUpdate();
	}

	void TitleScene::Render()
	{
		Scene::Render();
	}

	void TitleScene::Destroy()
	{
		Scene::Destroy();
	}

	void TitleScene::OnEnter()
	{
		Scene::OnEnter();

		AudioSource * pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetBGM();
		pBGM->SetClip(Resources::Find<AudioClip>(L"ForestBGM"));
		pBGM->Play();
		pBGM->SetLoop(true);
		pBGM->SetVolume(0.3f);

		SceneManager::DontUseOnLoad(eLayerType::Player);
		SceneManager::DontUseOnLoad(eLayerType::UI);
	}

	void TitleScene::OnExit()
	{
		Scene::OnExit();

		AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetBGM();
		pBGM->Stop();
	}
}