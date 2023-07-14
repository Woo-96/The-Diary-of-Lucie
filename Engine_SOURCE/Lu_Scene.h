#pragma once
#include "Lu_Entity.h"
#include "Lu_Layer.h"

namespace Lu
{
	class Scene : public Entity
	{
	public:
		Scene();
		virtual ~Scene();

	private:
		std::vector<Layer>	m_Layers;
		class Camera*		m_MainCam;

	public:
		Layer& GetLayer(eLayerType type)
		{
			return m_Layers[(UINT)type];
		}

	public:
		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();
		virtual void Destroy();

	public:
		void AddGameObject(eLayerType _Type, GameObject* _GameObj);

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