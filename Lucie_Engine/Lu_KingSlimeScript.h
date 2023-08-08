#pragma once
#include "Lu_MonsterScript.h"
#include "Lu_ScriptEnums.h"
#include "Lu_KingSlimeStateScript.h"

namespace Lu
{
	class PlayerScript;
	class KingSlimeScript : public MonsterScript
	{
	public:


	public:
		KingSlimeScript();
		virtual ~KingSlimeScript();

	private:
		std::map<KingSlimeStateScript::eState, KingSlimeStateScript*> m_State;
		KingSlimeStateScript*			m_CurState;
		KingSlimeStateScript::eState	m_PrevState;

		PlayerScript*					m_Target;

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
		KingSlimeStateScript* GetStateScript(KingSlimeStateScript::eState _State);
		void AddState(KingSlimeStateScript* _State);
		void CompleteAction();

	protected:
		virtual void CreateAnimation() override;
		virtual void AnimationUpdate() override;

	public:
		void ChangeState(KingSlimeStateScript::eState _NextState);
	};
}


