#include "Lu_Scene.h"
#include "Lu_Object.h"
#include "Lu_Camera.h"
#include "Lu_CameraScript.h"
#include "Lu_Renderer.h"
#include "Lu_GameObject.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_Transform.h"

namespace Lu
{
	Scene::Scene()
		: m_MainCam(nullptr)
		, m_UICam(nullptr)
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
			m_MainCam->TurnLayerMask(eLayerType::Mouse, false);
			renderer::cameras.push_back(m_MainCam);
			renderer::mainCamera = m_MainCam;
		}

		//UI Camera
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 0.f), eLayerType::Default);
			pObject->SetName(L"UICam");

			m_UICam = pObject->AddComponent<Camera>();
			m_UICam->DisableLayerMasks();
			m_UICam->TurnLayerMask(eLayerType::UI, true);
			m_UICam->TurnLayerMask(eLayerType::Mouse, true);
			renderer::cameras.push_back(m_UICam);
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

	void Scene::RemoveGameObject(eLayerType _Type, GameObject* _GameObj)
	{
		m_Layers[(int)_Type].RemoveGameObject(_GameObj);
	}

	GameObject* Scene::FindGameObject(eLayerType _Type, GameObject* _GameObj)
	{
		Layer& layer = m_Layers[(int)_Type];

		for (GameObject* obj : layer.GetGameObjects())
		{
			if (obj == _GameObj)
			{
				return obj;
			}
		}

		return nullptr;
	}

	GameObject* Scene::FindGameObject(eLayerType _Type, std::wstring _Name)
	{
		Layer& layer = m_Layers[(int)_Type];

		for (GameObject* obj : layer.GetGameObjects())
		{
			if (obj->GetName() == _Name)
			{
				return obj;
			}
		}

		return nullptr;
	}

	void Scene::OnEnter()
	{
		renderer::mainCamera = m_MainCam;
	}

	void Scene::OnExit()
	{

	}
}