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

		enum class eUI
		{
			Layout,
			HP,
			MP,
			EXP,
			Level,
			TP,
			Gold,
			QuickItem,
			WeaponSlot,
			Channeling,
			Inventory,
			Skill,
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
			int Attack;
			int Magic;
			float MaxTP;
			float CurTP;
			int MaxLevel;
			int CurLevel;
			float TPRecoveryRate;
			int MaxGold;
			int CurGold;

			tPlayerInfo()
				: MaxHP(6)
				, CurHP(6)
				, MaxMP(3)
				, CurMP(3)
				, MaxTP(10.f)
				, CurTP(10.f)
				, MaxEXP(100)
				, CurEXP(0)
				, Attack(10)
				, Magic(10)
				, MaxLevel(9)
				, CurLevel(1)
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
		tSkill*					m_CurSkill;

		tPlayerInfo				m_PlayerInfo;

		bool					m_bAction;					// Ư�� ���ۿ��� Ű �Է� ����
		bool					m_bInvincible;				// �뽬 ����
		bool					m_bDashSuccess;				// �뽬 �� SFX�� ���� ȸ���� �̹� ����Ǿ�����
		bool					m_bHitEffect;				// ��Ʈ �� ������ ����
		bool					m_bDontAnimChange;			// ������ ������ ���̵� ���·� ��ȯ�Ǵ� ���� ����
		bool					m_bCantHit;					// �κ��丮 �������� ���� ����
		bool					m_bChanneling;				// ä�θ� ��ų �����
		bool					m_bChargeAnim;				// ä�θ� ������ ���� �ִϸ��̼� ��¿�
		float					m_ChargeGauge;				// ä�θ� ��ų ���� ��¡ ������
		float					m_InvincibleTime;			// ���� �ܿ� �ð�
		int						m_Damage;					// ���⿡ ���� �޴� ���� �����

		bool					m_bSkillUse;				// ��ų �����
		bool					m_bFirst;					// ��ų ��� ��������
		float					m_SkillProjectileCoolTime;	// ����ü�� �� ������ ������ ���� ����

		bool					m_bWood;

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

		void SetWeaponType(eWeaponType _Type, int _Damage);

		void SetCantHit(bool _b)
		{
			m_bCantHit = _b;
		}

		void ResetChargeGauge()
		{
			m_ChargeGauge = 0.f;
		}

		void SetCurSkill(tSkill* _Skill)
		{
			m_CurSkill = _Skill;
		}

		void SetWood(bool _b)
		{
			m_bWood = _b;
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

		tSkill* GetCurSkill()
		{
			return m_CurSkill;
		}

		tPlayerInfo& GetPlayerInfo()
		{
			return m_PlayerInfo;
		}

		UIScript* GetUI(eUI _UIType)	const
		{
			return m_arrUI[(int)_UIType];
		}

		int GetPlayerDamage();

		float GetChargeGauge()	const
		{
			return m_ChargeGauge;
		}

		bool IsWoodGround()	const
		{
			return m_bWood;
		}

		bool IsSkillUse()	const
		{
			return m_bSkillUse;
		}

	public:
		virtual void Initialize() override;
		virtual void Update() override;

	public:
		virtual void OnCollisionEnter(Collider2D* _Other) override;
		virtual void OnCollisionExit(Collider2D* _Other) override;

	private:
		void CreatePlayerAnimation();
		StateScript* GetStateScript(StateScript::eState _State);
		void AddState(StateScript* _State);
		void StateUpdate();
		void AnimationUpdate();
		eDir CalDirToMouse();

		void CompleteAction();
		void DashFinish();
		void AttackSFX();
		void MagicCircleMove();
		void LookAround();
		
		void StaminaRecovery();
		
		void Skill_IceBall();
		void Skill_Cast();

	public:
		void ChangeState(StateScript::eState _NextState);
		void MagicPortalMove();
		void IdleStateEvent();
		void LearnSkill(tSkill* _Skill);
		void ResetPlayerInfo();

	// UI ��ȣ �ۿ� ���� �Լ�
	public:
		void InflictDamage(int _Damage);
		void UseMana(int _Value);
		void GetMaxMana(int _Value);
		void UseStamina(float _Value);
		void GetEXP(int _Value);
		void GetCoin(int _Value);
	};
}


