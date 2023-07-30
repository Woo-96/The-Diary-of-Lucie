#include "Lu_PlayScene.h"
#include "Lu_Transform.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_Mesh.h"
#include "Lu_CameraScript.h"
#include "Lu_Camera.h"
#include "Lu_SceneManager.h"
#include "Lu_Object.h"
#include "Lu_Renderer.h"
#include "Lu_Collider2D.h"
#include "Lu_CollisionManager.h"
#include "Lu_Input.h"
#include "Lu_Animator.h"
#include "Lu_PlayerScript.h"

namespace Lu
{
	PlayScene::PlayScene()
	{
	}
	PlayScene::~PlayScene()
	{
	}
	void PlayScene::Initialize()
	{
		{
			GameObject* player
				= object::Instantiate<GameObject>(Vector3(0.0f, 0.0f, 1.0001f), eLayerType::Player);

			player->SetName(L"Zelda");

			Collider2D* cd = player->AddComponent<Collider2D>();
			cd->SetSize(Vector2(1.2f, 1.2f));

			MeshRenderer* mr = player->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"SpriteAnimaionMaterial"));

			const float pi = 3.141592f;
			float degree = pi / 8.0f;

			player->GetComponent<Transform>()->SetPosition(Vector3(-2.0f, 0.0f, 1.0001f));
			player->GetComponent<Transform>()->SetScale(Vector3(100.f, 100.f, 100.f));

			std::shared_ptr<Texture> atlas
				= Resources::Load<Texture>(L"LinkSprite", L"..\\Resources\\Texture\\linkSprites.png");

			Animator* at = player->AddComponent<Animator>();
			at->Create(L"Idle", atlas, Vector2(0.0f, 0.0f), Vector2(120.0f, 130.0f), 3, Vector2(200.f, 200.f), Vector2(50.f, 50.f));

			//at->CompleteEvent(L"Idle") = std::bind();

			at->PlayAnimation(L"Idle", true);
			player->AddComponent<PlayerScript>();
		}

		//Main Camera
		Camera* cameraComp = nullptr;
		{
			GameObject* camera = new GameObject();
			AddGameObject(eLayerType::Default, camera);
			camera->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, -10.0f));
			cameraComp = camera->AddComponent<Camera>();
			cameraComp->TurnLayerMask(eLayerType::UI, false);
			camera->AddComponent<CameraScript>();
			renderer::cameras.push_back(cameraComp);
			renderer::mainCamera = cameraComp;
		}

		CollisionManager::SetLayer(eLayerType::Player, eLayerType::Monster, true);
	}

	void PlayScene::Update()
	{
		Scene::Update();

		if (Input::GetKeyUp(eKeyCode::ENTER))
		{
			SceneManager::LoadScene(L"TitleScene");
		}
	}

	void PlayScene::LateUpdate()
	{
		Scene::LateUpdate();
	}

	void PlayScene::Render()
	{
		Scene::Render();
	}

	void PlayScene::Destroy()
	{
		Scene::Destroy();
	}

	void PlayScene::OnEnter()
	{
	}

	void PlayScene::OnExit()
	{
	}
}