#include "Lu_SnabyScript.h"
#include "Lu_GameObject.h"
#include "Lu_Animator.h"
#include "Lu_Resources.h"
#include "Lu_PlayerScript.h"
#include "Lu_AudioSource.h"
#include "Lu_SoundManager.h"
#include "Lu_SceneManager.h"

#include "Lu_SnabyIdleState.h"
#include "Lu_SnabyPatrolState.h"
#include "Lu_SnabyAttackState.h"
#include "Lu_SnabyDeadState.h"

namespace Lu
{
	SnabyScript::SnabyScript()
		: m_CurState(nullptr)
		, m_PrevState(SnabyStateScript::eState::End)
		, m_Target(nullptr)
	{
		SetName(L"SnabyScript");

		GetInfo().HP = 30;
		GetInfo().MaxHP = 30;
		GetInfo().DetectRange = 200.f;
		GetInfo().MoveSpeed = 100.f;
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
		MonsterScript::Initialize();

		AddState(new SnabyIdleState);
		AddState(new SnabyPatrolState);
		AddState(new SnabyAttackState);
		AddState(new SnabyDeadState);

		m_CurState = GetStateScript(SnabyStateScript::eState::Idle);
		m_CurState->Enter();
	}

	void SnabyScript::Update()
	{
		MonsterScript::Update();

		if (GetOwner()->IsDead())
			return;

		m_CurState->Update();
		AnimationUpdate();

		m_PrevState = m_CurState->GetStateType();
		SetPrevDir(GetCurDir());
	}

	void SnabyScript::OnCollisionEnter(Collider2D* _Other)
	{
		if ((int)eLayerType::Immovable == _Other->GetOwner()->GetLayerIndex())
			ChangeState(SnabyStateScript::eState::Idle);

		if ((int)eLayerType::PlayerProjectile == _Other->GetOwner()->GetLayerIndex())
		{
			if (SnabyStateScript::eState::Dead == m_CurState->GetStateType())
				return;

			GetInfo().HP -= m_Target->GetPlayerDamage();

			if (GetInfo().HP <= 0.f)
			{
				ChangeState(SnabyStateScript::eState::Dead);
			}
		}
	}

	void SnabyScript::OnCollisionStay(Collider2D* _Other)
	{
		if ((int)eLayerType::Immovable == _Other->GetOwner()->GetLayerIndex())
			ChangeState(SnabyStateScript::eState::Idle);

		if ((int)eLayerType::PlayerProjectile == _Other->GetOwner()->GetLayerIndex())
		{
			if (SnabyStateScript::eState::Dead == m_CurState->GetStateType())
				return;

			GetInfo().HP -= m_Target->GetPlayerDamage();

			if (GetInfo().HP <= 0.f)
			{
				ChangeState(SnabyStateScript::eState::Dead);
			}
		}
	}

	void SnabyScript::OnCollisionExit(Collider2D* _Other)
	{
	}

	void SnabyScript::CreateAnimation()
	{
		std::shared_ptr<Texture> pAtlas
			= Resources::Load<Texture>(L"SnabySprite", L"..\\Resources\\Texture\\Monster\\Normal\\Snaby.png");

		// Idle
		GetAnimator()->Create(L"Snaby_Idle_Left", pAtlas, Vector2(0.f, 432.f), Vector2(48.f, 48.f), 2, Vector2(48.f, 48.f), Vector2::Zero, 0.5f);
		GetAnimator()->Create(L"Snaby_Idle_Right", pAtlas, Vector2(0.f, 480.f), Vector2(48.f, 48.f), 2, Vector2(48.f, 48.f), Vector2::Zero, 0.5f);
		GetAnimator()->Create(L"Snaby_Idle_Up", pAtlas, Vector2(0.f, 528.f), Vector2(48.f, 48.f), 2, Vector2(48.f, 48.f), Vector2::Zero, 0.5f);
		GetAnimator()->Create(L"Snaby_Idle_Down", pAtlas, Vector2(0.f, 384.f), Vector2(48.f, 48.f), 2, Vector2(48.f, 48.f), Vector2::Zero, 0.5f);
	
		// Move
		GetAnimator()->Create(L"Snaby_Move_Left", pAtlas, Vector2(0.f, 48.f), Vector2(48.f, 48.f), 3, Vector2(48.f, 48.f), Vector2::Zero, 0.3f);
		GetAnimator()->Create(L"Snaby_Move_Right", pAtlas, Vector2(0.f, 96.f), Vector2(48.f, 48.f), 3, Vector2(48.f, 48.f), Vector2::Zero, 0.3f);
		GetAnimator()->Create(L"Snaby_Move_Up", pAtlas, Vector2(0.f, 144.f), Vector2(48.f, 48.f), 3, Vector2(48.f, 48.f), Vector2::Zero, 0.3f);
		GetAnimator()->Create(L"Snaby_Move_Down", pAtlas, Vector2(0.f, 0.f), Vector2(48.f, 48.f), 3, Vector2(48.f, 48.f), Vector2::Zero, 0.3f);

		// Attack
		GetAnimator()->Create(L"Snaby_Attack_Left", pAtlas, Vector2(0.f, 240.f), Vector2(48.f, 48.f), 3, Vector2(48.f, 48.f), Vector2::Zero, 0.3f);
		GetAnimator()->StartEvent(L"Snaby_Attack_Left") = std::bind(&SnabyScript::AttackSFX, this);

		GetAnimator()->Create(L"Snaby_Attack_Right", pAtlas, Vector2(0.f, 288.f), Vector2(48.f, 48.f), 3, Vector2(48.f, 48.f), Vector2::Zero, 0.3f);
		GetAnimator()->StartEvent(L"Snaby_Attack_Right") = std::bind(&SnabyScript::AttackSFX, this);

		GetAnimator()->Create(L"Snaby_Attack_Up", pAtlas, Vector2(0.f, 336.f), Vector2(48.f, 48.f), 3, Vector2(48.f, 48.f), Vector2::Zero, 0.3f);
		GetAnimator()->StartEvent(L"Snaby_Attack_Up") = std::bind(&SnabyScript::AttackSFX, this);

		GetAnimator()->Create(L"Snaby_Attack_Down", pAtlas, Vector2(0.f, 192.f), Vector2(48.f, 48.f), 3, Vector2(48.f, 48.f), Vector2::Zero, 0.3f);
		GetAnimator()->StartEvent(L"Snaby_Attack_Down") = std::bind(&SnabyScript::AttackSFX, this);

		// Dead
		GetAnimator()->Create(L"Snaby_Dead", pAtlas, Vector2(0.f, 576.f), Vector2(48.f, 48.f), 1, Vector2(48.f, 48.f), Vector2::Zero, 1.f);
		GetAnimator()->StartEvent(L"Snaby_Dead") = std::bind(&SnabyScript::DeadSFX, this);
		GetAnimator()->CompleteEvent(L"Snaby_Dead") = std::bind(&SnabyScript::CompleteAction, this);
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

	void SnabyScript::CompleteAction()
	{
		ChangeState(SnabyStateScript::eState::Idle);
	}

	void SnabyScript::AttackSFX()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"SnabyAttackSFX", L"..\\Resources\\Sound\\SFX\\Monster\\Snaby\\SnabyAttackSFX.ogg"));
		pSFX->Play();
	}

	void SnabyScript::DeadSFX()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"SnabyDeadSFX", L"..\\Resources\\Sound\\SFX\\Monster\\Snaby\\SnabyDeadSFX.ogg"));
		pSFX->Play();
	}

	void SnabyScript::AnimationUpdate()
	{
		SnabyStateScript::eState eCurState = m_CurState->GetStateType();
		eAnimDir eCurDir = GetCurDir();

		if (m_PrevState == eCurState
			&& GetPrevDir() == eCurDir)
			return;

		switch (eCurState)
		{
		case SnabyStateScript::eState::Idle:
		{
			switch (eCurDir)
			{
			case Lu::MonsterScript::eAnimDir::Left:
				GetAnimator()->PlayAnimation(L"Snaby_Idle_Left", true);
				break;
			case Lu::MonsterScript::eAnimDir::Right:
				GetAnimator()->PlayAnimation(L"Snaby_Idle_Right", true);
				break;
			case Lu::MonsterScript::eAnimDir::Up:
				GetAnimator()->PlayAnimation(L"Snaby_Idle_Up", true);
				break;
			case Lu::MonsterScript::eAnimDir::Down:
				GetAnimator()->PlayAnimation(L"Snaby_Idle_Down", true);
				break;
			default:
				break;
			}
		}
			break;
		case SnabyStateScript::eState::Patrol:
		{
			switch (eCurDir)
			{
			case Lu::MonsterScript::eAnimDir::Left:
				GetAnimator()->PlayAnimation(L"Snaby_Move_Left", true);
				break;
			case Lu::MonsterScript::eAnimDir::Right:
				GetAnimator()->PlayAnimation(L"Snaby_Move_Right", true);
				break;
			case Lu::MonsterScript::eAnimDir::Up:
				GetAnimator()->PlayAnimation(L"Snaby_Move_Up", true);
				break;
			case Lu::MonsterScript::eAnimDir::Down:
				GetAnimator()->PlayAnimation(L"Snaby_Move_Down", true);
				break;
			default:
				break;
			}
		}
			break;
		case SnabyStateScript::eState::Attack:
		{
			switch (eCurDir)
			{
			case Lu::MonsterScript::eAnimDir::Left:
				GetAnimator()->PlayAnimation(L"Snaby_Attack_Left", true);
				break;
			case Lu::MonsterScript::eAnimDir::Right:
				GetAnimator()->PlayAnimation(L"Snaby_Attack_Right", true);
				break;
			case Lu::MonsterScript::eAnimDir::Up:
				GetAnimator()->PlayAnimation(L"Snaby_Attack_Up", true);
				break;
			case Lu::MonsterScript::eAnimDir::Down:
				GetAnimator()->PlayAnimation(L"Snaby_Attack_Down", true);
				break;
			default:
				break;
			}
		}
			break;
		case SnabyStateScript::eState::Dead:
			GetAnimator()->PlayAnimation(L"Snaby_Dead", true);
			break;
		}
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