#include "Lu_Scene.h"
#include "Lu_Object.h"
#include "Lu_Camera.h"
#include "Lu_CameraScript.h"
#include "Lu_Renderer.h"
#include "Lu_GameObject.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "..\Lucie_Engine\Lu_MouseScript.h"
#include "Lu_Input.h"
#include "Lu_Transform.h"

namespace Lu
{
	Scene::Scene()
		: m_MainCam(nullptr)
		, m_Mouse(nullptr)
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

		//UI Camera & Mouse Obj
		{
			GameObject* pObject = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 0.f), eLayerType::Default);
			pObject->SetName(L"UICam");

			Camera* pCam = pObject->AddComponent<Camera>();
			pCam->DisableLayerMasks();
			pCam->TurnLayerMask(eLayerType::UI, true);
			pCam->TurnLayerMask(eLayerType::Mouse, true);
			renderer::cameras.push_back(pCam);

			GameObject* pMouse = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 10.f), Vector3(48.f, 48.f, 100.f), eLayerType::Mouse);
			pMouse->SetName(L"Mouse");

			m_Mouse = pMouse->AddComponent<MouseScript>();
			m_Mouse->SetCam(pCam);

			MeshRenderer* pMeshRender = pMouse->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Mouse_Mtrl"));
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