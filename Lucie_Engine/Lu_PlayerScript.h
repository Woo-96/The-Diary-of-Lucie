#pragma once
#include "..\\Engine_SOURCE\\Lu_Script.h"
#include "Lu_ScriptEnums.h"
#include "Lu_StateScript.h"

namespace Lu
{
	class Animator;
	class UIScript;
	class PlayerScript : public Script
	{
	public:
		enum class eMoveType
		{
			Walk,
			Run
		};

		enum class eSkillType
		{
			End
		};

		enum class eUI
		{
			HP,
			MP,
			EXP,
			Level,
			TP,
			Gold,
			QuickItem,
			WeaponSlot,
			End,
		};

		struct tPlayerInfo
		{
			int MaxHP;
			int CurHP;
			int MaxMP;
			int CurMP;
			int MaxEXP;
			int CurEXP;
			int MaxLevel;
			int CurLevel;
			float MaxTP;
			float CurTP;
			float TPRecoveryRate;
			int MaxGold;
			int CurGold;

			tPlayerInfo()
				: MaxHP(6)
				, CurHP(6)
				, MaxMP(3)
				, CurMP(3)
				, MaxEXP(100)
				, CurEXP(0)
				, MaxLevel(9)
				, CurLevel(1)
				, MaxTP(100.f)
				, CurTP(100.f)
				, TPRecoveryRate(10.f)
				, MaxGold(9)
				, CurGold(0)
			{

			}
		};

	public:
		PlayerScript();
		virtual ~PlayerScript();

	private:
		std::map<StateScript::eState, StateScript*> m_State;
		StateScript*			m_CurState;
		StateScript::eState		m_PrevState;

		eDir					m_Dir;
		eDir					m_PrevDir;

		eMoveType				m_MoveType;
		eWeaponType				m_CurWeapon;

		tPlayerInfo				m_PlayerInfo;

		bool					m_bAction;
		bool					m_bInvincible;
		bool					m_bHitEffect;
		bool					m_bDontAnimChange;
		float					m_InvincibleTime;
		int						m_Damage;

		Animator*				m_Animator;

		UIScript*				m_arrUI[(int)eUI::End];

	public:
		void SetAction(bool _Action)
		{
			m_bAction = _Action;
		}

		void SetDir(eDir _Dir)
		{
			m_Dir = _Dir;
		}

		void SetUI(eUI _UIType, UIScript* _UI)
		{
			m_arrUI[(int)_UIType] = _UI;
		}

		void SetWeaponType(eWeaponType _Type)
		{
			m_CurWeapon = _Type;
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

		UIScript* GetUI(eUI _UIType)	const
		{
			return m_arrUI[(int)_UIType];
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
		void StaminaRecovery();

	public:
		void ChangeState(StateScript::eState _NextState);
		void MagicPortalMove();
		void IdleStateEvent();

	// UI 상호 작용 관련 함수
	public:
		void InflictDamage(int _Damage);
		void UseMana(int _Value);
		void GetMaxMana(int _Value);
		void UseStamina(float _Value);
		void GetEXP(int _Value);
		void GetCoin(int _Value);
	};
}


