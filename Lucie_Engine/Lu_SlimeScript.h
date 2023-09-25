#pragma once
#include "Lu_MonsterScript.h"
#include "Lu_ScriptEnums.h"
#include "Lu_SlimeStateScript.h"

namespace Lu
{
	class PlayerScript;
	class SlimeScript : public MonsterScript
	{
	public:
		SlimeScript();
		virtual ~SlimeScript();

	private:
		std::map<SlimeStateScript::eState, SlimeStateScript*> m_State;
		SlimeStateScript*			m_CurState;
		SlimeStateScript::eState	m_PrevState;

		PlayerScript*				m_Target;

		float						m_HitCoolTime;

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
		SlimeStateScript* GetStateScript(SlimeStateScript::eState _State);
		void AddState(SlimeStateScript* _State);
		void CompleteAction();
		void AttackSFX();
		void JumpSFX();
		void DeadSFX();

	protected:
		virtual void CreateAnimation() override;
		virtual void AnimationUpdate() override;

	public:
		void ChangeState(SlimeStateScript::eState _NextState);
	};
}