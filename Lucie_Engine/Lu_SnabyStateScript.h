#pragma once
#include "..\\Engine_SOURCE\\Lu_Script.h"

namespace Lu
{
	class SnabyScript;
	class PlayerScript;
	class SnabyStateScript : public Script
	{
	public:
		enum class eState
		{
			Idle,
			Patrol,
			Attack,
			Dead,
			End
		};

	public:
		SnabyStateScript();
		virtual ~SnabyStateScript();

	private:
		eState				m_StateType;
		SnabyScript*		m_SnabyScript;
		Transform*			m_Transform;
		PlayerScript*		m_Target;
		float				m_ActionSpeed;
		Vector3				m_ActionDir;

	public:
		void SetStateType(eState _StateType)
		{
			m_StateType = _StateType;
		}

		void SetSnabyScript(SnabyScript* _SnabyScript)
		{
			m_SnabyScript = _SnabyScript;
		}

		void SetTransform(Transform* _Transform)
		{
			m_Transform = _Transform;
		}

		void SetTarget(PlayerScript* _Target)
		{
			m_Target = _Target;
		}

		void SetSpeed(float _Speed)
		{
			m_ActionSpeed = _Speed;
		}

	public:
		Transform* GetTransform()	const
		{
			return m_Transform;
		}

		PlayerScript* GetTarget()	const
		{
			return m_Target;
		}

		float GetSpeed()	const
		{
			return m_ActionSpeed;
		}

		Vector3 GetDir()	const
		{
			return m_ActionDir;
		}

	public:
		eState GetStateType()	const
		{
			return m_StateType;
		}

		SnabyScript* GetSnabyScript()	const
		{
			return m_SnabyScript;
		}

	public:
		virtual void Initialize() override;
		virtual void Update() override {}

	public:
		virtual void Enter() = 0;
		virtual void Exit() = 0;
	};
}