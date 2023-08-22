#pragma once
#include "..\\Engine_SOURCE\\Lu_Script.h"

namespace Lu
{
	class SlimeScript;
	class PlayerScript;
	class SlimeStateScript : public Script
	{
	public:
		enum class eState
		{
			Idle,
			Patrol,
			Trace,
			Attack,
			JumpAttack,
			Dead,
			End
		};

	public:
		SlimeStateScript();
		virtual ~SlimeStateScript();

	private:
		eState				m_StateType;
		SlimeScript*		m_SlimeScript;
		Transform*			m_Transform;
		PlayerScript*		m_Target;
		Vector3				m_ActionDir;
		float				m_Time;

	public:
		void SetStateType(eState _StateType)
		{
			m_StateType = _StateType;
		}

		void SetSlimeScript(SlimeScript* _SlimeScript)
		{
			m_SlimeScript = _SlimeScript;
		}

		void SetTransform(Transform* _Transform)
		{
			m_Transform = _Transform;
		}

		void SetDir(Vector3 _Dir)
		{
			m_ActionDir = _Dir;
		}

		void SetTime(float _Time)
		{
			m_Time = _Time;
		}

	public:
		eState GetStateType()	const
		{
			return m_StateType;
		}

		SlimeScript* GetSlimeScript()	const
		{
			return m_SlimeScript;
		}

		Transform* GetTransform()	const
		{
			return m_Transform;
		}

		PlayerScript* GetTarget();

		Vector3 GetDir()	const
		{
			return m_ActionDir;
		}

		float GetTime()	const
		{
			return m_Time;
		}

	public:
		virtual void Initialize() override;
		virtual void Update() override {}

	public:
		virtual void Enter() = 0;
		virtual void Exit() = 0;

	protected:
		void DetermineAnimDir(float _Angle);
		void DetermineAnimDir(Vector3 _Dir);
		void ChangeStateAfterTime(float _Time, eState _State);
		Vector2 CalDirToPlayer();
	};
}