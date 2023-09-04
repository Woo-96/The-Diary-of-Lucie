#pragma once
#include "Lu_MonsterScript.h"
#include "Lu_ScriptEnums.h"
#include "Lu_KingSlimeStateScript.h"

namespace Lu
{
	class PlayerScript;
	class GameObject;
	class KingSlimeScript : public MonsterScript
	{
	public:
		KingSlimeScript();
		virtual ~KingSlimeScript();

	private:
		std::map<KingSlimeStateScript::eState, KingSlimeStateScript*> m_State;
		KingSlimeStateScript*			m_CurState;
		KingSlimeStateScript::eState	m_PrevState;

		PlayerScript*					m_Target;
		GameObject*						m_HPFrame;
		GameObject*						m_HPBar;

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
		virtual void OnCollisionExit(Collider2D* _Other) override;

	private:
		KingSlimeStateScript* GetStateScript(KingSlimeStateScript::eState _State);
		void AddState(KingSlimeStateScript* _State);
		void CompleteAction();
		void JumpSFX();
		void AttackSFX();
		void DeadSFX();
		void CircleAttack();

	protected:
		virtual void CreateAnimation() override;
		virtual void AnimationUpdate() override;

	public:
		void ChangeState(KingSlimeStateScript::eState _NextState);
	};
}


