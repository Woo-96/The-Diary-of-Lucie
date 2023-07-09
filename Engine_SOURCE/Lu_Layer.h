#pragma once
#include "Lu_GameObject.h"

namespace Lu
{
	class Layer
	{
	public:
		Layer();
		~Layer();

	private:
		std::vector<GameObject*> m_GameObjects;

	public:
		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();
		
	public:
		void AddGameObject(GameObject* _GameObj);
	};
}