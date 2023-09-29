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
		enum class eSlimeType
		{
			Nomal,
			Boss,
		};

	public:
		SlimeScript();
		virtual ~SlimeScript();

	private:
		std::map<SlimeStateScript::eState, SlimeStateScript*> m_State;
		SlimeStateScript*			m_CurState;
		SlimeStateScript::eState	m_PrevState;

		eSlimeType					m_SlimeType;

	public:
		void SetSlimeType(eSlimeType _Type)
		{
			m_SlimeType = _Type;
		}

	public:
		eSlimeType GetSlimeType()	const
		{
			return m_SlimeType;
		}

	public:
		virtual void Initialize() override;
		virtual void Update() override;

	public:
		virtual void OnCollisionEnter(Collider2D* _Other) override;
		virtual void OnCollisionStay(Collider2D* _Other) override;

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
		virtual void ChangeIdleState() override;
		virtual void ChangeDeadState() override;

	public:
		void ChangeState(SlimeStateScript::eState _NextState);
	};
}