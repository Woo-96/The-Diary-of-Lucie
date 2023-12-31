#pragma once
#include "Lu_MonsterScript.h"
#include "Lu_ScriptEnums.h"
#include "Lu_BigSlimeStateScript.h"

namespace Lu
{
	class PlayerScript;
	class GameObject;
	class BigSlimeScript : public MonsterScript
	{
	public:
		BigSlimeScript();
		virtual ~BigSlimeScript();

	private:
		std::map<BigSlimeStateScript::eState, BigSlimeStateScript*> m_State;
		BigSlimeStateScript* m_CurState;
		BigSlimeStateScript::eState	m_PrevState;

		GameObject*						m_HPFrame;
		GameObject*						m_HPBar;

	public:
		void SetHPFrame(GameObject* _HPFrame)
		{
			m_HPFrame = _HPFrame;
		}

		void SetHPBar(GameObject* _HPBar)
		{
			m_HPBar = _HPBar;
		}

	public:
		GameObject* GetHPFrame()	const
		{
			return m_HPFrame;
		}

		GameObject* GetHPBar()	const
		{
			return m_HPBar;
		}

	public:
		virtual void Initialize() override;
		virtual void Update() override;

	public:
		virtual void OnCollisionEnter(Collider2D* _Other) override;
		virtual void OnCollisionStay(Collider2D* _Other) override;

	private:
		BigSlimeStateScript* GetStateScript(BigSlimeStateScript::eState _State);
		void AddState(BigSlimeStateScript* _State);
		void CompleteAction();
		void JumpSFX();
		void AttackSFX();
		void DeadSFX();

	protected:
		virtual void CreateAnimation() override;
		virtual void AnimationUpdate() override;
		virtual void ChangeIdleState() override;
		virtual void ChangeDeadState() override;

	public:
		void ChangeState(BigSlimeStateScript::eState _NextState);
	};
}