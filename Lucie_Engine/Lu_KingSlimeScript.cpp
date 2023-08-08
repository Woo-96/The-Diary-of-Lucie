#include "Lu_KingSlimeScript.h"
#include "Lu_GameObject.h"
#include "Lu_Animator.h"
#include "Lu_Resources.h"
#include "Lu_PlayerScript.h"

//#include "Lu_KingSlimeIdleState.h"
//#include "Lu_KingSlimePatrolState.h"
//#include "Lu_KingSlimeAttackState.h"
//#include "Lu_KingSlimeDeadState.h"

namespace Lu
{
	KingSlimeScript::KingSlimeScript()
		: m_CurState(nullptr)
		, m_PrevState(KingSlimeStateScript::eState::End)
	{
		SetName(L"KingSlimeScript");

		GetInfo().HP = 100;
		GetInfo().DetectRange = 200.f;
		GetInfo().MoveSpeed = 100.f;
	}

	KingSlimeScript::~KingSlimeScript()
	{
		for (const auto& pair : m_State)
		{
			if (nullptr != pair.second)
				delete pair.second;
		}

		m_State.clear();
	}

	void KingSlimeScript::Initialize()
	{
		MonsterScript::Initialize();

		//AddState(new KingSlimeIdleState);
		//AddState(new KingSlimePatrolState);
		//AddState(new KingSlimeAttackState);
		//AddState(new KingSlimeDeadState);

		m_CurState = GetStateScript(KingSlimeStateScript::eState::Idle);
		m_CurState->Enter();
	}

	void KingSlimeScript::Update()
	{
		MonsterScript::Update();

		if (GetOwner()->IsDead())
			return;

		m_CurState->Update();
		AnimationUpdate();

		m_PrevState = m_CurState->GetStateType();
	}

	void KingSlimeScript::OnCollisionEnter(Collider2D* _Other)
	{
		if ((int)eLayerType::Immovable == _Other->GetOwner()->GetLayerIndex())
			ChangeState(KingSlimeStateScript::eState::Idle);

		if ((int)eLayerType::PlayerProjectile == _Other->GetOwner()->GetLayerIndex())
		{
			if (KingSlimeStateScript::eState::Dead == m_CurState->GetStateType())
				return;

			GetInfo().HP -= 50;

			if (GetInfo().HP <= 0.f)
			{
				ChangeState(KingSlimeStateScript::eState::Dead);
			}
		}
	}

	void KingSlimeScript::OnCollisionStay(Collider2D* _Other)
	{
	}

	void KingSlimeScript::OnCollisionExit(Collider2D* _Other)
	{
	}

	void KingSlimeScript::CreateAnimation()
	{
		std::shared_ptr<Texture> pAtlas
			= Resources::Load<Texture>(L"KingSlimeSprite", L"..\\Resources\\Texture\\Monster\\Normal\\KingSlime.png");

		// Idle
		GetAnimator()->Create(L"Snbay_Idle_Left", pAtlas, Vector2(0.f, 432.f), Vector2(48.f, 48.f), 2, Vector2(48.f, 48.f), Vector2::Zero, 0.5f);
		GetAnimator()->Create(L"Snbay_Idle_Right", pAtlas, Vector2(0.f, 480.f), Vector2(48.f, 48.f), 2, Vector2(48.f, 48.f), Vector2::Zero, 0.5f);
		GetAnimator()->Create(L"Snbay_Idle_Up", pAtlas, Vector2(0.f, 528.f), Vector2(48.f, 48.f), 2, Vector2(48.f, 48.f), Vector2::Zero, 0.5f);
		GetAnimator()->Create(L"Snbay_Idle_Down", pAtlas, Vector2(0.f, 384.f), Vector2(48.f, 48.f), 2, Vector2(48.f, 48.f), Vector2::Zero, 0.5f);

		// Move
		GetAnimator()->Create(L"Snbay_Move_Left", pAtlas, Vector2(0.f, 48.f), Vector2(48.f, 48.f), 3, Vector2(48.f, 48.f), Vector2::Zero, 0.3f);
		GetAnimator()->Create(L"Snbay_Move_Right", pAtlas, Vector2(0.f, 96.f), Vector2(48.f, 48.f), 3, Vector2(48.f, 48.f), Vector2::Zero, 0.3f);
		GetAnimator()->Create(L"Snbay_Move_Up", pAtlas, Vector2(0.f, 144.f), Vector2(48.f, 48.f), 3, Vector2(48.f, 48.f), Vector2::Zero, 0.3f);
		GetAnimator()->Create(L"Snbay_Move_Down", pAtlas, Vector2(0.f, 0.f), Vector2(48.f, 48.f), 3, Vector2(48.f, 48.f), Vector2::Zero, 0.3f);

		// Attack
		GetAnimator()->Create(L"Snbay_Attack_Left", pAtlas, Vector2(0.f, 240.f), Vector2(48.f, 48.f), 3, Vector2(48.f, 48.f), Vector2::Zero, 0.3f);
		GetAnimator()->Create(L"Snbay_Attack_Right", pAtlas, Vector2(0.f, 288.f), Vector2(48.f, 48.f), 3, Vector2(48.f, 48.f), Vector2::Zero, 0.3f);
		GetAnimator()->Create(L"Snbay_Attack_Up", pAtlas, Vector2(0.f, 336.f), Vector2(48.f, 48.f), 3, Vector2(48.f, 48.f), Vector2::Zero, 0.3f);
		GetAnimator()->Create(L"Snbay_Attack_Down", pAtlas, Vector2(0.f, 192.f), Vector2(48.f, 48.f), 3, Vector2(48.f, 48.f), Vector2::Zero, 0.3f);

		// Dead
		GetAnimator()->Create(L"Snbay_Dead", pAtlas, Vector2(0.f, 576.f), Vector2(48.f, 48.f), 1, Vector2(48.f, 48.f), Vector2::Zero, 1.f);
		GetAnimator()->CompleteEvent(L"Snbay_Dead") = std::bind(&KingSlimeScript::CompleteAction, this);
	}

	KingSlimeStateScript* KingSlimeScript::GetStateScript(KingSlimeStateScript::eState _State)
	{
		std::map<KingSlimeStateScript::eState, KingSlimeStateScript*>::iterator iter = m_State.find(_State);

		if (iter == m_State.end())
			return nullptr;

		return iter->second;
	}

	void KingSlimeScript::AddState(KingSlimeStateScript* _State)
	{
		if (nullptr == _State)
			return;

		_State->SetKingSlimeScript(this);
		_State->Initialize();
		KingSlimeStateScript::eState eState = _State->GetStateType();
		m_State.insert(std::make_pair(eState, _State));
	}

	void KingSlimeScript::CompleteAction()
	{
		ChangeState(KingSlimeStateScript::eState::Idle);
	}

	void KingSlimeScript::AnimationUpdate()
	{
		KingSlimeStateScript::eState eCurState = m_CurState->GetStateType();
		eAnimDir eCurDir = GetCurDir();

		if (m_PrevState == eCurState
			&& GetPrevDir() == eCurDir)
			return;

		switch (eCurState)
		{
		case KingSlimeStateScript::eState::Idle:
		{
			switch (eCurDir)
			{
			case Lu::MonsterScript::eAnimDir::Left:
				GetAnimator()->PlayAnimation(L"Snbay_Idle_Left", true);
				break;
			case Lu::MonsterScript::eAnimDir::Right:
				GetAnimator()->PlayAnimation(L"Snbay_Idle_Right", true);
				break;
			case Lu::MonsterScript::eAnimDir::Up:
				GetAnimator()->PlayAnimation(L"Snbay_Idle_Up", true);
				break;
			case Lu::MonsterScript::eAnimDir::Down:
				GetAnimator()->PlayAnimation(L"Snbay_Idle_Down", true);
				break;
			default:
				break;
			}
		}
		break;
		case KingSlimeStateScript::eState::Patrol:
		{
			switch (eCurDir)
			{
			case Lu::MonsterScript::eAnimDir::Left:
				GetAnimator()->PlayAnimation(L"Snbay_Move_Left", true);
				break;
			case Lu::MonsterScript::eAnimDir::Right:
				GetAnimator()->PlayAnimation(L"Snbay_Move_Right", true);
				break;
			case Lu::MonsterScript::eAnimDir::Up:
				GetAnimator()->PlayAnimation(L"Snbay_Move_Up", true);
				break;
			case Lu::MonsterScript::eAnimDir::Down:
				GetAnimator()->PlayAnimation(L"Snbay_Move_Down", true);
				break;
			default:
				break;
			}
		}
		break;
		case KingSlimeStateScript::eState::Attack:
		{
			switch (eCurDir)
			{
			case Lu::MonsterScript::eAnimDir::Left:
				GetAnimator()->PlayAnimation(L"Snbay_Attack_Left", true);
				break;
			case Lu::MonsterScript::eAnimDir::Right:
				GetAnimator()->PlayAnimation(L"Snbay_Attack_Right", true);
				break;
			case Lu::MonsterScript::eAnimDir::Up:
				GetAnimator()->PlayAnimation(L"Snbay_Attack_Up", true);
				break;
			case Lu::MonsterScript::eAnimDir::Down:
				GetAnimator()->PlayAnimation(L"Snbay_Attack_Down", true);
				break;
			default:
				break;
			}
		}
		break;
		case KingSlimeStateScript::eState::Dead:
			GetAnimator()->PlayAnimation(L"Snbay_Dead", true);
			break;
		}
	}

	void KingSlimeScript::ChangeState(KingSlimeStateScript::eState _NextState)
	{
		KingSlimeStateScript* pNextState = GetStateScript(_NextState);

		if (pNextState->GetStateType() == m_CurState->GetStateType())
			return;

		m_CurState->Exit();
		m_PrevState = m_CurState->GetStateType();
		m_CurState = pNextState;
		m_CurState->Enter();
	}
}