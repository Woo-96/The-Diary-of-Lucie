#pragma once
#include "..\\Engine_SOURCE\\Lu_Script.h"
#include "Lu_ScriptEnums.h"
#include "Lu_StateScript.h"

namespace Lu
{
	class Animator;
	class PlayerScript : public Script
	{
	public:
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

		struct tPlayerInfo
		{
			int HP;

			tPlayerInfo()
				: HP(2)
			{

			}
		};

	public:
		PlayerScript();
		virtual ~PlayerScript();

	private:
		std::map<StateScript::eState, StateScript*> m_State;
		StateScript*		m_CurState;
		StateScript::eState	m_PrevState;

		eDir				m_Dir;
		eDir				m_PrevDir;

		eMoveType			m_MoveType;
		eWeaponType			m_CurWeapon;

		tPlayerInfo			m_PlayerInfo;

		bool				m_bAction;
		bool				m_bInvincible;
		bool				m_bHitEffect;
		float				m_InvincibleTime;
		int					m_Damage;

		Animator*			m_Animator;

	public:
		void SetAction(bool _Action)
		{
			m_bAction = _Action;
		}

		void SetDir(eDir _Dir)
		{
			m_Dir = _Dir;
		}

	public:
		eDir GetDir()	const
		{
			return m_Dir;
		}

		eMoveType GetMoveType()	const
		{
			return m_MoveType;
		}

		eWeaponType GetWeaponType()	const
		{
			return m_CurWeapon;
		}

		tPlayerInfo& GetPlayerInfo()
		{
			return m_PlayerInfo;
		}

	public:
		virtual void Initialize() override;
		virtual void Update() override;

	public:
		virtual void OnCollisionEnter(Collider2D* _Other) override;
		virtual void OnCollisionStay(Collider2D* _Other) override;
		virtual void OnCollisionExit(Collider2D* _Other) override;

	private:
		void CreatePlayerAnimation();
		StateScript* GetStateScript(StateScript::eState _State);
		void AddState(StateScript* _State);
		void StateUpdate();
		void AnimationUpdate();
		eDir CalDirToMouse();
		void CompleteAction();
		void AttackSFX();

	public:
		void ChangeState(StateScript::eState _NextState);
	};
}


