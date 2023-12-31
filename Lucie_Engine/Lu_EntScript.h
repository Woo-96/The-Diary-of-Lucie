#pragma once
#include "Lu_MonsterScript.h"
#include "Lu_ScriptEnums.h"
#include "Lu_EntStateScript.h"

namespace Lu
{
	class PlayerScript;
	class GameObject;
	class EntScript : public MonsterScript
	{
	public:
		enum class ePhase
		{
			Phase_1,
			Phase_2,
			End,
		};

		enum class eAttackType
		{
			Thorn,
			PoisonBreath,
			WindBreath,
			TomatoBomb,
			Crater,
			End,
		};

	public:
		EntScript();
		virtual ~EntScript();

	private:
		std::map<EntStateScript::eState, EntStateScript*> m_State;
		EntStateScript*				m_CurState;
		EntStateScript::eState		m_PrevState;

		GameObject*					m_HPFrame;
		GameObject*					m_HPBar;

		ePhase						m_CurPhase;
		eAttackType					m_CurAttack;

		bool						m_bWakeUp;
		bool						m_bHowling;
		bool						m_bPatternStart;

		bool						m_bRandomAttack;
		int							m_AttackNumber;

	public:
		void SetAttackType(eAttackType _Type)
		{
			m_CurAttack = _Type;
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

		ePhase GetPhase()	const
		{
			return m_CurPhase;
		}

		bool IsRandomAttack()	const
		{
			return m_bRandomAttack;
		}

		int GetAttackNumber()	const
		{
			return m_AttackNumber;
		}

		bool IsPatternStart()	const
		{
			return m_bPatternStart;
		}

	public:
		virtual void Initialize() override;
		virtual void Update() override;

	public:
		virtual void OnCollisionEnter(Collider2D* _Other) override;
		virtual void OnCollisionStay(Collider2D* _Other) override;

	private:
		EntStateScript* GetStateScript(EntStateScript::eState _State);
		void AddState(EntStateScript* _State);
		void CompleteAction();

		void HowlingSFX();
		void AttackPoisonBreathSFX();
		void AttackWindBreathSFX();
		void RageSFX();
		void DeadSFX();

		void Cheat();

	protected:
		virtual void CreateAnimation() override;
		virtual void AnimationUpdate() override;
		virtual void ChangeIdleState() override;
		virtual void ChangeDeadState() override;

	public:
		void ChangeState(EntStateScript::eState _NextState);
		void PatternStart();

	public:
		void WakeUp();
		void Howling();
	};
}


