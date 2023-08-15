#include "Lu_SlimeScript.h"
#include "Lu_GameObject.h"
#include "Lu_Animator.h"
#include "Lu_Resources.h"
#include "Lu_PlayerScript.h"

#include "Lu_SlimeIdleState.h"
#include "Lu_SlimePatrolState.h"
#include "Lu_SlimeAttackState.h"
#include "Lu_SlimeDeadState.h"

namespace Lu
{
	SlimeScript::SlimeScript()
		: m_CurState(nullptr)
		, m_PrevState(SlimeStateScript::eState::End)
		, m_Target(nullptr)
	{
		SetName(L"SlimeScript");

		GetInfo().HP = 100;
		GetInfo().MaxHP = 100;
		GetInfo().DetectRange = 200.f;
		GetInfo().MoveSpeed = 100.f;
	}

	SlimeScript::~SlimeScript()
	{
		for (const auto& pair : m_State)
		{
			if (nullptr != pair.second)
				delete pair.second;
		}

		m_State.clear();
	}

	void SlimeScript::Initialize()
	{
		MonsterScript::Initialize();

		AddState(new SlimeIdleState);
		AddState(new SlimePatrolState);
		AddState(new SlimeAttackState);
		AddState(new SlimeDeadState);

		m_CurState = GetStateScript(SlimeStateScript::eState::Idle);
		m_CurState->Enter();
	}

	void SlimeScript::Update()
	{
		MonsterScript::Update();

		if (GetOwner()->IsDead())
			return;

		m_CurState->Update();
		AnimationUpdate();

		m_PrevState = m_CurState->GetStateType();
		SetPrevDir(GetCurDir());
	}

	void SlimeScript::OnCollisionEnter(Collider2D* _Other)
	{
		if ((int)eLayerType::Immovable == _Other->GetOwner()->GetLayerIndex())
			ChangeState(SlimeStateScript::eState::Idle);

		if ((int)eLayerType::PlayerProjectile == _Other->GetOwner()->GetLayerIndex())
		{
			if (SlimeStateScript::eState::Dead == m_CurState->GetStateType())
				return;

			GetInfo().HP -= 50;

			if (GetInfo().HP <= 0.f)
			{
				ChangeState(SlimeStateScript::eState::Dead);
			}
		}
	}

	void SlimeScript::OnCollisionStay(Collider2D* _Other)
	{
	}

	void SlimeScript::OnCollisionExit(Collider2D* _Other)
	{
	}

	void SlimeScript::CreateAnimation()
	{
		std::shared_ptr<Texture> pAtlas
			= Resources::Load<Texture>(L"SlimeSprite", L"..\\Resources\\Texture\\Monster\\MidBoss\\Slime.png");

		// Idle
		GetAnimator()->Create(L"Slime_Idle_Left", pAtlas, Vector2(0.f, 432.f), Vector2(48.f, 48.f), 2, Vector2(48.f, 48.f), Vector2::Zero, 0.5f);
		GetAnimator()->Create(L"Slime_Idle_Right", pAtlas, Vector2(0.f, 480.f), Vector2(48.f, 48.f), 2, Vector2(48.f, 48.f), Vector2::Zero, 0.5f);
		GetAnimator()->Create(L"Slime_Idle_Up", pAtlas, Vector2(0.f, 528.f), Vector2(48.f, 48.f), 2, Vector2(48.f, 48.f), Vector2::Zero, 0.5f);
		GetAnimator()->Create(L"Slime_Idle_Down", pAtlas, Vector2(0.f, 384.f), Vector2(48.f, 48.f), 2, Vector2(48.f, 48.f), Vector2::Zero, 0.5f);

		// Move
		GetAnimator()->Create(L"Slime_Move_Left", pAtlas, Vector2(0.f, 48.f), Vector2(48.f, 48.f), 3, Vector2(48.f, 48.f), Vector2::Zero, 0.3f);
		GetAnimator()->Create(L"Slime_Move_Right", pAtlas, Vector2(0.f, 96.f), Vector2(48.f, 48.f), 3, Vector2(48.f, 48.f), Vector2::Zero, 0.3f);
		GetAnimator()->Create(L"Slime_Move_Up", pAtlas, Vector2(0.f, 144.f), Vector2(48.f, 48.f), 3, Vector2(48.f, 48.f), Vector2::Zero, 0.3f);
		GetAnimator()->Create(L"Slime_Move_Down", pAtlas, Vector2(0.f, 0.f), Vector2(48.f, 48.f), 3, Vector2(48.f, 48.f), Vector2::Zero, 0.3f);

		// Attack
		GetAnimator()->Create(L"Slime_Attack_Left", pAtlas, Vector2(0.f, 240.f), Vector2(48.f, 48.f), 3, Vector2(48.f, 48.f), Vector2::Zero, 0.3f);
		GetAnimator()->Create(L"Slime_Attack_Right", pAtlas, Vector2(0.f, 288.f), Vector2(48.f, 48.f), 3, Vector2(48.f, 48.f), Vector2::Zero, 0.3f);
		GetAnimator()->Create(L"Slime_Attack_Up", pAtlas, Vector2(0.f, 336.f), Vector2(48.f, 48.f), 3, Vector2(48.f, 48.f), Vector2::Zero, 0.3f);
		GetAnimator()->Create(L"Slime_Attack_Down", pAtlas, Vector2(0.f, 192.f), Vector2(48.f, 48.f), 3, Vector2(48.f, 48.f), Vector2::Zero, 0.3f);

		// Dead
		GetAnimator()->Create(L"Slime_Dead", pAtlas, Vector2(0.f, 576.f), Vector2(48.f, 48.f), 1, Vector2(48.f, 48.f), Vector2::Zero, 1.f);
		GetAnimator()->CompleteEvent(L"Slime_Dead") = std::bind(&SlimeScript::CompleteAction, this);
	}

	SlimeStateScript* SlimeScript::GetStateScript(SlimeStateScript::eState _State)
	{
		std::map<SlimeStateScript::eState, SlimeStateScript*>::iterator iter = m_State.find(_State);

		if (iter == m_State.end())
			return nullptr;

		return iter->second;
	}

	void SlimeScript::AddState(SlimeStateScript* _State)
	{
		if (nullptr == _State)
			return;

		_State->SetSlimeScript(this);
		_State->Initialize();
		SlimeStateScript::eState eState = _State->GetStateType();
		m_State.insert(std::make_pair(eState, _State));
	}

	void SlimeScript::CompleteAction()
	{
		ChangeState(SlimeStateScript::eState::Idle);
	}

	void SlimeScript::AnimationUpdate()
	{
		SlimeStateScript::eState eCurState = m_CurState->GetStateType();
		eAnimDir eCurDir = GetCurDir();

		if (m_PrevState == eCurState
			&& GetPrevDir() == eCurDir)
			return;

		switch (eCurState)
		{
		case SlimeStateScript::eState::Idle:
		{
			switch (eCurDir)
			{
			case Lu::MonsterScript::eAnimDir::Left:
				GetAnimator()->PlayAnimation(L"Slime_Idle_Left", true);
				break;
			case Lu::MonsterScript::eAnimDir::Right:
				GetAnimator()->PlayAnimation(L"Slime_Idle_Right", true);
				break;
			case Lu::MonsterScript::eAnimDir::Up:
				GetAnimator()->PlayAnimation(L"Slime_Idle_Up", true);
				break;
			case Lu::MonsterScript::eAnimDir::Down:
				GetAnimator()->PlayAnimation(L"Slime_Idle_Down", true);
				break;
			default:
				break;
			}
		}
		break;
		case SlimeStateScript::eState::Patrol:
		{
			switch (eCurDir)
			{
			case Lu::MonsterScript::eAnimDir::Left:
				GetAnimator()->PlayAnimation(L"Slime_Move_Left", true);
				break;
			case Lu::MonsterScript::eAnimDir::Right:
				GetAnimator()->PlayAnimation(L"Slime_Move_Right", true);
				break;
			case Lu::MonsterScript::eAnimDir::Up:
				GetAnimator()->PlayAnimation(L"Slime_Move_Up", true);
				break;
			case Lu::MonsterScript::eAnimDir::Down:
				GetAnimator()->PlayAnimation(L"Slime_Move_Down", true);
				break;
			default:
				break;
			}
		}
		break;
		case SlimeStateScript::eState::Attack:
		{
			switch (eCurDir)
			{
			case Lu::MonsterScript::eAnimDir::Left:
				GetAnimator()->PlayAnimation(L"Slime_Attack_Left", true);
				break;
			case Lu::MonsterScript::eAnimDir::Right:
				GetAnimator()->PlayAnimation(L"Slime_Attack_Right", true);
				break;
			case Lu::MonsterScript::eAnimDir::Up:
				GetAnimator()->PlayAnimation(L"Slime_Attack_Up", true);
				break;
			case Lu::MonsterScript::eAnimDir::Down:
				GetAnimator()->PlayAnimation(L"Slime_Attack_Down", true);
				break;
			default:
				break;
			}
		}
		break;
		case SlimeStateScript::eState::Dead:
			GetAnimator()->PlayAnimation(L"Slime_Dead", true);
			break;
		}
	}

	void SlimeScript::ChangeState(SlimeStateScript::eState _NextState)
	{
		SlimeStateScript* pNextState = GetStateScript(_NextState);

		if (pNextState->GetStateType() == m_CurState->GetStateType())
			return;

		m_CurState->Exit();
		m_PrevState = m_CurState->GetStateType();
		m_CurState = pNextState;
		m_CurState->Enter();
	}
}