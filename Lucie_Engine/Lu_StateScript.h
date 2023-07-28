#pragma once
#include "..\\Engine_SOURCE\\Lu_Script.h"

namespace Lu
{
	class PlayerScript;
	class Transform;
	class StateScript :	public Script
	{
	public:
		enum class eState
		{
			Idle,
			Move,
			Dash,
			Attack,
			Hit,
			Dead,
			End
		};

	public:
		StateScript();
		virtual ~StateScript();

	private:
		eState				m_StateType;
		PlayerScript*		m_PlayerScript;
		Transform*			m_Transform;
		float				m_ActionSpeed;
		Vector3				m_ActionDir;

	public:
		void SetStateType(eState _StateType)
		{
			m_StateType = _StateType;
		}

		void SetPlayerScript(PlayerScript* _PlayerScript)
		{
			m_PlayerScript = _PlayerScript;
		}

		void SetTransform(Transform* _Transform)
		{
			m_Transform = _Transform;
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

		PlayerScript* GetPlayerScript()	const
		{
			return m_PlayerScript;
		}

	public:
		virtual void Initialize() override;
		virtual void Update() override {}

	public:
		virtual void Enter() = 0;
		virtual void Exit() = 0;

	protected:
		void CalDirToMouse();
	};
}


