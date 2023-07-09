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
		std::vector<Layer> m_Layers;

	public:
		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

	public:
		void AddGameObject(eLayerType _Type, GameObject* _GameObj);

	public:
		virtual void OnEnter();
		virtual void OnExit();
	};
}