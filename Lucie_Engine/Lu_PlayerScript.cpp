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
		, m_bAction(false)
		, m_bInvincible(false)
		, m_bHitEffect(false)
		, m_bDontAnimChange(false)
		, m_InvincibleTime(0.f)
		, m_Damage(1)
		, m_Animator(nullptr)
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

		m_CurWeapon = eWeaponType::Wand;
	}

	void PlayerScript::Update()
	{
		if (GetOwner()->IsDead())
			return;

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

		StateUpdate();
 		m_CurState->Update();
		AnimationUpdate();

		m_PrevState = m_CurState->GetStateType();
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
				m_PlayerInfo.HP -= m_Damage;

				if (m_PlayerInfo.HP <= 0)
				{
					m_PlayerInfo.HP = 0;
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
		}
	}

	void PlayerScript::OnCollisionStay(Collider2D* _Other)
	{
		if ((int)eLayerType::MonsterProjectile == _Other->GetOwner()->GetLayerIndex())
		{
			// Hit & Dead
			if (!m_bInvincible)
			{
				m_PlayerInfo.HP -= 1;

				if (m_PlayerInfo.HP <= 0)
				{
					m_PlayerInfo.HP = 0;
					m_bInvincible = false;
					m_bHitEffect = false;
					m_InvincibleTime = 0.f;
					ChangeState(StateScript::eState::Dead);
				}
				else
				{
					m_bInvincible = true;
					m_bHitEffect = true;
					m_InvincibleTime = 1.f;
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
		m_Animator->CompleteEvent(L"Player_Dash_Left") = std::bind(&PlayerScript::CompleteAction, this);
		m_Animator->Create(L"Player_Dash_Right", pAtlas, Vector2(0.f, 4400.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->CompleteEvent(L"Player_Dash_Right") = std::bind(&PlayerScript::CompleteAction, this);
		m_Animator->Create(L"Player_Dash_Up", pAtlas, Vector2(0.f, 4600.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->CompleteEvent(L"Player_Dash_Up") = std::bind(&PlayerScript::CompleteAction, this);
		m_Animator->Create(L"Player_Dash_Down", pAtlas, Vector2(0.f, 4100.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->CompleteEvent(L"Player_Dash_Down") = std::bind(&PlayerScript::CompleteAction, this);
		m_Animator->Create(L"Player_Dash_LeftUp", pAtlas, Vector2(0.f, 4500.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->CompleteEvent(L"Player_Dash_LeftUp") = std::bind(&PlayerScript::CompleteAction, this);
		m_Animator->Create(L"Player_Dash_RightUp", pAtlas, Vector2(0.f, 4700.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->CompleteEvent(L"Player_Dash_RightUp") = std::bind(&PlayerScript::CompleteAction, this);
		m_Animator->Create(L"Player_Dash_LeftDown", pAtlas, Vector2(0.f, 4000.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->CompleteEvent(L"Player_Dash_LeftDown") = std::bind(&PlayerScript::CompleteAction, this);
		m_Animator->Create(L"Player_Dash_RightDown", pAtlas, Vector2(0.f, 4200.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->CompleteEvent(L"Player_Dash_RightDown") = std::bind(&PlayerScript::CompleteAction, this);

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
		m_Animator->Create(L"Player_Wand_Left", pAtlas, Vector2(0.f, 5600.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.07f, false);
		m_Animator->StartEvent(L"Player_Wand_Left") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_Wand_Left") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_Wand_Right", pAtlas, Vector2(0.f, 5700.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.07f, false);
		m_Animator->StartEvent(L"Player_Wand_Right") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_Wand_Right") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_Wand_Up", pAtlas, Vector2(0.f, 5900.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.07f, false);
		m_Animator->StartEvent(L"Player_Wand_Up") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_Wand_Up") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_Wand_Down", pAtlas, Vector2(0.f, 5400.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.07f, false);
		m_Animator->StartEvent(L"Player_Wand_Down") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_Wand_Down") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_Wand_LeftUp", pAtlas, Vector2(0.f, 5800.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.07f, false);
		m_Animator->StartEvent(L"Player_Wand_LeftUp") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_Wand_LeftUp") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_Wand_RightUp", pAtlas, Vector2(0.f, 6000.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.07f, false);
		m_Animator->StartEvent(L"Player_Wand_RightUp") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_Wand_RightUp") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_Wand_LeftDown", pAtlas, Vector2(0.f, 5300.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.07f, false);
		m_Animator->StartEvent(L"Player_Wand_LeftDown") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_Wand_LeftDown") = std::bind(&PlayerScript::CompleteAction, this);

		m_Animator->Create(L"Player_Wand_RightDown", pAtlas, Vector2(0.f, 5500.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.07f, false);
		m_Animator->StartEvent(L"Player_Wand_RightDown") = std::bind(&PlayerScript::AttackSFX, this);
		m_Animator->CompleteEvent(L"Player_Wand_RightDown") = std::bind(&PlayerScript::CompleteAction, this);

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
		if (Input::GetKeyDown(eKeyCode::G))
		{
			if (1 == m_Damage)
				m_Damage = 0;
			else
				m_Damage = 1;
		}

		StateScript::eState eCurState = m_CurState->GetStateType();

		if (m_bAction || StateScript::eState::Dead == eCurState)
			return;

		// 이동 모드
		if (Input::GetKeyDown(eKeyCode::SHIFT))
		{
			if (eMoveType::Walk == m_MoveType)
				m_MoveType = eMoveType::Run;
			else
				m_MoveType = eMoveType::Walk;
		}

		// 공격 모드 (임시)
		if (Input::GetKeyDown(eKeyCode::R))
		{
			if (eWeaponType::Wand == m_CurWeapon)
				m_CurWeapon = eWeaponType::Bow;
			else
				m_CurWeapon = eWeaponType::Wand;
		}

		// 대쉬
		if (Input::GetKeyDown(eKeyCode::SPACE))
		{
			m_PrevDir = m_Dir;
			m_Dir = CalDirToMouse();
			m_bInvincible = true;
			m_InvincibleTime = 0.6f;
			ChangeState(StateScript::eState::Dash);
		}

		// 공격
		if (Input::GetKeyUp(eKeyCode::LBUTTON))
		{
			m_PrevDir = m_Dir;
			m_Dir = CalDirToMouse();
			ChangeState(StateScript::eState::Attack);
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
			&& m_PrevDir == m_Dir)
			return;

		switch (eCurState)
		{
		case StateScript::eState::Idle:
		{
			if (!m_bDontAnimChange)
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
			case Lu::PlayerScript::eWeaponType::Sword:
				break;
			case Lu::PlayerScript::eWeaponType::Bow:
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
			case Lu::PlayerScript::eWeaponType::Wand:
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
			}
		}
			break;
		case StateScript::eState::Dead:
			m_Animator->PlayAnimation(L"Player_Dead", true);
			break;
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
		m_Animator->PlayAnimation(L"Player_MagicCircleMove", true);
	}

	void PlayerScript::IdleStateEvent()
	{
		m_bDontAnimChange = true;
		m_Dir = eDir::Down;
		m_Animator->PlayAnimation(L"Player_LookAround", true);
	}
}