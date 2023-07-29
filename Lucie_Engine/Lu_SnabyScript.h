#pragma once
#include "Lu_MonsterScript.h"
#include "Lu_ScriptEnums.h"
#include "Lu_SnabyStateScript.h"

namespace Lu
{
	class Animator;
	class SnabyScript : public MonsterScript
	{
	public:


	public:
		SnabyScript();
		virtual ~SnabyScript();

	private:
		std::map<SnabyStateScript::eState, SnabyStateScript*> m_State;
		SnabyStateScript*			m_CurState;
		SnabyStateScript::eState	m_PrevState;
		eDir						m_Dir;
		eDir						m_PrevDir;
		Animator*					m_Animator;

	public:
		virtual void Initialize() override;
		virtual void Update() override;

	public:
		virtual void OnCollisionEnter(Collider2D* _Other) override;
		virtual void OnCollisionStay(Collider2D* _Other) override;
		virtual void OnCollisionExit(Collider2D* _Other) override;

	private:
		void CreateSnabyAnimation();
		SnabyStateScript* GetStateScript(SnabyStateScript::eState _State);
		void AddState(SnabyStateScript* _State);
		void AnimationUpdate();
		eDir CalDirToPlayer();
		void CompleteAction();

	public:
		void ChangeState(SnabyStateScript::eState _NextState);
	};
}


