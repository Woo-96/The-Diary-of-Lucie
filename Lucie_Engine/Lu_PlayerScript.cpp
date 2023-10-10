#include "Lu_PlayerScript.h"
#include "Lu_Animator.h"
#include "Lu_GameObject.h"
#include "Lu_Input.h"
#include "Lu_Resources.h"
#include "Lu_Renderer.h"
#include "Lu_Camera.h"
#include "Lu_Time.h"
#include "Lu_MeshRenderer.h"
#include "Lu_SceneManager.h"
#include "Lu_AudioSource.h"
#include "Lu_SoundManager.h"
#include "Lu_HeartScript.h"
#include "Lu_ManaScript.h"
#include "Lu_ProgressBarScript.h"
#include "Lu_NumberScript.h"
#include "Lu_QuickItemScript.h"
#include "Lu_WeaponSlotScript.h"
#include "Lu_InventoryScript.h"
#include "Lu_ChannelingBarScript.h"
#include "Lu_Object.h"
#include "Lu_IceBallScript.h"
#include "Lu_LayoutScript.h"
#include "Lu_SkillScript.h"
#include "Lu_SkillCastScript.h"

#include "Lu_IdleState.h"
#include "Lu_MoveState.h"
#include "Lu_DashState.h"
#include "Lu_AttackState.h"
#include "Lu_DeadState.h"

namespace Lu
{
	PlayerScript::PlayerScript()
		: m_CurState(nullptr)
		, m_PrevState(StateScript::eState::End)
		, m_Dir(eDir::Down)
		, m_PrevDir(eDir::None)
		, m_MoveType(eMoveType::Walk)
		, m_CurWeapon(eWeaponType::None)
		, m_CurSkill{}
		, m_bAction(false)
		, m_bInvincible(false)
		, m_bDashSuccess(false)
		, m_bHitEffect(false)
		, m_bDontAnimChange(false)
		, m_bCantHit(false)
		, m_bChanneling(false)
		, m_bChargeAnim(false)
		, m_ChargeGauge(0.f)
		, m_InvincibleTime(0.f)
		, m_Damage(1)
		, m_bSkillUse(false)
		, m_bFirst(false)
		, m_SkillProjectileCoolTime(0.f)
		, m_bWood(false)
		, m_AppearFX(nullptr)
		, m_Animator(nullptr)
		, m_arrUI{}
	{
		SetName(L"PlayerScript");
	}

	PlayerScript::~PlayerScript()
	{
		for (const auto& pair : m_State)
		{
			if (nullptr != pair.second)
				delete pair.second;
		}

		m_State.clear();
	}

	void PlayerScript::SetWeaponType(eWeaponType _Type, int _Damage)
	{
		m_CurWeapon = _Type;

		if (m_CurWeapon == eWeaponType::None)
			return;
		else if (m_CurWeapon == eWeaponType::Wand)
		{
			m_PlayerInfo.Magic += _Damage;
		}
		else
		{
			m_PlayerInfo.Attack += _Damage;
		}
	}

	int PlayerScript::GetPlayerDamage()
	{
		switch (m_CurWeapon)
		{
		case Lu::eWeaponType::Wand:
			return m_PlayerInfo.Magic;
			break;
		case Lu::eWeaponType::Bow:
			return m_PlayerInfo.Attack;
			break;
		case Lu::eWeaponType::Sword:
			break;
		case Lu::eWeaponType::None:
			return m_PlayerInfo.Magic / 2;
			break;
		default:
			return 0;
			break;
		}
	}

	void PlayerScript::Initialize()
	{
		m_Animator = GetOwner()->GetComponent<Animator>();
		CreatePlayerAnimation();

		AddState(new IdleState);
		AddState(new MoveState);
		AddState(new DashState);
		AddState(new AttackState);
		AddState(new DeadState);

		m_CurState = GetStateScript(StateScript::eState::Idle);
		m_CurState->Enter();

		m_CurWeapon = eWeaponType::None;
	}

	void PlayerScript::Update()
	{
		if (GetOwner()->IsDead())
			return;

		// 무적 상태 블링크
		if (m_bInvincible)
		{
			m_InvincibleTime -= (float)Time::DeltaTime();

			if (m_bHitEffect)
			{
				if (m_InvincibleTime >= 0.f && ((int)(m_InvincibleTime * 10) % 2) == 0)
				{
					int bInvincible = m_bInvincible;
					float fAlpha = 0.3f;
					GetOwner()->GetComponent<MeshRenderer>()->GetMaterial()->SetScalarParam(Lu::graphics::SCALAR_PARAM::INT_3, &bInvincible);
					GetOwner()->GetComponent<MeshRenderer>()->GetMaterial()->SetScalarParam(Lu::graphics::SCALAR_PARAM::FLOAT_3, &fAlpha);
				}
				else
				{
					int bInvincible = m_bInvincible;
					float fAlpha = 1.f;
					GetOwner()->GetComponent<MeshRenderer>()->GetMaterial()->SetScalarParam(Lu::graphics::SCALAR_PARAM::INT_3, &bInvincible);
					GetOwner()->GetComponent<MeshRenderer>()->GetMaterial()->SetScalarParam(Lu::graphics::SCALAR_PARAM::FLOAT_3, &fAlpha);
				}
			}

			if (m_InvincibleTime <= 0.f)
			{
				m_bInvincible = false;
				m_InvincibleTime = 0.f;
				
				if (m_bHitEffect)
				{
					int bInvincible = m_bInvincible;
					float fAlpha = 1.f;
					GetOwner()->GetComponent<MeshRenderer>()->GetMaterial()->SetScalarParam(Lu::graphics::SCALAR_PARAM::INT_3, &bInvincible);
					GetOwner()->GetComponent<MeshRenderer>()->GetMaterial()->SetScalarParam(Lu::graphics::SCALAR_PARAM::FLOAT_3, &fAlpha);

					m_bHitEffect = false;
				}
			}
		}

		// 키 입력을 통해 상태 및 방향 업데이트
		StateUpdate();

		// 현재 상태 Update
 		m_CurState->Update();

		// 현재 상태에 따른 애니메이션 업데이트
		AnimationUpdate();

		// 스테미너 자동 회복 기능
		if (m_PlayerInfo.CurTP < m_PlayerInfo.MaxTP)
		{
			StaminaRecovery();
		}

		// 이전 상태 저장
		m_PrevState = m_CurState->GetStateType();

		m_bCantHit = false;
		m_bWood = false;
	}

	void PlayerScript::OnCollisionEnter(Collider2D* _Other)
	{
		if (StateScript::eState::Dead == m_CurState->GetStateType())
			return;

		if ((int)eLayerType::MonsterProjectile == _Other->GetOwner()->GetLayerIndex())
		{
			// Hit & Dead
			if (!m_bInvincible)
			{
				InflictDamage(m_Damage);

				AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
				pSFX->SetClip(Resources::Load<AudioClip>(L"HitSFX", L"..\\Resources\\Sound\\SFX\\Player\\InflicDamageSFX.ogg"));
				pSFX->Play();

				if (m_PlayerInfo.CurHP <= 0)
				{
					m_PlayerInfo.CurHP = 0;
					m_bInvincible = false;
					m_bHitEffect = false;
					m_InvincibleTime = 0.f;
					ChangeState(StateScript::eState::Dead);
				}
				else
				{
					m_bInvincible = true;
					m_bHitEffect = true;
					m_InvincibleTime = 1.f;		// 피격 무적
				}
			}
			else
			{
				if (StateScript::eState::Dash == m_CurState->GetStateType()
					&& !m_bDashSuccess)
				{
					UseMana(-1);

					AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
					pSFX->SetClip(Resources::Load<AudioClip>(L"AvoidSFX", L"..\\Resources\\Sound\\SFX\\Player\\AvoidSFX.ogg"));
					pSFX->Play();

					m_bDashSuccess = true;
				}
			}
		}
	}

	void PlayerScript::OnCollisionExit(Collider2D* _Other)
	{
		if ((int)eLayerType::Immovable == _Other->GetOwner()->GetLayerIndex())
			return;
	}

	void PlayerScript::CreatePlayerAnimation()
	{
		std::shared_ptr<Texture> pAtlas
			= Resources::Load<Texture>(L"PlayerSprite", L"..\\Resources\\Texture\\Player\\Player.png");

		// Idle
		m_Animator->Create(L"Player_Idle_Left", pAtlas, Vector2(400.f, 100.f), Vector2(100.f, 100.f), 1, Vector2(100.f, 100.f));
		m_Animator->Create(L"Player_Idle_Right", pAtlas, Vector2(100.f, 200.f), Vector2(100.f, 100.f), 1, Vector2(100.f, 100.f));
		m_Animator->Create(L"Player_Idle_Up", pAtlas, Vector2(100.f, 300.f), Vector2(100.f, 100.f), 1, Vector2(100.f, 100.f));
		m_Animator->Create(L"Player_Idle_Down", pAtlas, Vector2(400.f, 0.f), Vector2(100.f, 100.f), 1, Vector2(100.f, 100.f));
		m_Animator->Create(L"Player_Idle_LeftUp", pAtlas, Vector2(400.f, 200.f), Vector2(100.f, 100.f), 1, Vector2(100.f, 100.f));
		m_Animator->Create(L"Player_Idle_RightUp", pAtlas, Vector2(400.f, 300.f), Vector2(100.f, 100.f), 1, Vector2(100.f, 100.f));
		m_Animator->Create(L"Player_Idle_LeftDown", pAtlas, Vector2(100.f, 0.f), Vector2(100.f, 100.f), 1, Vector2(100.f, 100.f));
		m_Animator->Create(L"Player_Idle_RightDown", pAtlas, Vector2(100.f, 100.f), Vector2(100.f, 100.f), 1, Vector2(100.f, 100.f));

		// Walk
		m_Animator->Create(L"Player_Walk_Left", pAtlas, Vector2(300.f, 100.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);
		m_Animator->Create(L"Player_Walk_Right", pAtlas, Vector2(0.f, 200.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);
		m_Animator->Create(L"Player_Walk_Up", pAtlas, Vector2(0.f, 300.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);
		m_Animator->Create(L"Player_Walk_Down", pAtlas, Vector2(300.f, 0.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);
		m_Animator->Create(L"Player_Walk_LeftUp", pAtlas, Vector2(300.f, 200.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);
		m_Animator->Create(L"Player_Walk_RightUp", pAtlas, Vector2(300.f, 300.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);
		m_Animator->Create(L"Player_Walk_LeftDown", pAtlas, Vector2::Zero, Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);
		m_Animator->Create(L"Player_Walk_RightDown", pAtlas, Vector2(0.f, 100.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);

		// Run
		m_Animator->Create(L"Player_Run_Left", pAtlas, Vector2(0.f, 2900.f), Vector2(100.f, 100.f), 4, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);
		m_Animator->Create(L"Player_Run_Right", pAtlas, Vector2(0.f, 3000.f), Vector2(100.f, 100.f), 4, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);
		m_Animator->Create(L"Player_Run_Up", pAtlas, Vector2(0.f, 3100.f), Vector2(100.f, 100.f), 4, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);
		m_Animator->Create(L"Player_Run_Down", pAtlas, Vector2(0.f, 2800.f), Vector2(100.f, 100.f), 4, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);
		m_Animator->Create(L"Player_Run_LeftUp", pAtlas, Vector2(0.f, 3400.f), Vector2(100.f, 100.f), 4, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);
		m_Animator->Create(L"Player_Run_RightUp", pAtlas, Vector2(0.f, 3500.f), Vector2(100.f, 100.f), 4, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);
		m_Animator->Create(L"Player_Run_LeftDown", pAtlas, Vector2(0.f, 3200.f), Vector2(100.f, 100.f), 4, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);
		m_Animator->Create(L"Player_Run_RightDown", pAtlas, Vector2(0.f, 3300.f), Vector2(100.f, 100.f), 4, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);

		// Dash
		m_Animator->Create(L"Player_Dash_Left", pAtlas, Vector2(0.f, 4300.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->CompleteEvent(L"Player_Dash_Left") = std::bind(&PlayerScript::DashFinish, this);
		m_Animator->Create(L"Player_Dash_Right", pAtlas, Vector2(0.f, 4400.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->CompleteEvent(L"Player_Dash_Right") = std::bind(&PlayerScript::DashFinish, this);
		m_Animator->Create(L"Player_Dash_Up", pAtlas, Vector2(0.f, 4600.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->CompleteEvent(L"Player_Dash_Up") = std::bind(&PlayerScript::DashFinish, this);
		m_Animator->Create(L"Player_Dash_Down", pAtlas, Vector2(0.f, 4100.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->CompleteEvent(L"Player_Dash_Down") = std::bind(&PlayerScript::DashFinish, this);
		m_Animator->Create(L"Player_Dash_LeftUp", pAtlas, Vector2(0.f, 4500.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->CompleteEvent(L"Player_Dash_LeftUp") = std::bind(&PlayerScript::DashFinish, this);
		m_Animator->Create(L"Player_Dash_RightUp", pAtlas, Vector2(0.f, 4700.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->CompleteEvent(L"Player_Dash_RightUp") = std::bind(&PlayerScript::DashFinish, this);
		m_Animator->Create(L"Player_Dash_LeftDown", pAtlas, Vector2(0.f, 4000.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->CompleteEvent(L"Player_Dash_LeftDown") = std::bind(&PlayerScript::DashFinish, this);
		m_Animator->Create(L"Player_Dash_RightDown", pAtlas, Vector2(0.f, 4200.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->CompleteEvent(L"Player_Dash_RightDown") = std::bind(&PlayerScript::DashFinish, this);

		// Attack - Bow
		m_Animator->Create(L"Player_Bow_Left", pAtlas, Vector2(300.f, 1500.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->StartEvent(L"Player_Bow_Left") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_Bow_Left") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_Bow_Right", pAtlas, Vector2(0.f, 1600.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->StartEvent(L"Player_Bow_Right") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_Bow_Right") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_Bow_Up", pAtlas, Vector2(0.f, 1700.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->StartEvent(L"Player_Bow_Up") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_Bow_Up") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_Bow_Down", pAtlas, Vector2(300.f, 1400.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->StartEvent(L"Player_Bow_Down") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_Bow_Down") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_Bow_LeftUp", pAtlas, Vector2(300.f, 1600.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->StartEvent(L"Player_Bow_LeftUp") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_Bow_LeftUp") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_Bow_RightUp", pAtlas, Vector2(300.f, 1700.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->StartEvent(L"Player_Bow_RightUp") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_Bow_RightUp") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_Bow_LeftDown", pAtlas, Vector2(0.f, 1400.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->StartEvent(L"Player_Bow_LeftDown") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_Bow_LeftDown") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_Bow_RightDown", pAtlas, Vector2(0.f, 1500.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->StartEvent(L"Player_Bow_RightDown") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_Bow_RightDown") = std::bind(&PlayerScript::CompleteAction, this);

		// Attack - Wand
		m_Animator->Create(L"Player_Wand_Left", pAtlas, Vector2(0.f, 5600.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->StartEvent(L"Player_Wand_Left") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_Wand_Left") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_Wand_Right", pAtlas, Vector2(0.f, 5700.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->StartEvent(L"Player_Wand_Right") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_Wand_Right") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_Wand_Up", pAtlas, Vector2(0.f, 5900.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->StartEvent(L"Player_Wand_Up") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_Wand_Up") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_Wand_Down", pAtlas, Vector2(0.f, 5400.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->StartEvent(L"Player_Wand_Down") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_Wand_Down") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_Wand_LeftUp", pAtlas, Vector2(0.f, 5800.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->StartEvent(L"Player_Wand_LeftUp") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_Wand_LeftUp") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_Wand_RightUp", pAtlas, Vector2(0.f, 6000.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->StartEvent(L"Player_Wand_RightUp") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_Wand_RightUp") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_Wand_LeftDown", pAtlas, Vector2(0.f, 5300.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->StartEvent(L"Player_Wand_LeftDown") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_Wand_LeftDown") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_Wand_RightDown", pAtlas, Vector2(0.f, 5500.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->StartEvent(L"Player_Wand_RightDown") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_Wand_RightDown") = std::bind(&PlayerScript::CompleteAction, this);


		// Attack - SkillCast
		m_Animator->Create(L"Player_SkillCast_LeftDown", pAtlas, Vector2(0.f, 7200.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);
		m_Animator->StartEvent(L"Player_SkillCast_LeftDown") = std::bind(&PlayerScript::Skill_Cast, this);
		m_Animator->CompleteEvent(L"Player_SkillCast_LeftDown") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_SkillCast_Down", pAtlas, Vector2(300.f, 7200.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);
		m_Animator->StartEvent(L"Player_SkillCast_Down") = std::bind(&PlayerScript::Skill_Cast, this);
		m_Animator->CompleteEvent(L"Player_SkillCast_Down") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_SkillCast_RightDown", pAtlas, Vector2(0.f, 7300.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);
		m_Animator->StartEvent(L"Player_SkillCast_RightDown") = std::bind(&PlayerScript::Skill_Cast, this);
		m_Animator->CompleteEvent(L"Player_SkillCast_RightDown") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_SkillCast_Left", pAtlas, Vector2(300.f, 7300.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);
		m_Animator->StartEvent(L"Player_SkillCast_Left") = std::bind(&PlayerScript::Skill_Cast, this);
		m_Animator->CompleteEvent(L"Player_SkillCast_Left") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_SkillCast_Right", pAtlas, Vector2(0.f, 7400.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);
		m_Animator->StartEvent(L"Player_SkillCast_Right") = std::bind(&PlayerScript::Skill_Cast, this);
		m_Animator->CompleteEvent(L"Player_SkillCast_Right") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_SkillCast_LeftUp", pAtlas, Vector2(300.f, 7400.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);
		m_Animator->StartEvent(L"Player_SkillCast_LeftUp") = std::bind(&PlayerScript::Skill_Cast, this);
		m_Animator->CompleteEvent(L"Player_SkillCast_LeftUp") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_SkillCast_Up", pAtlas, Vector2(0.f, 7500.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);
		m_Animator->StartEvent(L"Player_SkillCast_Up") = std::bind(&PlayerScript::Skill_Cast, this);
		m_Animator->CompleteEvent(L"Player_SkillCast_Up") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_SkillCast_RightUp", pAtlas, Vector2(300.f, 7500.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);
		m_Animator->StartEvent(L"Player_SkillCast_RightUp") = std::bind(&PlayerScript::Skill_Cast, this);
		m_Animator->CompleteEvent(L"Player_SkillCast_RightUp") = std::bind(&PlayerScript::CompleteAction, this);


		// Attack - WandChanneling
		m_Animator->Create(L"Player_WandChanneling_Left", pAtlas, Vector2(300.f, 1900.f), Vector2(100.f, 100.f), 2, Vector2(100.f, 100.f), Vector2::Zero, 0.3f, false);
		m_Animator->StartEvent(L"Player_WandChanneling_Left") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_WandChanneling_Left") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_WandChanneling_Right", pAtlas, Vector2(0.f, 2000.f), Vector2(100.f, 100.f), 2, Vector2(100.f, 100.f), Vector2::Zero, 0.3f, false);
		m_Animator->StartEvent(L"Player_WandChanneling_Right") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_WandChanneling_Right") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_WandChanneling_Up", pAtlas, Vector2(0.f, 2100.f), Vector2(100.f, 100.f), 2, Vector2(100.f, 100.f), Vector2::Zero, 0.3f, false);
		m_Animator->StartEvent(L"Player_WandChanneling_Up") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_WandChanneling_Up") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_WandChanneling_Down", pAtlas, Vector2(300.f, 1800.f), Vector2(100.f, 100.f), 2, Vector2(100.f, 100.f), Vector2::Zero, 0.3f, false);
		m_Animator->StartEvent(L"Player_WandChanneling_Down") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_WandChanneling_Down") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_WandChanneling_LeftUp", pAtlas, Vector2(300.f, 2000.f), Vector2(100.f, 100.f), 2, Vector2(100.f, 100.f), Vector2::Zero, 0.3f, false);
		m_Animator->StartEvent(L"Player_WandChanneling_LeftUp") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_WandChanneling_LeftUp") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_WandChanneling_RightUp", pAtlas, Vector2(300.f, 2100.f), Vector2(100.f, 100.f), 2, Vector2(100.f, 100.f), Vector2::Zero, 0.3f, false);
		m_Animator->StartEvent(L"Player_WandChanneling_RightUp") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_WandChanneling_RightUp") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_WandChanneling_LeftDown", pAtlas, Vector2(0.f, 1800.f), Vector2(100.f, 100.f), 2, Vector2(100.f, 100.f), Vector2::Zero, 0.3f, false);
		m_Animator->StartEvent(L"Player_WandChanneling_LeftDown") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_WandChanneling_LeftDown") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_WandChanneling_RightDown", pAtlas, Vector2(0.f, 1900.f), Vector2(100.f, 100.f), 2, Vector2(100.f, 100.f), Vector2::Zero, 0.3f, false);
		m_Animator->StartEvent(L"Player_WandChanneling_RightDown") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_WandChanneling_RightDown") = std::bind(&PlayerScript::CompleteAction, this);



		// Attack - ChannelingSkill
		m_Animator->Create(L"Player_ChannelingSkill_Idle_Left", pAtlas, Vector2(500.f, 1900.f), Vector2(100.f, 100.f), 1, Vector2(100.f, 100.f));
		m_Animator->Create(L"Player_ChannelingSkill_Idle_Right", pAtlas, Vector2(200.f, 2000.f), Vector2(100.f, 100.f), 1, Vector2(100.f, 100.f));
		m_Animator->Create(L"Player_ChannelingSkill_Idle_Up", pAtlas, Vector2(200.f, 2100.f), Vector2(100.f, 100.f), 1, Vector2(100.f, 100.f));
		m_Animator->Create(L"Player_ChannelingSkill_Idle_Down", pAtlas, Vector2(500.f, 1800.f), Vector2(100.f, 100.f), 1, Vector2(100.f, 100.f));
		m_Animator->Create(L"Player_ChannelingSkill_Idle_LeftUp", pAtlas, Vector2(200.f, 2000.f), Vector2(100.f, 100.f), 1, Vector2(100.f, 100.f));
		m_Animator->Create(L"Player_ChannelingSkill_Idle_RightUp", pAtlas, Vector2(500.f, 2100.f), Vector2(100.f, 100.f), 1, Vector2(100.f, 100.f));
		m_Animator->Create(L"Player_ChannelingSkill_Idle_LeftDown", pAtlas, Vector2(200.f, 1800.f), Vector2(100.f, 100.f), 1, Vector2(100.f, 100.f));
		m_Animator->Create(L"Player_ChannelingSkill_Idle_RightDown", pAtlas, Vector2(200.f, 1900.f), Vector2(100.f, 100.f), 1, Vector2(100.f, 100.f));

		// Attack - ChannelingSkillWalk
		m_Animator->Create(L"Player_ChannelingSkill_Walk_Left", pAtlas, Vector2(300.f, 2500.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);
		m_Animator->Create(L"Player_ChannelingSkill_Walk_Right", pAtlas, Vector2(0.f, 2600.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);
		m_Animator->Create(L"Player_ChannelingSkill_Walk_Up", pAtlas, Vector2(0.f, 2700.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);
		m_Animator->Create(L"Player_ChannelingSkill_Walk_Down", pAtlas, Vector2(300.f, 2400.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);
		m_Animator->Create(L"Player_ChannelingSkill_Walk_LeftUp", pAtlas, Vector2(300.f, 2600.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);
		m_Animator->Create(L"Player_ChannelingSkill_Walk_RightUp", pAtlas, Vector2(300.f, 2700.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);
		m_Animator->Create(L"Player_ChannelingSkill_Walk_LeftDown", pAtlas, Vector2(0.f , 2400.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);
		m_Animator->Create(L"Player_ChannelingSkill_Walk_RightDown", pAtlas, Vector2(0.f, 2500.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.2f, false);



		// Dead
		m_Animator->Create(L"Player_Dead", pAtlas, Vector2(400.f, 2300.f), Vector2(100.f, 100.f), 2, Vector2(100.f, 100.f), Vector2::Zero, 2.f, false);
		m_Animator->CompleteEvent(L"Player_Dead") = std::bind(&PlayerScript::CompleteAction, this);


		// LookAround
		m_Animator->Create(L"Player_MagicCircleMove", pAtlas, Vector2(0.f, 4100.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->CompleteEvent(L"Player_MagicCircleMove") = std::bind(&PlayerScript::MagicCircleMove, this);

		m_Animator->Create(L"Player_LookAround", pAtlas, Vector2(0.f, 3700.f), Vector2(100.f, 100.f), 5, Vector2(100.f, 100.f), Vector2::Zero, 0.3f, false);
		m_Animator->CompleteEvent(L"Player_LookAround") = std::bind(&PlayerScript::LookAround, this);
	}

	StateScript* PlayerScript::GetStateScript(StateScript::eState _State)
	{
		std::map<StateScript::eState, StateScript*>::iterator iter = m_State.find(_State);

		if (iter == m_State.end())
			return nullptr;

		return iter->second;
	}

	void PlayerScript::AddState(StateScript* _State)
	{
		if (nullptr == _State)
			return;

		_State->SetPlayerScript(this);
		_State->Initialize();
		StateScript::eState eState = _State->GetStateType();
		m_State.insert(std::make_pair(eState, _State));
	}

	void PlayerScript::StateUpdate()
	{
		// 치트
		if (Input::GetKeyDown(eKeyCode::G)
			&& Input::GetKey(eKeyCode::LEFT_BRACKET)
			&& Input::GetKey(eKeyCode::RIGHT_BRACKET))
		{
			if (1 == m_Damage)
				m_Damage = 0;
			else
				m_Damage = 1;
		}

		if (Input::GetKey(eKeyCode::LEFT_BRACKET) && Input::GetKeyDown(eKeyCode::_1))
		{
			HeartScript* pHPScript = (HeartScript*)m_arrUI[(int)eUI::HP];
			if (pHPScript)
			{
				pHPScript->SetMaxHP(--m_PlayerInfo.MaxHP);

				if(m_PlayerInfo.MaxHP < m_PlayerInfo.CurHP)
					pHPScript->SetHeart(m_PlayerInfo.MaxHP);
				else
					pHPScript->SetHeart(m_PlayerInfo.CurHP);
			}
		}

		if (Input::GetKey(eKeyCode::LEFT_BRACKET) && Input::GetKeyDown(eKeyCode::_2))
		{
			InflictDamage(1);
		}

		if (Input::GetKey(eKeyCode::RIGHT_BRACKET) && Input::GetKeyDown(eKeyCode::_1))
		{
			HeartScript* pHPScript = (HeartScript*)m_arrUI[(int)eUI::HP];
			if (pHPScript)
			{
				pHPScript->SetMaxHP(++m_PlayerInfo.MaxHP);

				if (m_PlayerInfo.MaxHP < m_PlayerInfo.CurHP)
					pHPScript->SetHeart(m_PlayerInfo.MaxHP);
				else
					pHPScript->SetHeart(m_PlayerInfo.CurHP);
			}
		}

		if (Input::GetKey(eKeyCode::RIGHT_BRACKET) && Input::GetKeyDown(eKeyCode::_2))
		{
			InflictDamage(-1);
		}

		if (Input::GetKey(eKeyCode::LEFT_BRACKET) && Input::GetKeyDown(eKeyCode::_3))
		{
			GetMaxMana(-1);

			if (m_PlayerInfo.MaxMP < m_PlayerInfo.CurMP)
				UseMana(1);
		}

		if (Input::GetKey(eKeyCode::LEFT_BRACKET) && Input::GetKeyDown(eKeyCode::_4))
		{
			UseMana(1);
		}

		if (Input::GetKey(eKeyCode::RIGHT_BRACKET) && Input::GetKeyDown(eKeyCode::_3))
		{
			GetMaxMana(1);
		}

		if (Input::GetKey(eKeyCode::RIGHT_BRACKET) && Input::GetKeyDown(eKeyCode::_4))
		{
			UseMana(-1);
		}

		StateScript::eState eCurState = m_CurState->GetStateType();

		if (m_bAction || StateScript::eState::Dead == eCurState)
			return;

		// 이동 모드
		if (Input::GetKeyDown(eKeyCode::SHIFT))
		{
			if (eMoveType::Walk == m_MoveType)
			{
				if(!m_bSkillUse && !m_bChanneling)
					m_MoveType = eMoveType::Run;
			}
			else
				m_MoveType = eMoveType::Walk;
		}

		// 무기 교체
		if (Input::GetKeyDown(eKeyCode::R))
		{
			WeaponSlotScript* pWeaponSlot = (WeaponSlotScript*)m_arrUI[(int)eUI::WeaponSlot];
			if (pWeaponSlot)
			{
				pWeaponSlot->ChangeSlot();
			}
		}

		// 인벤토리
		if (Input::GetKeyDown(eKeyCode::I))
		{
			InventoryScript* pInven = (InventoryScript*)m_arrUI[(int)eUI::Inventory];
			if (pInven)
			{
				pInven->InventoryOnOff();
			}
		}

		// 대쉬
		if (Input::GetKey(eKeyCode::SPACE) && m_PlayerInfo.CurTP > 2.f)
		{
			m_PrevDir = m_Dir;
			if (Input::GetKey(eKeyCode::A)
				&& Input::GetKey(eKeyCode::D))
			{
				m_Dir = CalDirToMouse();
			}
			else if (Input::GetKey(eKeyCode::W)
				&& Input::GetKey(eKeyCode::S))
			{
				m_Dir = CalDirToMouse();
			}
			else if (Input::GetKey(eKeyCode::A)
				&& Input::GetKey(eKeyCode::W))
			{
				m_Dir = eDir::LeftUp;
			}
			else if (Input::GetKey(eKeyCode::A)
				&& Input::GetKey(eKeyCode::S))
			{
				m_Dir = eDir::LeftDown;
			}
			else if (Input::GetKey(eKeyCode::D)
				&& Input::GetKey(eKeyCode::S))
			{
				m_Dir = eDir::RightDown;
			}
			else if (Input::GetKey(eKeyCode::D)
				&& Input::GetKey(eKeyCode::W))
			{
				m_Dir = eDir::RightUp;
			}
			else if (Input::GetKey(eKeyCode::A))
			{
				m_Dir = eDir::Left;
			}
			else if (Input::GetKey(eKeyCode::D))
			{
				m_Dir = eDir::Right;
			}
			else if (Input::GetKey(eKeyCode::S))
			{
				m_Dir = eDir::Down;
			}
			else if (Input::GetKey(eKeyCode::W))
			{
				m_Dir = eDir::Up;
			}
			else
			{
				m_Dir = CalDirToMouse();
			}

			m_bInvincible = true;
			m_InvincibleTime = 0.6f;
			ChangeState(StateScript::eState::Dash);
		}

		// 공격
		else if (Input::GetKey(eKeyCode::LBUTTON)
			&& m_PlayerInfo.CurTP > 1.f
			&& !m_bCantHit
			&& !m_bSkillUse)
		{
			if (m_CurWeapon == eWeaponType::Wand)
			{
				m_bChanneling = true;
				ChannelingBarScript* pChanneling = (ChannelingBarScript*)m_arrUI[(int)eUI::Channeling];
				if (pChanneling)
				{
					pChanneling->SetChannelingType(ChannelingBarScript::eChannelingType::Charging);
					pChanneling->ChannelingOnOff(m_bChanneling);
				}

				m_ChargeGauge = pChanneling->GetChargeGauge();
				if(m_ChargeGauge < 1.f)
					UseStamina(0.0005f);
			}
			else
			{
				m_PrevDir = m_Dir;
				m_Dir = CalDirToMouse();
				ChangeState(StateScript::eState::Attack);
			}
		}
		else if (Input::GetKey(eKeyCode::RBUTTON)
			&& m_CurSkill && m_CurSkill->CurCoolTime == 0.f
			&& !m_bCantHit)
		{
			if (!m_bSkillUse && m_PlayerInfo.CurMP < m_CurSkill->NeedMana)
				return;

			switch (m_CurSkill->SkillType)
			{
			case eSkillType::IceBall:
			{
				ChannelingBarScript* pChanneling = (ChannelingBarScript*)m_arrUI[(int)eUI::Channeling];
				pChanneling->SetChannelingType(ChannelingBarScript::eChannelingType::Consuming);

				if (!m_bFirst)
				{
					UseMana(m_CurSkill->NeedMana);
					m_bSkillUse = true;
					m_bChanneling = true;
					m_bFirst = true;
					m_MoveType = eMoveType::Walk;
					Skill_IceBall();
					pChanneling->SetMaxTime(1.5f);
				}

				if (pChanneling->IsComplete())
				{
					m_bSkillUse = false;
					m_bChanneling = false;
					m_bFirst = false;
					m_CurSkill->CurCoolTime = m_CurSkill->SkillCoolTime;
					m_MoveType = eMoveType::Run;
				}
				else
				{
					m_SkillProjectileCoolTime += (float)Time::DeltaTime();

					if (m_SkillProjectileCoolTime >= 0.2f)
					{
						Skill_IceBall();
						m_SkillProjectileCoolTime = 0.f;
					}
				}

				pChanneling->ChannelingOnOff(m_bChanneling);

				m_PrevDir = m_Dir;
				m_Dir = CalDirToMouse();
			}
				break;
			case eSkillType::FireStrike:
			{
				UseMana(m_CurSkill->NeedMana);
				m_bAction = true;
				m_bSkillUse = true;
				m_PrevDir = m_Dir;
				m_Dir = CalDirToMouse();
				ChangeState(StateScript::eState::Idle);
			}
				break;
			default:
				break;
			}
		}

		if (m_bChanneling)
		{
			if(Input::GetKeyUp(eKeyCode::LBUTTON))
			{
				ChannelingBarScript* pChanneling = (ChannelingBarScript*)m_arrUI[(int)eUI::Channeling];
				m_ChargeGauge = pChanneling->GetChargeGauge();

				m_bChanneling = false;
				pChanneling->ChannelingOnOff(false);

				if (m_ChargeGauge > 0.167f)
					m_bChargeAnim = true;

				m_PrevDir = m_Dir;
				m_Dir = CalDirToMouse();
				ChangeState(StateScript::eState::Attack);
			}
			else if (Input::GetKeyUp(eKeyCode::RBUTTON))
			{
				// 채널링 스킬 종료
				if (m_CurSkill && m_CurSkill->SkillType == eSkillType::IceBall)
				{
					m_bSkillUse = false;
					m_bChanneling = false;
					m_bFirst = false;
					m_CurSkill->CurCoolTime = m_CurSkill->SkillCoolTime;
					m_MoveType = eMoveType::Run;
					ChannelingBarScript* pChanneling = (ChannelingBarScript*)m_arrUI[(int)eUI::Channeling];
					pChanneling->ChannelingOnOff(m_bChanneling);
				}
			}
		}

		// 퀵슬롯 아이템 사용
		if (Input::GetKeyDown(eKeyCode::C))
		{
			QuickItemScript* pQuickSlot = (QuickItemScript*)m_arrUI[(int)eUI::QuickItem];
			if (pQuickSlot)
				pQuickSlot->UseQuickSlotItem();
		}

		// 스킬
		if (Input::GetKeyDown(eKeyCode::_1))
		{
			LayoutScript* pLayout = (LayoutScript*)m_arrUI[(int)eUI::Layout];
			if (pLayout)
				pLayout->SelectSlot(1);
		}
		else if (Input::GetKeyDown(eKeyCode::_2))
		{
			LayoutScript* pLayout = (LayoutScript*)m_arrUI[(int)eUI::Layout];
			if (pLayout)
				pLayout->SelectSlot(2);
		}
		else if (Input::GetKeyDown(eKeyCode::_3))
		{
			LayoutScript* pLayout = (LayoutScript*)m_arrUI[(int)eUI::Layout];
			if (pLayout)
				pLayout->SelectSlot(3);
		}
		else if (Input::GetKeyDown(eKeyCode::_4))
		{
			LayoutScript* pLayout = (LayoutScript*)m_arrUI[(int)eUI::Layout];
			if (pLayout)
				pLayout->SelectSlot(4);
		}


		if (!m_bAction)
		{
			// 이동
			if (Input::GetKey(eKeyCode::A)
				&& Input::GetKey(eKeyCode::D))
			{
				ChangeState(StateScript::eState::Idle);
			}
			else if (Input::GetKey(eKeyCode::W)
				&& Input::GetKey(eKeyCode::S))
			{
				ChangeState(StateScript::eState::Idle);
			}
			else if (Input::GetKey(eKeyCode::A)
				&& Input::GetKey(eKeyCode::W))
			{
				m_PrevDir = m_Dir;
				m_Dir = eDir::LeftUp;
				ChangeState(StateScript::eState::Move);
			}
			else if (Input::GetKey(eKeyCode::A)
				&& Input::GetKey(eKeyCode::S))
			{
				m_PrevDir = m_Dir;
				m_Dir = eDir::LeftDown;
				ChangeState(StateScript::eState::Move);
			}
			else if (Input::GetKey(eKeyCode::D)
				&& Input::GetKey(eKeyCode::S))
			{
				m_PrevDir = m_Dir;
				m_Dir = eDir::RightDown;
				ChangeState(StateScript::eState::Move);
			}
			else if (Input::GetKey(eKeyCode::D)
				&& Input::GetKey(eKeyCode::W))
			{
				m_PrevDir = m_Dir;
				m_Dir = eDir::RightUp;
				ChangeState(StateScript::eState::Move);
			}
			else if (Input::GetKey(eKeyCode::A))
			{
				m_PrevDir = m_Dir;
				m_Dir = eDir::Left;
				ChangeState(StateScript::eState::Move);
			}
			else if (Input::GetKey(eKeyCode::D))
			{
				m_PrevDir = m_Dir;
				m_Dir = eDir::Right;
				ChangeState(StateScript::eState::Move);
			}
			else if (Input::GetKey(eKeyCode::S))
			{
				m_PrevDir = m_Dir;
				m_Dir = eDir::Down;
				ChangeState(StateScript::eState::Move);
			}
			else if (Input::GetKey(eKeyCode::W))
			{
				m_PrevDir = m_Dir;
				m_Dir = eDir::Up;
				ChangeState(StateScript::eState::Move);
			}
			else
			{
				if (m_bAction || StateScript::eState::Dead == eCurState)
					return;

				ChangeState(StateScript::eState::Idle);
			}
		}
	}

	void PlayerScript::AnimationUpdate()
	{
		StateScript::eState eCurState = m_CurState->GetStateType();

		if (m_PrevState == eCurState
			&& m_PrevDir == m_Dir
			&& m_bChanneling
			&& !m_bSkillUse)
			return;

		if (m_bSkillUse && m_bAction
			&& m_CurSkill->SkillType == eSkillType::FireStrike
			&& eCurState != StateScript::eState::Dead)
		{
			switch (m_Dir)
			{
			case eDir::Left:
				m_Animator->PlayAnimation(L"Player_SkillCast_Left", true);
				break;
			case eDir::Right:
				m_Animator->PlayAnimation(L"Player_SkillCast_Right", true);
				break;
			case eDir::Up:
				m_Animator->PlayAnimation(L"Player_SkillCast_Up", true);
				break;
			case eDir::Down:
				m_Animator->PlayAnimation(L"Player_SkillCast_Down", true);
				break;
			case eDir::LeftUp:
				m_Animator->PlayAnimation(L"Player_SkillCast_LeftUp", true);
				break;
			case eDir::RightUp:
				m_Animator->PlayAnimation(L"Player_SkillCast_RightUp", true);
				break;
			case eDir::LeftDown:
				m_Animator->PlayAnimation(L"Player_SkillCast_LeftDown", true);
				break;
			case eDir::RightDown:
				m_Animator->PlayAnimation(L"Player_SkillCast_RightDown", true);
				break;
			}
		}
		else
		{
			switch (eCurState)
			{
			case StateScript::eState::Idle:
			{
				if (m_bChanneling && m_bSkillUse && !m_bDontAnimChange)
				{
					switch (m_Dir)
					{
					case eDir::Left:
						m_Animator->PlayAnimation(L"Player_ChannelingSkill_Idle_Left", true);
						break;
					case eDir::Right:
						m_Animator->PlayAnimation(L"Player_ChannelingSkill_Idle_Right", true);
						break;
					case eDir::Up:
						m_Animator->PlayAnimation(L"Player_ChannelingSkill_Idle_Up", true);
						break;
					case eDir::Down:
						m_Animator->PlayAnimation(L"Player_ChannelingSkill_Idle_Down", true);
						break;
					case eDir::LeftUp:
						m_Animator->PlayAnimation(L"Player_ChannelingSkill_Idle_LeftUp", true);
						break;
					case eDir::RightUp:
						m_Animator->PlayAnimation(L"Player_ChannelingSkill_Idle_RightUp", true);
						break;
					case eDir::LeftDown:
						m_Animator->PlayAnimation(L"Player_ChannelingSkill_Idle_LeftDown", true);
						break;
					case eDir::RightDown:
						m_Animator->PlayAnimation(L"Player_ChannelingSkill_Idle_RightDown", true);
						break;
					}
				}
				else if (!m_bDontAnimChange)
				{
					switch (m_Dir)
					{
					case eDir::Left:
						m_Animator->PlayAnimation(L"Player_Idle_Left", true);
						break;
					case eDir::Right:
						m_Animator->PlayAnimation(L"Player_Idle_Right", true);
						break;
					case eDir::Up:
						m_Animator->PlayAnimation(L"Player_Idle_Up", true);
						break;
					case eDir::Down:
						m_Animator->PlayAnimation(L"Player_Idle_Down", true);
						break;
					case eDir::LeftUp:
						m_Animator->PlayAnimation(L"Player_Idle_LeftUp", true);
						break;
					case eDir::RightUp:
						m_Animator->PlayAnimation(L"Player_Idle_RightUp", true);
						break;
					case eDir::LeftDown:
						m_Animator->PlayAnimation(L"Player_Idle_LeftDown", true);
						break;
					case eDir::RightDown:
						m_Animator->PlayAnimation(L"Player_Idle_RightDown", true);
						break;
					}
				}
			}
			break;
			case StateScript::eState::Move:
			{
				if (eMoveType::Walk == m_MoveType)
				{
					if (m_bChanneling && m_bSkillUse)
					{
						switch (m_Dir)
						{
						case eDir::Left:
							m_Animator->PlayAnimation(L"Player_ChannelingSkill_Walk_Left", true);
							break;
						case eDir::Right:
							m_Animator->PlayAnimation(L"Player_ChannelingSkill_Walk_Right", true);
							break;
						case eDir::Up:
							m_Animator->PlayAnimation(L"Player_ChannelingSkill_Walk_Up", true);
							break;
						case eDir::Down:
							m_Animator->PlayAnimation(L"Player_ChannelingSkill_Walk_Down", true);
							break;
						case eDir::LeftUp:
							m_Animator->PlayAnimation(L"Player_ChannelingSkill_Walk_LeftUp", true);
							break;
						case eDir::RightUp:
							m_Animator->PlayAnimation(L"Player_ChannelingSkill_Walk_RightUp", true);
							break;
						case eDir::LeftDown:
							m_Animator->PlayAnimation(L"Player_ChannelingSkill_Walk_LeftDown", true);
							break;
						case eDir::RightDown:
							m_Animator->PlayAnimation(L"Player_ChannelingSkill_Walk_RightDown", true);
							break;
						}
					}
					else
					{
						switch (m_Dir)
						{
						case eDir::Left:
							m_Animator->PlayAnimation(L"Player_Walk_Left", true);
							break;
						case eDir::Right:
							m_Animator->PlayAnimation(L"Player_Walk_Right", true);
							break;
						case eDir::Up:
							m_Animator->PlayAnimation(L"Player_Walk_Up", true);
							break;
						case eDir::Down:
							m_Animator->PlayAnimation(L"Player_Walk_Down", true);
							break;
						case eDir::LeftUp:
							m_Animator->PlayAnimation(L"Player_Walk_LeftUp", true);
							break;
						case eDir::RightUp:
							m_Animator->PlayAnimation(L"Player_Walk_RightUp", true);
							break;
						case eDir::LeftDown:
							m_Animator->PlayAnimation(L"Player_Walk_LeftDown", true);
							break;
						case eDir::RightDown:
							m_Animator->PlayAnimation(L"Player_Walk_RightDown", true);
							break;
						}
					}
				}
				else
				{
					switch (m_Dir)
					{
					case eDir::Left:
						m_Animator->PlayAnimation(L"Player_Run_Left", true);
						break;
					case eDir::Right:
						m_Animator->PlayAnimation(L"Player_Run_Right", true);
						break;
					case eDir::Up:
						m_Animator->PlayAnimation(L"Player_Run_Up", true);
						break;
					case eDir::Down:
						m_Animator->PlayAnimation(L"Player_Run_Down", true);
						break;
					case eDir::LeftUp:
						m_Animator->PlayAnimation(L"Player_Run_LeftUp", true);
						break;
					case eDir::RightUp:
						m_Animator->PlayAnimation(L"Player_Run_RightUp", true);
						break;
					case eDir::LeftDown:
						m_Animator->PlayAnimation(L"Player_Run_LeftDown", true);
						break;
					case eDir::RightDown:
						m_Animator->PlayAnimation(L"Player_Run_RightDown", true);
						break;
					}
				}
			}
			break;
			case StateScript::eState::Dash:
			{
				switch (m_Dir)
				{
				case eDir::Left:
					m_Animator->PlayAnimation(L"Player_Dash_Left", true);
					break;
				case eDir::Right:
					m_Animator->PlayAnimation(L"Player_Dash_Right", true);
					break;
				case eDir::Up:
					m_Animator->PlayAnimation(L"Player_Dash_Up", true);
					break;
				case eDir::Down:
					m_Animator->PlayAnimation(L"Player_Dash_Down", true);
					break;
				case eDir::LeftUp:
					m_Animator->PlayAnimation(L"Player_Dash_LeftUp", true);
					break;
				case eDir::RightUp:
					m_Animator->PlayAnimation(L"Player_Dash_RightUp", true);
					break;
				case eDir::LeftDown:
					m_Animator->PlayAnimation(L"Player_Dash_LeftDown", true);
					break;
				case eDir::RightDown:
					m_Animator->PlayAnimation(L"Player_Dash_RightDown", true);
					break;
				}
			}
			break;
			case StateScript::eState::Attack:
			{
				switch (m_CurWeapon)
				{
				case eWeaponType::None:
				{
					switch (m_Dir)
					{
					case eDir::Left:
						m_Animator->PlayAnimation(L"Player_Wand_Left", true);
						break;
					case eDir::Right:
						m_Animator->PlayAnimation(L"Player_Wand_Right", true);
						break;
					case eDir::Up:
						m_Animator->PlayAnimation(L"Player_Wand_Up", true);
						break;
					case eDir::Down:
						m_Animator->PlayAnimation(L"Player_Wand_Down", true);
						break;
					case eDir::LeftUp:
						m_Animator->PlayAnimation(L"Player_Wand_LeftUp", true);
						break;
					case eDir::RightUp:
						m_Animator->PlayAnimation(L"Player_Wand_RightUp", true);
						break;
					case eDir::LeftDown:
						m_Animator->PlayAnimation(L"Player_Wand_LeftDown", true);
						break;
					case eDir::RightDown:
						m_Animator->PlayAnimation(L"Player_Wand_RightDown", true);
						break;
					}
				}
				break;
				case eWeaponType::Wand:
				{
					if (m_bChargeAnim)
					{
						switch (m_Dir)
						{
						case eDir::Left:
							m_Animator->PlayAnimation(L"Player_WandChanneling_Left", true);
							break;
						case eDir::Right:
							m_Animator->PlayAnimation(L"Player_WandChanneling_Right", true);
							break;
						case eDir::Up:
							m_Animator->PlayAnimation(L"Player_WandChanneling_Up", true);
							break;
						case eDir::Down:
							m_Animator->PlayAnimation(L"Player_WandChanneling_Down", true);
							break;
						case eDir::LeftUp:
							m_Animator->PlayAnimation(L"Player_WandChanneling_LeftUp", true);
							break;
						case eDir::RightUp:
							m_Animator->PlayAnimation(L"Player_WandChanneling_RightUp", true);
							break;
						case eDir::LeftDown:
							m_Animator->PlayAnimation(L"Player_WandChanneling_LeftDown", true);
							break;
						case eDir::RightDown:
							m_Animator->PlayAnimation(L"Player_WandChanneling_RightDown", true);
							break;
						}
					}
					else
					{
						switch (m_Dir)
						{
						case eDir::Left:
							m_Animator->PlayAnimation(L"Player_Wand_Left", true);
							break;
						case eDir::Right:
							m_Animator->PlayAnimation(L"Player_Wand_Right", true);
							break;
						case eDir::Up:
							m_Animator->PlayAnimation(L"Player_Wand_Up", true);
							break;
						case eDir::Down:
							m_Animator->PlayAnimation(L"Player_Wand_Down", true);
							break;
						case eDir::LeftUp:
							m_Animator->PlayAnimation(L"Player_Wand_LeftUp", true);
							break;
						case eDir::RightUp:
							m_Animator->PlayAnimation(L"Player_Wand_RightUp", true);
							break;
						case eDir::LeftDown:
							m_Animator->PlayAnimation(L"Player_Wand_LeftDown", true);
							break;
						case eDir::RightDown:
							m_Animator->PlayAnimation(L"Player_Wand_RightDown", true);
							break;
						}
					}
				}
				break;
				case eWeaponType::Bow:
				{
					switch (m_Dir)
					{
					case eDir::Left:
						m_Animator->PlayAnimation(L"Player_Bow_Left", true);
						break;
					case eDir::Right:
						m_Animator->PlayAnimation(L"Player_Bow_Right", true);
						break;
					case eDir::Up:
						m_Animator->PlayAnimation(L"Player_Bow_Up", true);
						break;
					case eDir::Down:
						m_Animator->PlayAnimation(L"Player_Bow_Down", true);
						break;
					case eDir::LeftUp:
						m_Animator->PlayAnimation(L"Player_Bow_LeftUp", true);
						break;
					case eDir::RightUp:
						m_Animator->PlayAnimation(L"Player_Bow_RightUp", true);
						break;
					case eDir::LeftDown:
						m_Animator->PlayAnimation(L"Player_Bow_LeftDown", true);
						break;
					case eDir::RightDown:
						m_Animator->PlayAnimation(L"Player_Bow_RightDown", true);
						break;
					}
				}
				break;
				case eWeaponType::Sword:
					break;
				default:
					break;
				}
			}
			break;
			case StateScript::eState::Dead:
				m_Animator->PlayAnimation(L"Player_Dead", true);
				break;
			}
		}
	}

	eDir PlayerScript::CalDirToMouse()
	{
		Vector2 vScreenPos = Lu::Input::GetMousePos();
		Vector3 vMouseWorldPos = renderer::mainCamera->ScreenToWorld(vScreenPos);
		Vector3 vPlayerPos = GetOwner()->GetComponent<Transform>()->GetPosition();

		Vector3 vDir = vMouseWorldPos - vPlayerPos;
		vDir.Normalize();

		float angle = atan2(vDir.y, vDir.x) * (180.0f / math::PI);

		if (angle > -22.5f && angle <= 22.5f)
			return eDir::Right;
		else if (angle > 22.5f && angle <= 67.5f)
			return eDir::RightUp;
		else if (angle > 67.5f && angle <= 112.5f)
			return eDir::Up;
		else if (angle > 112.5f && angle <= 157.5f)
			return eDir::LeftUp;
		else if (angle > 157.5f || angle <= -157.5f)
			return eDir::Left;
		else if (angle > -157.5f && angle <= -112.5f)
			return eDir::LeftDown;
		else if (angle > -112.5f && angle <= -67.5f)
			return eDir::Down;
		else if (angle > -67.5f && angle <= -22.5f)
			return eDir::RightDown;

		return eDir::None;
	}

	void PlayerScript::CompleteAction()
	{
		ChangeState(StateScript::eState::Idle);

		if (m_bChargeAnim)
			m_bChargeAnim = false;

		if (m_bSkillUse)
		{
			m_bAction = false;
			m_bSkillUse = false;
			m_CurSkill->CurCoolTime = m_CurSkill->SkillCoolTime;
		}
	}

	void PlayerScript::DashFinish()
	{
		ChangeState(StateScript::eState::Idle);
		m_bDashSuccess = false;
	}

	void PlayerScript::AttackSFX()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"AttackSFX", L"..\\Resources\\Sound\\SFX\\Player\\AttackSFX.ogg"));
		pSFX->Play();
	}

	void PlayerScript::MagicCircleMove()
	{
		m_Animator->PlayAnimation(L"Player_LookAround", true);
	}

	void PlayerScript::LookAround()
	{
		m_Animator->PlayAnimation(L"Player_Idle_Down", true);
		m_bAction = false;
		m_bDontAnimChange = false;
	}

	void PlayerScript::AppearFXFinished()
	{
		if (m_AppearFX)
		{
			object::Destroy(m_AppearFX);
			m_AppearFX = nullptr;
		}
	}

	void PlayerScript::StaminaRecovery()
	{
		StateScript::eState eCurState = m_CurState->GetStateType();

		if (m_PrevState == StateScript::eState::Attack
			|| m_PrevState == StateScript::eState::Dash
			|| eCurState == StateScript::eState::Attack
			|| eCurState == StateScript::eState::Dash
			|| m_ChargeGauge > 0.f)
			return;

		float RecoveryValue = m_PlayerInfo.TPRecoveryRate * (float)Time::DeltaTime();
		m_PlayerInfo.CurTP += RecoveryValue;

		if (m_PlayerInfo.CurTP > m_PlayerInfo.MaxTP)
			m_PlayerInfo.CurTP = m_PlayerInfo.MaxTP;

		ProgressBarScript* pTPScript = (ProgressBarScript*)m_arrUI[(int)eUI::TP];
		if (pTPScript)
		{
			pTPScript->SetCurValue(m_PlayerInfo.CurTP);
		}
	}

	void PlayerScript::Skill_IceBall()
	{
		Vector3 vPos = GetOwner()->GetComponent<Transform>()->GetPosition();
		Vector2 vScreenPos = Lu::Input::GetMousePos();
		Vector3 vMouseWorldPos = renderer::mainCamera->ScreenToWorld(vScreenPos);

		Vector3 vActionDir = vMouseWorldPos - vPos;
		vActionDir.Normalize();
		float angle = std::atan2(vActionDir.y, vActionDir.x);

		GameObject* pProjectile = object::Instantiate<GameObject>(vPos, Vector3(90.f, 49.5f, 100.f), Vector3(0.f, 0.f, angle + (PI * 2.f)), eLayerType::PlayerProjectile);
		pProjectile->SetName(L"IceBallProjectile");

		MeshRenderer* pMeshRender = pProjectile->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		pMeshRender->SetMaterial(Resources::Find<Material>(L"IceBall_Mtrl"));

		Collider2D* pCollider = pProjectile->AddComponent<Collider2D>();
		pCollider->SetSize(Vector2(0.5f, 0.5f));

		Animator* pAnimator = pProjectile->AddComponent<Animator>();
		pAnimator->Create(L"IceBall", Resources::Load<Texture>(L"IceBall_Tex", L"..\\Resources\\Texture\\Player\\IceBall.png")
			, Vector2(0.f, 0.f), Vector2(60.f, 33.f), 10, Vector2(60.f, 33.f), Vector2::Zero, 0.1f);
		pAnimator->PlayAnimation(L"IceBall", true);

		IceBallScript* pIceBallScript = pProjectile->AddComponent<IceBallScript>();
		pIceBallScript->SetTransform(pProjectile->GetComponent<Transform>());
		pIceBallScript->SetPlayerScript(this);
		pIceBallScript->SetDir(vActionDir);
		pIceBallScript->SetSpeed(300.f);
		pIceBallScript->SetDuration(1.f);

		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"IceBall1SFX", L"..\\Resources\\Sound\\SFX\\Player\\IceBall1SFX.ogg"));
		pSFX->Play();
	}

	void PlayerScript::Skill_Cast()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"SkillCastSFX", L"..\\Resources\\Sound\\SFX\\Player\\SkillCastSFX.ogg"));
		pSFX->Play();

		Vector3 vPos = GetOwner()->GetComponent<Transform>()->GetPosition();
		GameObject* pObject = object::Instantiate<GameObject>(Vector3(vPos.x, vPos.y + 60.f, vPos.z), Vector3(288.f, 288.f, 100.f), eLayerType::FX);
		pObject->SetName(L"SkillCastFX");
		MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		pMeshRender->SetMaterial(Resources::Find<Material>(L"SkillCast_Mtrl"));
		pObject->AddComponent<Animator>();

		switch (m_Dir)
		{
		case Lu::eDir::None:
			break;
		case Lu::eDir::Left:
			vPos.x -= 300.f;
			break;
		case Lu::eDir::Right:
			vPos.x += 300.f;
			break;
		case Lu::eDir::Up:
			vPos.y += 300.f;
			break;
		case Lu::eDir::Down:
			vPos.y -= 300.f;
			break;
		case Lu::eDir::LeftUp:
		{
			float diagonalFactor = 1.0f / sqrt(2.0f);
			vPos.x -= 300.f * diagonalFactor;
			vPos.y += 300.f * diagonalFactor;
		}
			break;
		case Lu::eDir::RightUp:
		{
			float diagonalFactor = 1.0f / sqrt(2.0f);
			vPos.x += 300.f * diagonalFactor;
			vPos.y += 300.f * diagonalFactor;
		}
			break;
		case Lu::eDir::LeftDown:
		{
			float diagonalFactor = 1.0f / sqrt(2.0f);
			vPos.x -= 300.f * diagonalFactor;
			vPos.y -= 300.f * diagonalFactor;
		}
			break;
		case Lu::eDir::RightDown:
		{
			float diagonalFactor = 1.0f / sqrt(2.0f);
			vPos.x += 300.f * diagonalFactor;
			vPos.y -= 300.f * diagonalFactor;
		}
			break;
		default:
			break;
		}

		pObject->AddComponent<SkillCastScript>()->SetSpawnPos(vPos);
	}

	void PlayerScript::LearnSkill(tSkill* _Skill)
	{
		if (!_Skill)
			return;

		LayoutScript* pLayout = (LayoutScript*)m_arrUI[(int)eUI::Layout];
		if (pLayout)
			pLayout->LearnSkill(_Skill);

		if (!m_CurSkill)
		{
			pLayout->SelectSlot(1);
		}
	}

	void PlayerScript::ResetPlayerInfo()
	{
		m_CurState = GetStateScript(StateScript::eState::Idle);
		m_CurWeapon = eWeaponType::None;
		m_PrevState = StateScript::eState::End;
		m_Dir = eDir::Down;
		m_PrevDir = eDir::None;
		m_CurSkill = nullptr;
		m_bAction = false;
		m_bInvincible = false;
		m_bDashSuccess = false;
		m_bHitEffect = false;
		m_bDontAnimChange = false;
		m_bCantHit = false;
		m_bChanneling = false;
		m_bChargeAnim = false;
		m_ChargeGauge = 0.f;
		m_InvincibleTime = 0.f;
		m_Damage = 1;
		m_bSkillUse = false;
		m_bFirst = false;
		m_SkillProjectileCoolTime = 0.f;


		HeartScript* pHPScript = (HeartScript*)m_arrUI[(int)eUI::HP];
		//MaxHP(6)
		m_PlayerInfo.MaxHP = 6;
		pHPScript->SetMaxHP(m_PlayerInfo.MaxHP);
		//CurHP(6)
		m_PlayerInfo.CurHP = 6;
		pHPScript->SetHeart(m_PlayerInfo.CurHP);


		ManaScript* pMPScript = (ManaScript*)m_arrUI[(int)eUI::MP];
		//MaxMP(3)
		m_PlayerInfo.MaxMP = 3;
		pMPScript->SetMaxMP(m_PlayerInfo.MaxMP);
		//CurMP(3)
		m_PlayerInfo.CurMP = 3;
		pMPScript->SetMana(m_PlayerInfo.CurMP);


		ProgressBarScript* pTPScript = (ProgressBarScript*)m_arrUI[(int)eUI::TP];
		//MaxTP(10.f)
		m_PlayerInfo.MaxTP = 10.f;
		pTPScript->SetMaxValue((int)m_PlayerInfo.MaxTP);
		//CurTP(10.f)
		m_PlayerInfo.CurTP = 10.f;
		pTPScript->SetCurValue((int)m_PlayerInfo.CurTP);

	
		ProgressBarScript* pEXPScript = (ProgressBarScript*)m_arrUI[(int)eUI::EXP];
		//MaxEXP(100)
		m_PlayerInfo.MaxEXP = 100;
		pEXPScript->SetMaxValue(m_PlayerInfo.MaxEXP);
		//CurEXP(0)
		m_PlayerInfo.CurEXP = 0;
		pEXPScript->SetCurValue(m_PlayerInfo.CurEXP);


		//Attack(10)
		m_PlayerInfo.Attack = 10;
		//Magic(10)
		m_PlayerInfo.Magic = 10;
		

		NumberScript* pNumScript = (NumberScript*)m_arrUI[(int)eUI::Level];
		//MaxLevel(9)
		//CurLevel(1)
		m_PlayerInfo.CurLevel = 1;
		pNumScript->SetCurNumber(m_PlayerInfo.CurLevel);


		//TPRecoveryRate(10.f)
		

		pNumScript = (NumberScript*)m_arrUI[(int)eUI::Gold];
		//MaxGold(9)
		m_PlayerInfo.MaxGold = 9;
		//CurGold(0)
		m_PlayerInfo.CurGold = 0;
		pNumScript->SetCurNumber(m_PlayerInfo.CurGold);


		LayoutScript* pLayout = (LayoutScript*)m_arrUI[(int)eUI::Layout];
		pLayout->ResetLayout();


		QuickItemScript* pQuickSlot = (QuickItemScript*)m_arrUI[(int)eUI::QuickItem];
		pQuickSlot->ResetQuickSlotItem();

		WeaponSlotScript* pWeaponSlot = (WeaponSlotScript*)m_arrUI[(int)eUI::WeaponSlot];
		pWeaponSlot->ResetWeaponSlot();


		InventoryScript* pInven = (InventoryScript*)m_arrUI[(int)eUI::Inventory];
		pInven->ResetInventory();
		
		
		SkillScript* pSkillUI = (SkillScript*)m_arrUI[(int)eUI::Skill];
		pSkillUI->ResetSkillUI();
	}

	void PlayerScript::Stop(bool _b)
	{
		if (_b)
		{
			m_bAction = _b;
			m_bCantHit = _b;

			ChangeState(StateScript::eState::Idle);
		}
		else
		{
			m_bAction = _b;
			m_bCantHit = _b;
		}
	}

	void PlayerScript::ChangeState(StateScript::eState _NextState)
	{
		StateScript* pNextState = GetStateScript(_NextState);

		if (pNextState->GetStateType() == m_CurState->GetStateType())
			return;

		m_CurState->Exit();
		m_PrevState = m_CurState->GetStateType();
		m_CurState = pNextState;
		m_CurState->Enter();
	}

	void PlayerScript::MagicPortalMove()
	{
		m_bDontAnimChange = true;
		m_bAction = true;
		m_Dir = eDir::Down;
		ChangeState(StateScript::eState::Idle);
		m_Animator->PlayAnimation(L"Player_MagicCircleMove", true);

		m_AppearFX = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 400.f), Vector3(120.f, 120.f, 100.f), eLayerType::FX);
		m_AppearFX->SetName(L"AppearFX");

		MeshRenderer* pMeshRender = m_AppearFX->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		pMeshRender->SetMaterial(Resources::Find<Material>(L"AppearFX_Mtrl"));

		Animator* pAnimator = m_AppearFX->AddComponent<Animator>();
		pAnimator->Create(L"AppearFX", Resources::Load<Texture>(L"AppearFX_Tex", L"..\\Resources\\Texture\\Player\\AppearFX.png")
			, Vector2::Zero, Vector2(192.f, 192.f), 15, Vector2(192.f, 192.f));
		pAnimator->CompleteEvent(L"AppearFX") = std::bind(&PlayerScript::AppearFXFinished, this);
		pAnimator->PlayAnimation(L"AppearFX", true);
	}

	void PlayerScript::IdleStateEvent()
	{
		m_bDontAnimChange = true;
		m_Dir = eDir::Down;
		m_Animator->PlayAnimation(L"Player_LookAround", true);
	}

	void PlayerScript::InflictDamage(int _Damage)
	{
		m_PlayerInfo.CurHP -= _Damage;
		HeartScript* pHPScript = (HeartScript*)m_arrUI[(int)eUI::HP];
		if (pHPScript)
			pHPScript->SetHeart(m_PlayerInfo.CurHP);
	}

	void PlayerScript::UseMana(int _Value)
	{
		if (m_PlayerInfo.CurMP - _Value > m_PlayerInfo.MaxMP)
			return;

		m_PlayerInfo.CurMP -= _Value;
		ManaScript* pMPScript = (ManaScript*)m_arrUI[(int)eUI::MP];
		if (pMPScript)
			pMPScript->SetMana(m_PlayerInfo.CurMP);
	}

	void PlayerScript::GetMaxMana(int _Value)
	{
		m_PlayerInfo.MaxMP += _Value;

		if (m_PlayerInfo.MaxMP > 5)
			m_PlayerInfo.MaxMP = 5;

		ManaScript* pMPScript = (ManaScript*)m_arrUI[(int)eUI::MP];
		if (pMPScript)
			pMPScript->SetMaxMP(m_PlayerInfo.MaxMP);
	}

	void PlayerScript::UseStamina(float _Value)
	{
		m_PlayerInfo.CurTP -= _Value;
		ProgressBarScript* pTPScript = (ProgressBarScript*)m_arrUI[(int)eUI::TP];
		if (pTPScript)
		{
			pTPScript->SetCurValue(m_PlayerInfo.CurTP);
		}
	}

	void PlayerScript::GetEXP(int _Value)
	{
		m_PlayerInfo.CurEXP += _Value;

		if (m_PlayerInfo.CurEXP >= 100)
		{
			NumberScript* pNumScript = (NumberScript*)m_arrUI[(int)eUI::Level];
			if (pNumScript)
			{
				// 레벨업
				pNumScript->SetCurNumber(++m_PlayerInfo.CurLevel);

				// HP, MP 회복
				int iRecoveryValue = m_PlayerInfo.MaxHP - m_PlayerInfo.CurHP;
				InflictDamage(-iRecoveryValue);
				iRecoveryValue = m_PlayerInfo.MaxMP - m_PlayerInfo.CurMP;
				UseMana(-iRecoveryValue);

				// 레벨업 SFX
				AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
				pSFX->SetClip(Resources::Load<AudioClip>(L"LevelUpSFX", L"..\\Resources\\Sound\\SFX\\Player\\LevelUpSFX.ogg"));
				pSFX->Play();

				// 스킬 획득
				SkillScript* pSkillUI = (SkillScript*)m_arrUI[(int)eUI::Skill];
				if(pSkillUI)
					pSkillUI->SkillUIOn();
			}

			m_PlayerInfo.CurEXP -= 100;
		}

		ProgressBarScript* pEXPScript = (ProgressBarScript*)m_arrUI[(int)eUI::EXP];
		if (pEXPScript)
		{
			pEXPScript->SetCurValue(m_PlayerInfo.CurEXP);
		}
	}

	void PlayerScript::GetCoin(int _Value)
	{
		m_PlayerInfo.CurGold += _Value;
		NumberScript* pNumScript = (NumberScript*)m_arrUI[(int)eUI::Gold];
		if (pNumScript)
		{
			pNumScript->SetCurNumber(m_PlayerInfo.CurGold);
		}
	}
}