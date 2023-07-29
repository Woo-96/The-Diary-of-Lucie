#include "Lu_SnabyScript.h"
#include "Lu_GameObject.h"
#include "Lu_Animator.h"
#include "Lu_Resources.h"
#include "Lu_PlayerScript.h"

#include "Lu_SnabyIdleState.h"
#include "Lu_SnabyPatrolState.h"
#include "Lu_SnabyAttackState.h"
#include "Lu_SnabyDeadState.h"

namespace Lu
{
	SnabyScript::SnabyScript()
		: m_CurState(nullptr)
		, m_PrevState(SnabyStateScript::eState::End)
		, m_Dir(eDir::Down)
		, m_PrevDir(eDir::None)
		, m_Animator(nullptr)
	{
		SetName(L"SnabyScript");
	}

	SnabyScript::~SnabyScript()
	{
		for (const auto& pair : m_State)
		{
			if (nullptr != pair.second)
				delete pair.second;
		}

		m_State.clear();
	}

	void SnabyScript::Initialize()
	{
		m_Animator = GetOwner()->GetComponent<Animator>();
		CreateSnabyAnimation();

		AddState(new SnabyIdleState);
		AddState(new SnabyPatrolState);
		AddState(new SnabyAttackState);
		AddState(new SnabyDeadState);

		m_CurState = GetStateScript(SnabyStateScript::eState::Idle);
		m_CurState->Enter();
	}

	void SnabyScript::Update()
	{
		if (GetOwner()->IsDead())
			return;

		m_CurState->Update();
		AnimationUpdate();

		m_PrevState = m_CurState->GetStateType();
	}

	void SnabyScript::OnCollisionEnter(Collider2D* _Other)
	{
		if ((int)eLayerType::PlayerProjectile == _Other->GetOwner()->GetLayerIndex())
		{
			if (SnabyStateScript::eState::Dead == m_CurState->GetStateType())
				return;

			GetInfo().HP -= 50;

			if (GetInfo().HP <= 0.f)
			{
				ChangeState(SnabyStateScript::eState::Dead);
			}
		}
	}

	void SnabyScript::OnCollisionStay(Collider2D* _Other)
	{
	}

	void SnabyScript::OnCollisionExit(Collider2D* _Other)
	{
	}

	void SnabyScript::CreateSnabyAnimation()
	{
		std::shared_ptr<Texture> pAtlas
			= Resources::Load<Texture>(L"SnabySprite", L"..\\Resources\\Texture\\Monster\\Normal\\Snaby.png");

		// Idle
		m_Animator->Create(L"Snbay_Idle_Left", pAtlas, Vector2(0.f, 432.f), Vector2(48.f, 48.f), 2, Vector2(48.f, 48.f), Vector2::Zero, 0.5f);
		m_Animator->Create(L"Snbay_Idle_Right", pAtlas, Vector2(0.f, 480.f), Vector2(48.f, 48.f), 2, Vector2(48.f, 48.f), Vector2::Zero, 0.5f);
		m_Animator->Create(L"Snbay_Idle_Up", pAtlas, Vector2(0.f, 528.f), Vector2(48.f, 48.f), 2, Vector2(48.f, 48.f), Vector2::Zero, 0.5f);
		m_Animator->Create(L"Snbay_Idle_Down", pAtlas, Vector2(0.f, 384.f), Vector2(48.f, 48.f), 2, Vector2(48.f, 48.f), Vector2::Zero, 0.5f);
	
	
		// Dead
		m_Animator->Create(L"Snbay_Dead", pAtlas, Vector2(0.f, 576.f), Vector2(48.f, 48.f), 1, Vector2(48.f, 48.f), Vector2::Zero, 1.f);
		m_Animator->CompleteEvent(L"Snbay_Dead") = std::bind(&SnabyScript::CompleteAction, this);
	}

	SnabyStateScript* SnabyScript::GetStateScript(SnabyStateScript::eState _State)
	{
		std::map<SnabyStateScript::eState, SnabyStateScript*>::iterator iter = m_State.find(_State);

		if (iter == m_State.end())
			return nullptr;

		return iter->second;
	}

	void SnabyScript::AddState(SnabyStateScript* _State)
	{
		if (nullptr == _State)
			return;

		_State->SetSnabyScript(this);
		_State->Initialize();
		SnabyStateScript::eState eState = _State->GetStateType();
		m_State.insert(std::make_pair(eState, _State));
	}

	void SnabyScript::AnimationUpdate()
	{
		SnabyStateScript::eState eCurState = m_CurState->GetStateType();

		if (m_PrevState == eCurState
			&& m_PrevDir == m_Dir)
			return;

		switch (eCurState)
		{
		case SnabyStateScript::eState::Idle:
		{
			switch (m_Dir)
			{
			case Lu::eDir::Left:
				m_Animator->PlayAnimation(L"Snbay_Idle_Left", true);
				break;
			case Lu::eDir::Right:
				m_Animator->PlayAnimation(L"Snbay_Idle_Right", true);
				break;
			case Lu::eDir::Up:
			case Lu::eDir::LeftUp:
			case Lu::eDir::RightUp:
				m_Animator->PlayAnimation(L"Snbay_Idle_Up", true);
				break;
			case Lu::eDir::Down:
			case Lu::eDir::LeftDown:
			case Lu::eDir::RightDown:
				m_Animator->PlayAnimation(L"Snbay_Idle_Down", true);
				break;
			}
		}
			break;
		case SnabyStateScript::eState::Patrol:
			break;
		case SnabyStateScript::eState::Attack:
			break;
		case SnabyStateScript::eState::Dead:
			m_Animator->PlayAnimation(L"Snbay_Dead", true);
			break;
		}
	}

	eDir SnabyScript::CalDirToPlayer()
	{
		Vector3 vPlayerPos = m_CurState->GetTarget()->GetOwner()->GetComponent<Transform>()->GetPosition();
		Vector3 vSnabyPos = GetOwner()->GetComponent<Transform>()->GetPosition();

		Vector3 vDir = vPlayerPos - vSnabyPos;
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

	void SnabyScript::CompleteAction()
	{
		ChangeState(SnabyStateScript::eState::Idle);
	}

	void SnabyScript::ChangeState(SnabyStateScript::eState _NextState)
	{
		SnabyStateScript* pNextState = GetStateScript(_NextState);

		if (pNextState->GetStateType() == m_CurState->GetStateType())
			return;

		m_CurState->Exit();
		m_PrevState = m_CurState->GetStateType();
		m_CurState = pNextState;
		m_CurState->Enter();
	}
}