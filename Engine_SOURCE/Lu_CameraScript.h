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
		Vector2			m_Offset;

		bool			m_bTargetMove;	// 카메라를 타겟에게 즉시 이동하는지 여부

		// Camera Shaking
		bool			m_bCameraShaking;
		float			m_ShakeIntensity;   
		float			m_ShakeDuration;
		float			m_ShakeTime;
		int				m_ShakeDir;


	public:
		void SetWindowResolution(Vector2 _WindowResolution)
		{
			m_WindowResolution = _WindowResolution;
		}

		void SetWorldResolution(Vector2 _WorldResolution)
		{
			m_WorldResolution = _WorldResolution;
		}

		void SetTarget(GameObject* _Target);

		void SetOffset(Vector2 _Offset)
		{
			m_Offset = _Offset;
		}

		void SetSpeed(float _Speed)
		{
			m_Speed = _Speed;
		}

		void SetTargetMove(bool _b)
		{
			m_bTargetMove = _b;
		}

		void RequestCameraShaking(float _Intensity, float _Duration);

	public:
		virtual void Initialize() override;
		virtual void Update() override;

	private:
		void CameraMove();
		void FollowTarget();
		void CameraShaking();
	};
}