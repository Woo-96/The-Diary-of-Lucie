#include "Lu_Scene.h"
#include "Lu_Object.h"
#include "Lu_Camera.h"
#include "Lu_CameraScript.h"
#include "Lu_Renderer.h"

namespace Lu
{
	Scene::Scene()
		: m_MainCam(nullptr)
	{
		m_Layers.resize((int)Lu::enums::eLayerType::End);
	}

	Scene::~Scene()
	{

	}

	void Scene::Initialize()
	{
		//Main Camera
		{
			GameObject* pMainCam = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 0.f), eLayerType::Default);
			pMainCam->SetName(L"MainCam");

			m_MainCam = pMainCam->AddComponent<Camera>();
			pMainCam->AddComponent<CameraScript>();

			m_MainCam->EnableLayerMasks();
			m_MainCam->TurnLayerMask(eLayerType::UI, false);
			renderer::cameras.push_back(m_MainCam);
			renderer::mainCamera = m_MainCam;
		}

		//UI Camera
		{
			GameObject* pUICam = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 0.f), eLayerType::Default);
			pUICam->SetName(L"UICam");

			Camera* pCameraCom = pUICam->AddComponent<Camera>();
			pCameraCom->DisableLayerMasks();
			pCameraCom->TurnLayerMask(eLayerType::UI, true);
			renderer::cameras.push_back(pCameraCom);
		}
	}

	void Scene::Update()
	{
		for (Layer& layer : m_Layers)
		{
			layer.Update();
		}
	}

	void Scene::LateUpdate()
	{
		for (Layer& layer : m_Layers)
		{
			layer.LateUpdate();
		}
	}

	void Scene::Render()
	{
		for (Layer& layer : m_Layers)
		{
			layer.Render();
		}
	}

	void Scene::Destroy()
	{
		for (Layer& layer : m_Layers)
		{
			layer.Destory();
		}
	}

	void Scene::AddGameObject(eLayerType _Type, GameObject* _GameObj)
	{
		m_Layers[(int)_Type].AddGameObject(_GameObj);
	}

	void Scene::OnEnter()
	{
		renderer::mainCamera = m_MainCam;
	}

	void Scene::OnExit()
	{

	}
}