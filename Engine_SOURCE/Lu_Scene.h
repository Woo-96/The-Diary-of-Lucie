#pragma once
#include "Lu_Entity.h"
#include "Lu_Layer.h"

namespace Lu
{
	class Camera;
	class Scene : public Entity
	{
	public:
		Scene();
		virtual ~Scene();

	private:
		std::vector<Layer>	m_Layers;
		Camera*				m_MainCam;
		Camera*				m_UICam;

	public:
		Layer& GetLayer(eLayerType type)
		{
			return m_Layers[(UINT)type];
		}

		Camera* GetUICam()	const
		{
			return m_UICam;
		}

	public:
		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();
		virtual void Destroy();

	public:
		void AddGameObject(eLayerType _Type, GameObject* _GameObj);
		void RemoveGameObject(eLayerType _Type, GameObject* _GameObj);
		GameObject* FindGameObject(eLayerType _Type, GameObject* _GameObj);
		GameObject* FindGameObject(eLayerType _Type, std::wstring _Name);

	public:
		virtual void OnEnter();
		virtual void OnExit();

	public:
		template <typename T>
		std::vector<T*> FindObjectsOfType()
		{
			std::vector<T*> findObjs = {};
			for (Layer* layer : m_Layers)
			{
				auto gameObjs = layer->GetGameObjects();
				for (GameObject* obj : gameObjs)
				{
					T* buff = dynamic_cast<T*>(obj);
					if (buff != nullptr)
						findObjs.push_back(buff);
				}
			}

			return findObjs;
		}
	};
}