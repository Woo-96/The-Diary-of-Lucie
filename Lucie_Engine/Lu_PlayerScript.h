#pragma once
#include "..\\Engine_SOURCE\\Lu_Script.h"
#include "Lu_StateScript.h"

namespace Lu
{
	class Animator;
	class PlayerScript : public Script
	{
	public:
		enum class ePlayerDir
		{
			None,
			Left,
			Right,
			Up,
			Down,
			LeftUp,
			RightUp,
			LeftDown,
			RightDown
		};

		enum class eMoveType
		{
			Walk,
			Run
		};

		enum class eWeaponType
		{
			None,
			Sword,
			Bow,
			Wand,
		};

		enum class eSkillType
		{
			End
		};

	public:
		PlayerScript();
		virtual ~PlayerScript();

	private:
		std::map<StateScript::eState, StateScript*> m_State;
		StateScript*		m_CurState;
		StateScript::eState	m_PrevState;

		ePlayerDir			m_Dir;
		ePlayerDir			m_PrevDir;

		eMoveType			m_MoveType;
		eWeaponType			m_CurWeapon;

		bool				m_bAction;

		Animator*			m_Animator;

	public:
		void SetAction(bool _Action)
		{
			m_bAction = _Action;
		}

	public:
		ePlayerDir GetDir()	const
		{
			return m_Dir;
		}

		eMoveType GetMoveType()	const
		{
			return m_MoveType;
		}

	public:
		virtual void Initialize() override;
		virtual void Update() override;

	public:
		virtual void OnCollisionEnter(Collider2D* other) override;
		virtual void OnCollisionStay(Collider2D* other) override;
		virtual void OnCollisionExit(Collider2D* other) override;

	private:
		void CreatePlayerAnimation();
		StateScript* GetStateScript(StateScript::eState _State);
		void AddState(StateScript* _State);
		void StateUpdate();
		void AnimationUpdate();
		ePlayerDir CalDirToMouse();
		void CompleteAction();

	public:
		void ChangeState(StateScript::eState _NextState);
	};
}


