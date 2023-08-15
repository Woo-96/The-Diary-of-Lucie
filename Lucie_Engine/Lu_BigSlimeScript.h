#pragma once
#include "Lu_MonsterScript.h"
#include "Lu_ScriptEnums.h"
#include "Lu_BigSlimeStateScript.h"

namespace Lu
{
	class PlayerScript;
	class BigSlimeScript : public MonsterScript
	{
	public:
		BigSlimeScript();
		virtual ~BigSlimeScript();

	private:
		std::map<BigSlimeStateScript::eState, BigSlimeStateScript*> m_State;
		BigSlimeStateScript* m_CurState;
		BigSlimeStateScript::eState	m_PrevState;

		PlayerScript* m_Target;

		float							m_Time;
		bool							m_bAttack;

	public:
		void SetTarget(PlayerScript* _Target)
		{
			m_Target = _Target;
		}

	public:
		PlayerScript* GetTarget()	const
		{
			return m_Target;
		}

	public:
		virtual void Initialize() override;
		virtual void Update() override;

	public:
		virtual void OnCollisionEnter(Collider2D* _Other) override;
		virtual void OnCollisionStay(Collider2D* _Other) override;
		virtual void OnCollisionExit(Collider2D* _Other) override;

	private:
		BigSlimeStateScript* GetStateScript(BigSlimeStateScript::eState _State);
		void AddState(BigSlimeStateScript* _State);
		void CompleteAction();
		void CircleAttack();

	protected:
		virtual void CreateAnimation() override;
		virtual void AnimationUpdate() override;

	public:
		void ChangeState(BigSlimeStateScript::eState _NextState);
	};
}