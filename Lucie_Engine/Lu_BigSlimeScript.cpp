#include "Lu_BigSlimeScript.h"
#include "Lu_GameObject.h"
#include "Lu_Animator.h"
#include "Lu_Resources.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Time.h"
#include "Lu_Object.h"
#include "Lu_CircleProjectile.h"
#include "Lu_AudioSource.h"
#include "Lu_SoundManager.h"
#include "Lu_SceneManager.h"
#include "Lu_ProgressBarScript.h"

#include "Lu_BigSlimeIdleState.h"
#include "Lu_BigSlimeTraceState.h"
#include "Lu_BigSlimeAttackState.h"
#include "Lu_BigSlimeDeadState.h"

namespace Lu
{
	BigSlimeScript::BigSlimeScript()
		: m_CurState(nullptr)
		, m_PrevState(BigSlimeStateScript::eState::End)
		, m_HPFrame(nullptr)
		, m_HPBar(nullptr)
	{
		SetName(L"BigSlimeScript");

		GetInfo().HP = 100;
		GetInfo().MaxHP = 100;
		GetInfo().MoveSpeed = 100.f;
	}

	BigSlimeScript::~BigSlimeScript()
	{
		for (const auto& pair : m_State)
		{
			if (nullptr != pair.second)
				delete pair.second;
		}

		m_State.clear();
	}

	void BigSlimeScript::Initialize()
	{
		MonsterScript::Initialize();

		AddState(new BigSlimeIdleState);
		AddState(new BigSlimeTraceState);
		AddState(new BigSlimeAttackState);
		AddState(new BigSlimeDeadState);

		m_CurState = GetStateScript(BigSlimeStateScript::eState::Idle);
		m_CurState->Enter();
	}

	void BigSlimeScript::Update()
	{
		if (GetOwner()->IsDead())
			return;

		m_CurState->Update();
		AnimationUpdate();

		m_PrevState = m_CurState->GetStateType();
		SetPrevDir(GetCurDir());
	}

	void BigSlimeScript::OnCollisionEnter(Collider2D* _Other)
	{
		MonsterScript::OnCollisionEnter(_Other);

		m_HPBar->GetComponent<ProgressBarScript>()->SetCurValue(GetInfo().HP);
	}

	void BigSlimeScript::OnCollisionStay(Collider2D* _Other)
	{
		MonsterScript::OnCollisionStay(_Other);

		m_HPBar->GetComponent<ProgressBarScript>()->SetCurValue(GetInfo().HP);
	}

	void BigSlimeScript::CreateAnimation()
	{
		std::shared_ptr<Texture> pAtlas
			= Resources::Load<Texture>(L"BigSlimeSprite", L"..\\Resources\\Texture\\Monster\\MidBoss\\Slime.png");

		// Idle
		GetAnimator()->Create(L"BigSlime_Idle_Left", pAtlas, Vector2(0.f, 360.f), Vector2(360.f, 360.f), 2, Vector2(360.f, 360.f), Vector2::Zero, 0.3f);
		GetAnimator()->Create(L"BigSlime_Idle_Right", pAtlas, Vector2(0.f, 720.f), Vector2(360.f, 360.f), 2, Vector2(360.f, 360.f), Vector2::Zero, 0.3f);
		GetAnimator()->Create(L"BigSlime_Idle_Up", pAtlas, Vector2(0.f, 1080.f), Vector2(360.f, 360.f), 2, Vector2(360.f, 360.f), Vector2::Zero, 0.3f);
		GetAnimator()->Create(L"BigSlime_Idle_Down", pAtlas, Vector2(0.f, 0.f), Vector2(360.f, 360.f), 3, Vector2(360.f, 360.f), Vector2::Zero, 0.3f);

		// Move
		GetAnimator()->Create(L"BigSlime_Move_Left", pAtlas, Vector2(0.f, 1800.f), Vector2(360.f, 360.f), 3, Vector2(360.f, 360.f), Vector2::Zero, 0.2f);
		GetAnimator()->Create(L"BigSlime_Move_Right", pAtlas, Vector2(0.f, 2160.f), Vector2(360.f, 360.f), 3, Vector2(360.f, 360.f), Vector2::Zero, 0.2f);
		GetAnimator()->Create(L"BigSlime_Move_Up", pAtlas, Vector2(0.f, 2520.f), Vector2(360.f, 360.f), 3, Vector2(360.f, 360.f), Vector2::Zero, 0.2f);
		GetAnimator()->Create(L"BigSlime_Move_Down", pAtlas, Vector2(0.f, 1440.f), Vector2(360.f, 360.f), 3, Vector2(360.f, 360.f), Vector2::Zero, 0.2f);

		// Jump
		GetAnimator()->Create(L"BigSlime_Jump_Left", pAtlas, Vector2(0.f, 3240.f), Vector2(360.f, 360.f), 3, Vector2(360.f, 360.f), Vector2::Zero, 0.2f);
		GetAnimator()->StartEvent(L"BigSlime_Jump_Left") = std::bind(&BigSlimeScript::JumpSFX, this);
		GetAnimator()->CompleteEvent(L"BigSlime_Jump_Left") = std::bind(&BigSlimeScript::AttackSFX, this);

		GetAnimator()->Create(L"BigSlime_Jump_Right", pAtlas, Vector2(0.f, 3600.f), Vector2(360.f, 360.f), 3, Vector2(360.f, 360.f), Vector2::Zero, 0.2f);
		GetAnimator()->StartEvent(L"BigSlime_Jump_Right") = std::bind(&BigSlimeScript::JumpSFX, this);
		GetAnimator()->CompleteEvent(L"BigSlime_Jump_Right") = std::bind(&BigSlimeScript::AttackSFX, this);

		GetAnimator()->Create(L"BigSlime_Jump_Up", pAtlas, Vector2(0.f, 3960.f), Vector2(360.f, 360.f), 3, Vector2(360.f, 360.f), Vector2::Zero, 0.2f);
		GetAnimator()->StartEvent(L"BigSlime_Jump_Up") = std::bind(&BigSlimeScript::JumpSFX, this);
		GetAnimator()->CompleteEvent(L"BigSlime_Jump_Up") = std::bind(&BigSlimeScript::AttackSFX, this);

		GetAnimator()->Create(L"BigSlime_Jump_Down", pAtlas, Vector2(0.f, 2880.f), Vector2(360.f, 360.f), 3, Vector2(360.f, 360.f), Vector2::Zero, 0.2f);
		GetAnimator()->StartEvent(L"BigSlime_Jump_Down") = std::bind(&BigSlimeScript::JumpSFX, this);
		GetAnimator()->CompleteEvent(L"BigSlime_Jump_Down") = std::bind(&BigSlimeScript::AttackSFX, this);

		// Dead
		GetAnimator()->Create(L"BigSlime_Dead", pAtlas, Vector2(0.f, 7200.f), Vector2(360.f, 360.f), 1, Vector2(360.f, 360.f), Vector2::Zero, 1.f);
		GetAnimator()->StartEvent(L"BigSlime_Dead") = std::bind(&BigSlimeScript::DeadSFX, this);
		GetAnimator()->CompleteEvent(L"BigSlime_Dead") = std::bind(&BigSlimeScript::CompleteAction, this);
	}

	BigSlimeStateScript* BigSlimeScript::GetStateScript(BigSlimeStateScript::eState _State)
	{
		std::map<BigSlimeStateScript::eState, BigSlimeStateScript*>::iterator iter = m_State.find(_State);

		if (iter == m_State.end())
			return nullptr;

		return iter->second;
	}

	void BigSlimeScript::AddState(BigSlimeStateScript* _State)
	{
		if (nullptr == _State)
			return;

		_State->SetBigSlimeScript(this);
		_State->Initialize();
		BigSlimeStateScript::eState eState = _State->GetStateType();
		m_State.insert(std::make_pair(eState, _State));
	}

	void BigSlimeScript::CompleteAction()
	{
		ChangeState(BigSlimeStateScript::eState::Trace);
	}

	void BigSlimeScript::JumpSFX()
	{	
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"SlimeJumpSFX", L"..\\Resources\\Sound\\SFX\\Monster\\Slime\\SlimeJumpSFX.ogg"));
		pSFX->Play();
	}

	void BigSlimeScript::AttackSFX()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"SlimeBubbleSFX", L"..\\Resources\\Sound\\SFX\\Monster\\Slime\\SlimeBubbleSFX.ogg"));
		pSFX->Play();

		ChangeState(BigSlimeStateScript::eState::Trace);
	}

	void BigSlimeScript::DeadSFX()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"SlimeDeadSFX", L"..\\Resources\\Sound\\SFX\\Monster\\Slime\\SlimeDeadSFX.ogg"));
		pSFX->Play();
	}

	void BigSlimeScript::AnimationUpdate()
	{
		BigSlimeStateScript::eState eCurState = m_CurState->GetStateType();
		eAnimDir eCurDir = GetCurDir();

		if (m_PrevState == eCurState
			&& GetPrevDir() == eCurDir)
			return;

		switch (eCurState)
		{
		case BigSlimeStateScript::eState::Idle:
		{
			switch (eCurDir)
			{
			case Lu::MonsterScript::eAnimDir::Left:
				GetAnimator()->PlayAnimation(L"BigSlime_Idle_Left", true);
				break;
			case Lu::MonsterScript::eAnimDir::Right:
				GetAnimator()->PlayAnimation(L"BigSlime_Idle_Right", true);
				break;
			case Lu::MonsterScript::eAnimDir::Up:
				GetAnimator()->PlayAnimation(L"BigSlime_Idle_Up", true);
				break;
			case Lu::MonsterScript::eAnimDir::Down:
				GetAnimator()->PlayAnimation(L"BigSlime_Idle_Down", true);
				break;
			default:
				break;
			}
		}
		break;
		case BigSlimeStateScript::eState::Trace:
		{
			switch (eCurDir)
			{
			case Lu::MonsterScript::eAnimDir::Left:
				GetAnimator()->PlayAnimation(L"BigSlime_Move_Left", true);
				break;
			case Lu::MonsterScript::eAnimDir::Right:
				GetAnimator()->PlayAnimation(L"BigSlime_Move_Right", true);
				break;
			case Lu::MonsterScript::eAnimDir::Up:
				GetAnimator()->PlayAnimation(L"BigSlime_Move_Up", true);
				break;
			case Lu::MonsterScript::eAnimDir::Down:
				GetAnimator()->PlayAnimation(L"BigSlime_Move_Down", true);
				break;
			default:
				break;
			}
		}
		break;
		case BigSlimeStateScript::eState::Attack:
		{
			switch (eCurDir)
			{
			case Lu::MonsterScript::eAnimDir::Left:
				GetAnimator()->PlayAnimation(L"BigSlime_Jump_Left", true);
				break;
			case Lu::MonsterScript::eAnimDir::Right:
				GetAnimator()->PlayAnimation(L"BigSlime_Jump_Right", true);
				break;
			case Lu::MonsterScript::eAnimDir::Up:
				GetAnimator()->PlayAnimation(L"BigSlime_Jump_Up", true);
				break;
			case Lu::MonsterScript::eAnimDir::Down:
				GetAnimator()->PlayAnimation(L"BigSlime_Jump_Down", true);
				break;
			default:
				break;
			}
		}
		break;
		case BigSlimeStateScript::eState::Dead:
			GetAnimator()->PlayAnimation(L"BigSlime_Dead", true);
			break;
		}
	}

	void BigSlimeScript::ChangeIdleState()
	{
		ChangeState(BigSlimeStateScript::eState::Idle);
	}

	void BigSlimeScript::ChangeDeadState()
	{
		ChangeState(BigSlimeStateScript::eState::Dead);
	}

	void BigSlimeScript::ChangeState(BigSlimeStateScript::eState _NextState)
	{
		BigSlimeStateScript* pNextState = GetStateScript(_NextState);

		if (pNextState->GetStateType() == m_CurState->GetStateType())
			return;

		m_CurState->Exit();
		m_PrevState = m_CurState->GetStateType();
		m_CurState = pNextState;
		m_CurState->Enter();
	}
}