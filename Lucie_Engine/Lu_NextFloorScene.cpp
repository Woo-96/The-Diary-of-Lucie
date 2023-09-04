#include "Lu_NextFloorScene.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_AudioSource.h"
#include "Lu_SoundManager.h"

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

		// Back : 크기 원본 2배
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 900.f), Vector3(2016.f, 1344.f, 100.f), eLayerType::BackGround);
			pObject->SetName(L"NextFloor_BG");

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
		StageScene::OnExit();

		AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetBGM();
		pBGM->Stop();
	}
}