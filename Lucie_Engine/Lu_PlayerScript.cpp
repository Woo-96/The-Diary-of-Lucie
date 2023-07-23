#include "Lu_PlayerScript.h"
#include "Lu_Animator.h"
#include "Lu_GameObject.h"
#include "Lu_Input.h"
#include "Lu_Resources.h"
#include "Lu_Renderer.h"
#include "Lu_Camera.h"

#include "Lu_IdleState.h"
#include "Lu_MoveState.h"
#include "Lu_AttackState.h"
#include "Lu_DashState.h"
#include "Lu_DeadState.h"

namespace Lu
{
	PlayerScript::PlayerScript()
		: m_CurState(nullptr)
		, m_PrevState(StateScript::eState::End)
		, m_Dir(ePlayerDir::Down)
		, m_PrevDir(ePlayerDir::None)
		, m_MoveType(eMoveType::Walk)
		, m_CurWeapon(eWeaponType::None)
		, m_bAction(false)
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
		AddState(new AttackState);
		AddState(new DashState);
		AddState(new DeadState);

		m_CurState = GetStateScript(StateScript::eState::Idle);
		m_CurState->Enter();

		m_CurWeapon = eWeaponType::Wand;
	}

	void PlayerScript::Update()
	{
		StateUpdate();
 		m_CurState->Update();
		AnimationUpdate();

		m_PrevState = m_CurState->GetStateType();
	}

	void PlayerScript::OnCollisionEnter(Collider2D* other)
	{
	}

	void PlayerScript::OnCollisionStay(Collider2D* other)
	{
	}

	void PlayerScript::OnCollisionExit(Collider2D* other)
	{
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


		// Attack - Sword


		// Attack - Bow
		m_Animator->Create(L"Player_Bow_Left", pAtlas, Vector2(300.f, 1500.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->CompleteEvent(L"Player_Bow_Left") = std::bind(&PlayerScript::CompleteAction, this);
		m_Animator->Create(L"Player_Bow_Right", pAtlas, Vector2(0.f, 1600.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->CompleteEvent(L"Player_Bow_Right") = std::bind(&PlayerScript::CompleteAction, this);
		m_Animator->Create(L"Player_Bow_Up", pAtlas, Vector2(0.f, 1700.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->CompleteEvent(L"Player_Bow_Up") = std::bind(&PlayerScript::CompleteAction, this);
		m_Animator->Create(L"Player_Bow_Down", pAtlas, Vector2(300.f, 1400.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->CompleteEvent(L"Player_Bow_Down") = std::bind(&PlayerScript::CompleteAction, this);
		m_Animator->Create(L"Player_Bow_LeftUp", pAtlas, Vector2(300.f, 1600.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->CompleteEvent(L"Player_Bow_LeftUp") = std::bind(&PlayerScript::CompleteAction, this);
		m_Animator->Create(L"Player_Bow_RightUp", pAtlas, Vector2(300.f, 1700.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->CompleteEvent(L"Player_Bow_RightUp") = std::bind(&PlayerScript::CompleteAction, this);
		m_Animator->Create(L"Player_Bow_LeftDown", pAtlas, Vector2(0.f, 1400.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->CompleteEvent(L"Player_Bow_LeftDown") = std::bind(&PlayerScript::CompleteAction, this);
		m_Animator->Create(L"Player_Bow_RightDown", pAtlas, Vector2(0.f, 1500.f), Vector2(100.f, 100.f), 3, Vector2(100.f, 100.f), Vector2::Zero, 0.1f, false);
		m_Animator->CompleteEvent(L"Player_Bow_RightDown") = std::bind(&PlayerScript::CompleteAction, this);


		// Attack - Wand
		m_Animator->Create(L"Player_Wand_Left", pAtlas, Vector2(0.f, 5600.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.07f, false);
		m_Animator->CompleteEvent(L"Player_Wand_Left") = std::bind(&PlayerScript::CompleteAction, this);
		m_Animator->Create(L"Player_Wand_Right", pAtlas, Vector2(0.f, 5700.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.07f, false);
		m_Animator->CompleteEvent(L"Player_Wand_Right") = std::bind(&PlayerScript::CompleteAction, this);
		m_Animator->Create(L"Player_Wand_Up", pAtlas, Vector2(0.f, 5900.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.07f, false);
		m_Animator->CompleteEvent(L"Player_Wand_Up") = std::bind(&PlayerScript::CompleteAction, this);
		m_Animator->Create(L"Player_Wand_Down", pAtlas, Vector2(0.f, 5400.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.07f, false);
		m_Animator->CompleteEvent(L"Player_Wand_Down") = std::bind(&PlayerScript::CompleteAction, this);
		m_Animator->Create(L"Player_Wand_LeftUp", pAtlas, Vector2(0.f, 5800.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.07f, false);
		m_Animator->CompleteEvent(L"Player_Wand_LeftUp") = std::bind(&PlayerScript::CompleteAction, this);
		m_Animator->Create(L"Player_Wand_RightUp", pAtlas, Vector2(0.f, 6000.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.07f, false);
		m_Animator->CompleteEvent(L"Player_Wand_RightUp") = std::bind(&PlayerScript::CompleteAction, this);
		m_Animator->Create(L"Player_Wand_LeftDown", pAtlas, Vector2(0.f, 5300.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.07f, false);
		m_Animator->CompleteEvent(L"Player_Wand_LeftDown") = std::bind(&PlayerScript::CompleteAction, this);
		m_Animator->Create(L"Player_Wand_RightDown", pAtlas, Vector2(0.f, 5500.f), Vector2(100.f, 100.f), 6, Vector2(100.f, 100.f), Vector2::Zero, 0.07f, false);
		m_Animator->CompleteEvent(L"Player_Wand_RightDown") = std::bind(&PlayerScript::CompleteAction, this);


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

		// Dead
		m_Animator->Create(L"Player_Dead", pAtlas, Vector2(400.f, 2300.f), Vector2(100.f, 100.f), 2, Vector2(100.f, 100.f), Vector2::Zero, 0.4f, false);
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

		_State->SetOwner(this);
		_State->Initialize();
		StateScript::eState eState = _State->GetStateType();
		m_State.insert(std::make_pair(eState, _State));
	}

	void PlayerScript::StateUpdate()
	{
		StateScript::eState eCurState = m_CurState->GetStateType();

		if (m_bAction)
			return;

		// 이동 모드
		if (Input::GetKeyDown(eKeyCode::CTRL))
		{
			if (eMoveType::Walk == m_MoveType)
				m_MoveType = eMoveType::Run;
			else
				m_MoveType = eMoveType::Walk;
		}

		// 공격 모드 (임시)
		if (Input::GetKeyDown(eKeyCode::ALT))
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
			ChangeState(StateScript::eState::Dash);
		}

		// 공격
		if (Input::GetKeyUp(eKeyCode::LBUTTON))
		{
			m_PrevDir = m_Dir;
			m_Dir = CalDirToMouse();
			ChangeState(StateScript::eState::Attack);
		}

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
			m_Dir = ePlayerDir::LeftUp;
			ChangeState(StateScript::eState::Move);
		}
		else if (Input::GetKey(eKeyCode::A)
			&& Input::GetKey(eKeyCode::S))
		{
			m_PrevDir = m_Dir;
			m_Dir = ePlayerDir::LeftDown;
			ChangeState(StateScript::eState::Move);
		}
		else if (Input::GetKey(eKeyCode::D)
			&& Input::GetKey(eKeyCode::S))
		{
			m_PrevDir = m_Dir;
			m_Dir = ePlayerDir::RightDown;
			ChangeState(StateScript::eState::Move);
		}
		else if (Input::GetKey(eKeyCode::D)
			&& Input::GetKey(eKeyCode::W))
		{
			m_PrevDir = m_Dir;
			m_Dir = ePlayerDir::RightUp;
			ChangeState(StateScript::eState::Move);
		}
		else if (Input::GetKey(eKeyCode::A))
		{
			m_PrevDir = m_Dir;
			m_Dir = ePlayerDir::Left;
			ChangeState(StateScript::eState::Move);
		}
		else if (Input::GetKey(eKeyCode::D))
		{
			m_PrevDir = m_Dir;
			m_Dir = ePlayerDir::Right;
			ChangeState(StateScript::eState::Move);
		}
		else if (Input::GetKey(eKeyCode::S))
		{
			m_PrevDir = m_Dir;
			m_Dir = ePlayerDir::Down;
			ChangeState(StateScript::eState::Move);
		}
		else if (Input::GetKey(eKeyCode::W))
		{
			m_PrevDir = m_Dir;
			m_Dir = ePlayerDir::Up;
			ChangeState(StateScript::eState::Move);
		}
		else
		{
			if (m_bAction)
				return;

			ChangeState(StateScript::eState::Idle);
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
			switch (m_Dir)
			{
			case Lu::PlayerScript::ePlayerDir::Left:
				m_Animator->PlayAnimation(L"Player_Idle_Left", true);
				break;
			case Lu::PlayerScript::ePlayerDir::Right:
				m_Animator->PlayAnimation(L"Player_Idle_Right", true);
				break;
			case Lu::PlayerScript::ePlayerDir::Up:
				m_Animator->PlayAnimation(L"Player_Idle_Up", true);
				break;
			case Lu::PlayerScript::ePlayerDir::Down:
				m_Animator->PlayAnimation(L"Player_Idle_Down", true);
				break;
			case Lu::PlayerScript::ePlayerDir::LeftUp:
				m_Animator->PlayAnimation(L"Player_Idle_LeftUp", true);
				break;
			case Lu::PlayerScript::ePlayerDir::RightUp:
				m_Animator->PlayAnimation(L"Player_Idle_RightUp", true);
				break;
			case Lu::PlayerScript::ePlayerDir::LeftDown:
				m_Animator->PlayAnimation(L"Player_Idle_LeftDown", true);
				break;
			case Lu::PlayerScript::ePlayerDir::RightDown:
				m_Animator->PlayAnimation(L"Player_Idle_RightDown", true);
				break;
			}
		}
			break;
		case StateScript::eState::Move:
		{
			if (eMoveType::Walk == m_MoveType)
			{
				switch (m_Dir)
				{
				case Lu::PlayerScript::ePlayerDir::Left:
					m_Animator->PlayAnimation(L"Player_Walk_Left", true);
					break;
				case Lu::PlayerScript::ePlayerDir::Right:
					m_Animator->PlayAnimation(L"Player_Walk_Right", true);
					break;
				case Lu::PlayerScript::ePlayerDir::Up:
					m_Animator->PlayAnimation(L"Player_Walk_Up", true);
					break;
				case Lu::PlayerScript::ePlayerDir::Down:
					m_Animator->PlayAnimation(L"Player_Walk_Down", true);
					break;
				case Lu::PlayerScript::ePlayerDir::LeftUp:
					m_Animator->PlayAnimation(L"Player_Walk_LeftUp", true);
					break;
				case Lu::PlayerScript::ePlayerDir::RightUp:
					m_Animator->PlayAnimation(L"Player_Walk_RightUp", true);
					break;
				case Lu::PlayerScript::ePlayerDir::LeftDown:
					m_Animator->PlayAnimation(L"Player_Walk_LeftDown", true);
					break;
				case Lu::PlayerScript::ePlayerDir::RightDown:
					m_Animator->PlayAnimation(L"Player_Walk_RightDown", true);
					break;
				}
			}
			else
			{
				switch (m_Dir)
				{
				case Lu::PlayerScript::ePlayerDir::Left:
					m_Animator->PlayAnimation(L"Player_Run_Left", true);
					break;
				case Lu::PlayerScript::ePlayerDir::Right:
					m_Animator->PlayAnimation(L"Player_Run_Right", true);
					break;
				case Lu::PlayerScript::ePlayerDir::Up:
					m_Animator->PlayAnimation(L"Player_Run_Up", true);
					break;
				case Lu::PlayerScript::ePlayerDir::Down:
					m_Animator->PlayAnimation(L"Player_Run_Down", true);
					break;
				case Lu::PlayerScript::ePlayerDir::LeftUp:
					m_Animator->PlayAnimation(L"Player_Run_LeftUp", true);
					break;
				case Lu::PlayerScript::ePlayerDir::RightUp:
					m_Animator->PlayAnimation(L"Player_Run_RightUp", true);
					break;
				case Lu::PlayerScript::ePlayerDir::LeftDown:
					m_Animator->PlayAnimation(L"Player_Run_LeftDown", true);
					break;
				case Lu::PlayerScript::ePlayerDir::RightDown:
					m_Animator->PlayAnimation(L"Player_Run_RightDown", true);
					break;
				}
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
				case Lu::PlayerScript::ePlayerDir::Left:
					m_Animator->PlayAnimation(L"Player_Bow_Left", true);
					break;
				case Lu::PlayerScript::ePlayerDir::Right:
					m_Animator->PlayAnimation(L"Player_Bow_Right", true);
					break;
				case Lu::PlayerScript::ePlayerDir::Up:
					m_Animator->PlayAnimation(L"Player_Bow_Up", true);
					break;
				case Lu::PlayerScript::ePlayerDir::Down:
					m_Animator->PlayAnimation(L"Player_Bow_Down", true);
					break;
				case Lu::PlayerScript::ePlayerDir::LeftUp:
					m_Animator->PlayAnimation(L"Player_Bow_LeftUp", true);
					break;
				case Lu::PlayerScript::ePlayerDir::RightUp:
					m_Animator->PlayAnimation(L"Player_Bow_RightUp", true);
					break;
				case Lu::PlayerScript::ePlayerDir::LeftDown:
					m_Animator->PlayAnimation(L"Player_Bow_LeftDown", true);
					break;
				case Lu::PlayerScript::ePlayerDir::RightDown:
					m_Animator->PlayAnimation(L"Player_Bow_RightDown", true);
					break;
				}
			}
				break;
			case Lu::PlayerScript::eWeaponType::Wand:
			{
				switch (m_Dir)
				{
				case Lu::PlayerScript::ePlayerDir::Left:
					m_Animator->PlayAnimation(L"Player_Wand_Left", true);
					break;
				case Lu::PlayerScript::ePlayerDir::Right:
					m_Animator->PlayAnimation(L"Player_Wand_Right", true);
					break;
				case Lu::PlayerScript::ePlayerDir::Up:
					m_Animator->PlayAnimation(L"Player_Wand_Up", true);
					break;
				case Lu::PlayerScript::ePlayerDir::Down:
					m_Animator->PlayAnimation(L"Player_Wand_Down", true);
					break;
				case Lu::PlayerScript::ePlayerDir::LeftUp:
					m_Animator->PlayAnimation(L"Player_Wand_LeftUp", true);
					break;
				case Lu::PlayerScript::ePlayerDir::RightUp:
					m_Animator->PlayAnimation(L"Player_Wand_RightUp", true);
					break;
				case Lu::PlayerScript::ePlayerDir::LeftDown:
					m_Animator->PlayAnimation(L"Player_Wand_LeftDown", true);
					break;
				case Lu::PlayerScript::ePlayerDir::RightDown:
					m_Animator->PlayAnimation(L"Player_Wand_RightDown", true);
					break;
				}
			}
				break;
			}
		}
			break;
		case StateScript::eState::Dash:
		{
			switch (m_Dir)
			{
			case Lu::PlayerScript::ePlayerDir::Left:
				m_Animator->PlayAnimation(L"Player_Dash_Left", true);
				break;
			case Lu::PlayerScript::ePlayerDir::Right:
				m_Animator->PlayAnimation(L"Player_Dash_Right", true);
				break;
			case Lu::PlayerScript::ePlayerDir::Up:
				m_Animator->PlayAnimation(L"Player_Dash_Up", true);
				break;
			case Lu::PlayerScript::ePlayerDir::Down:
				m_Animator->PlayAnimation(L"Player_Dash_Down", true);
				break;
			case Lu::PlayerScript::ePlayerDir::LeftUp:
				m_Animator->PlayAnimation(L"Player_Dash_LeftUp", true);
				break;
			case Lu::PlayerScript::ePlayerDir::RightUp:
				m_Animator->PlayAnimation(L"Player_Dash_RightUp", true);
				break;
			case Lu::PlayerScript::ePlayerDir::LeftDown:
				m_Animator->PlayAnimation(L"Player_Dash_LeftDown", true);
				break;
			case Lu::PlayerScript::ePlayerDir::RightDown:
				m_Animator->PlayAnimation(L"Player_Dash_RightDown", true);
				break;
			}
		}
			break;
		case StateScript::eState::Dead:
			m_Animator->PlayAnimation(L"Player_Dead", false);
			break;
		}
	}

	PlayerScript::ePlayerDir PlayerScript::CalDirToMouse()
	{
		Vector2 vScreenPos = Lu::Input::GetMousePos();
		Vector3 vMouseWorldPos = renderer::mainCamera->ScreenToWorld(vScreenPos);
		Vector3 vPlayerPos = GetOwner()->GetComponent<Transform>()->GetPosition();

		Vector3 vDir = vMouseWorldPos - vPlayerPos;
		vDir.Normalize();

		float angle = atan2(vDir.y, vDir.x) * (180.0f / math::PI);

		if (angle > -22.5f && angle <= 22.5f)
			return ePlayerDir::Right;
		else if (angle > 22.5f && angle <= 67.5f)
			return ePlayerDir::RightUp;
		else if (angle > 67.5f && angle <= 112.5f)
			return ePlayerDir::Up;
		else if (angle > 112.5f && angle <= 157.5f)
			return ePlayerDir::LeftUp;
		else if (angle > 157.5f || angle <= -157.5f)
			return ePlayerDir::Left;
		else if (angle > -157.5f && angle <= -112.5f)
			return ePlayerDir::LeftDown;
		else if (angle > -112.5f && angle <= -67.5f)
			return ePlayerDir::Down;
		else if (angle > -67.5f && angle <= -22.5f)
			return ePlayerDir::RightDown;

		return ePlayerDir::None;
	}

	void PlayerScript::CompleteAction()
	{
		ChangeState(StateScript::eState::Idle);
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
}