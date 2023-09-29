#pragma once
#include "Lu_MonsterScript.h"
#include "Lu_ScriptEnums.h"
#include "Lu_SnabyStateScript.h"

namespace Lu
{
	class SnabyScript : public MonsterScript
	{
	public:
		SnabyScript();
		virtual ~SnabyScript();

	private:
		std::map<SnabyStateScript::eState, SnabyStateScript*> m_State;
		SnabyStateScript*			m_CurState;
		SnabyStateScript::eState	m_PrevState;

	public:
		virtual void Initialize() override;
		virtual void Update() override;

	public:
		virtual void OnCollisionEnter(Collider2D* _Other) override;
		virtual void OnCollisionStay(Collider2D* _Other) override;

	private:
		SnabyStateScript* GetStateScript(SnabyStateScript::eState _State);
		void AddState(SnabyStateScript* _State);
		void CompleteAction();
		void AttackSFX();
		void DeadSFX();

	protected:
		virtual void CreateAnimation() override;
		virtual void AnimationUpdate() override;
		virtual void ChangeIdleState() override;
		virtual void ChangeDeadState() override;

	public:
		void ChangeState(SnabyStateScript::eState _NextState);
	};
}


