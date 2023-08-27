#pragma once
#include "Lu_Entity.h"
#include "Lu_Layer.h"

namespace Lu
{
	class MouseScript;
	class Camera;
	class AudioSource;
	class Scene : public Entity
	{
	public:
		Scene();
		virtual ~Scene();

	private:
		std::vector<Layer>	m_Layers;
		Camera*				m_MainCam;
		MouseScript*		m_Mouse;
		AudioSource*		m_BGM;
		bool				m_bContinuousPlay;

	public:
		void SetBGM(AudioSource* _BGM)
		{
			m_BGM = _BGM;
		}

		void SetContinuousPlay(bool _bContinue)
		{
			m_bContinuousPlay = _bContinue;
		}

	public:
		Layer& GetLayer(eLayerType type)
		{
			return m_Layers[(UINT)type];
		}

		MouseScript* GetMouse()	const
		{
			return m_Mouse;
		}

		AudioSource* GetBGM()	const
		{
			return m_BGM;
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