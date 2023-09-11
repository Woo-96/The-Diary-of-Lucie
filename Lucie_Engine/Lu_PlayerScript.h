#pragma once
#include "..\\Engine_SOURCE\\Lu_Script.h"
#include "Lu_ScriptEnums.h"
#include "Lu_StateScript.h"

namespace Lu
{
	class Animator;
	class HeartScript;
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
			int MaxHP;
			int CurHP;
			int MaxMP;
			int CurMP;

			tPlayerInfo()
				: MaxHP(6)
				, CurHP(6)
				, MaxMP(3)
				, CurMP(3)
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
		bool				m_bDontAnimChange;
		float				m_InvincibleTime;
		int					m_Damage;

		Animator*			m_Animator;

		HeartScript*		m_HPScript;

	public:
		void SetAction(bool _Action)
		{
			m_bAction = _Action;
		}

		void SetDir(eDir _Dir)
		{
			m_Dir = _Dir;
		}

		void SetHPScript(HeartScript* _HPScript)
		{
			m_HPScript = _HPScript;
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
		void MagicCircleMove();
		void LookAround();

	public:
		void ChangeState(StateScript::eState _NextState);
		void MagicPortalMove();
		void IdleStateEvent();
		void InfoUpdate();
	};
}


