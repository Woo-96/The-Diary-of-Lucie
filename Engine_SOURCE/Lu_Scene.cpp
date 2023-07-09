#include "Lu_Scene.h"

namespace Lu
{
	Scene::Scene()
	{
		m_Layers.resize((int)Lu::enums::eLayerType::End);
	}

	Scene::~Scene()
	{

	}

	void Scene::Initialize()
	{
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

	void Scene::AddGameObject(eLayerType _Type, GameObject* _GameObj)
	{
		m_Layers[(int)_Type].AddGameObject(_GameObj);
	}

	void Scene::OnEnter()
	{

	}

	void Scene::OnExit()
	{

	}
}