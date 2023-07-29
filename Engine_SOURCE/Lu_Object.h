#pragma once
#include "Lu_Layer.h"
#include "Lu_GameObject.h"
#include "Lu_Scene.h"
#include "Lu_SceneManager.h"
#include "Lu_Transform.h"


namespace Lu::object
{
	template <typename T>
	static __forceinline T* Instantiate(enums::eLayerType _Layer)
	{
		T* gameObj = new T();
		Scene* scene = SceneManager::GetActiveScene();
		scene->AddGameObject(_Layer, gameObj);
		gameObj->SetLayerIndex((int)_Layer);
		gameObj->Initialize();

		return gameObj;
	}

	template <typename T>
	static __forceinline T* Instantiate(Vector3 _Pos, enums::eLayerType _Layer)
	{
		T* gameObj = new T();
		Transform* tr = gameObj->GetComponent<Transform>();
		tr->SetPosition(_Pos);

		Scene* scene = SceneManager::GetActiveScene();
		scene->AddGameObject(_Layer, gameObj);
		gameObj->SetLayerIndex((int)_Layer);
		gameObj->Initialize();

		return gameObj;
	}

	template <typename T>
	static __forceinline T* Instantiate(Vector3 _Pos, Vector3 _Scale, enums::eLayerType _Layer)
	{
		T* gameObj = new T();
		Transform* tr = gameObj->GetComponent<Transform>();
		tr->SetPosition(_Pos);
		tr->SetScale(_Scale);

		Scene* scene = SceneManager::GetActiveScene();
		scene->AddGameObject(_Layer, gameObj);
		gameObj->SetLayerIndex((int)_Layer);
		gameObj->Initialize();

		return gameObj;
	}

	template <typename T>
	static __forceinline T* Instantiate(Vector3 _Pos, Vector3 _Scale, Vector3 _Rot, enums::eLayerType _Layer)
	{
		T* gameObj = new T();
		Transform* tr = gameObj->GetComponent<Transform>();
		tr->SetPosition(_Pos);
		tr->SetScale(_Scale);
		tr->SetRotation(_Rot);

		Scene* scene = SceneManager::GetActiveScene();
		scene->AddGameObject(_Layer, gameObj);
		gameObj->SetLayerIndex((int)_Layer);
		gameObj->Initialize();

		return gameObj;
	}

	static __forceinline void Destroy(GameObject* _GameObj)
	{
		_GameObj->SetState(Lu::GameObject::eState::Dead);
	}
}