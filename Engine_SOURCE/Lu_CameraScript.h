#pragma once
#include "..\\Engine_SOURCE\Lu_Script.h"

namespace Lu
{
	class GameObject;
	class CameraScript : public Script
	{
	public:
		CameraScript();
		virtual ~CameraScript();

	private:
		GameObject*		m_Target;
		Vector2         m_WindowResolution;
		Vector2         m_WorldResolution;
		float			m_Speed;

	public:
		void SetWindowResolution(Vector2 _WindowResolution)
		{
			m_WindowResolution = _WindowResolution;
		}

		void SetWorldResolution(Vector2 _WorldResolution)
		{
			m_WorldResolution = _WorldResolution;
		}

		void SetTarget(GameObject* _Target)
		{
			m_Target = _Target;
		}

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
	};
}