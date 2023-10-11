#include "Lu_EntScript.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_ProgressBarScript.h"
#include "Lu_SoundManager.h"
#include "Lu_AudioSource.h"
#include "Lu_Input.h"

#include "Lu_EntIdleState.h"
#include "Lu_ThornState.h"
#include "Lu_PoisonBreathState.h"
#include "Lu_WindBreathState.h"
#include "Lu_TomatoBoomState.h"
#include "Lu_CraterState.h"
#include "Lu_EntDeadState.h"

namespace Lu
{
	EntScript::EntScript()
		: m_CurState(nullptr)
		, m_PrevState(EntStateScript::eState::End)
		, m_HPFrame(nullptr)
		, m_HPBar(nullptr)
		, m_CurPhase(ePhase::Phase_1)
		, m_CurAttack(eAttackType::End)
		, m_bWakeUp(false)
		, m_bHowling(false)
		, m_bRandomAttack(true)
		, m_AttackNumber(-1)
		, m_bPatternStart(false)
	{
		SetName(L"EntScript");

		GetInfo().HP = 200;
		GetInfo().MaxHP = 200;
	}

	EntScript::~EntScript()
	{
		for (const auto& pair : m_State)
		{
			if (nullptr != pair.second)
				delete pair.second;
		}

		m_State.clear();
	}

	void EntScript::Initialize()
	{
		MonsterScript::Initialize();

		GetAnimator()->PlayAnimation(L"Ent_Sleep", true);

		// 보스 HP
		{
			m_HPFrame = object::Instantiate<GameObject>(Vector3(0.f, 350.f, 100.f), Vector3(759.f, 48.f, 100.f), eLayerType::UI);
			m_HPFrame->SetName(L"Ent_HPFrame");

			MeshRenderer* pMeshRender = m_HPFrame->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"BossHPFrame_Mtrl"));
		}

		{
			m_HPBar = object::Instantiate<GameObject>(Vector3(0.f, 350.f, 100.f), Vector3(720.f, 30.f, 100.f), eLayerType::UI);
			m_HPBar->SetName(L"Ent_HPBar");

			MeshRenderer* pMeshRender = m_HPBar->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"Ent_BossHP_Mtrl"));

			ProgressBarScript* pHPBarScript = m_HPBar->AddComponent<ProgressBarScript>();
			pHPBarScript->SetTransform(m_HPBar->GetComponent<Transform>());
			pHPBarScript->SetMeshRender(pMeshRender);
			pHPBarScript->SetMaxValue(GetInfo().MaxHP);
			pHPBarScript->SetCurValue(GetInfo().HP);
			pHPBarScript->SetBossName(L"이그드라실");
			pHPBarScript->SetTextPrint(true);
		}

		// 상태
		AddState(new EntIdleState);
		AddState(new ThornState);
		AddState(new PoisonBreathState);
		AddState(new WindBreathState);
		AddState(new TomatoBoomState);
		AddState(new CraterState);
		AddState(new EntDeadState);

		m_CurState = GetStateScript(EntStateScript::eState::Idle);
		m_CurState->Enter();
	}

	void EntScript::Update()
	{
		if (GetOwner()->IsDead())
			return;

		if(m_bPatternStart)
			m_CurState->Update();
		AnimationUpdate();

		m_PrevState = m_CurState->GetStateType();
		SetPrevDir(GetCurDir());
	}

	void EntScript::OnCollisionEnter(Collider2D* _Other)
	{
		MonsterScript::OnCollisionEnter(_Other);

		m_HPBar->GetComponent<ProgressBarScript>()->SetCurValue(GetInfo().HP);

		if (m_CurPhase == ePhase::Phase_1
			&& GetInfo().HP / GetInfo().MaxHP <= 0.25f)
		{
			m_CurPhase = ePhase::Phase_2;
		}
	}

	void EntScript::OnCollisionStay(Collider2D* _Other)
	{
		MonsterScript::OnCollisionStay(_Other);

		m_HPBar->GetComponent<ProgressBarScript>()->SetCurValue(GetInfo().HP);
	
		if (m_CurPhase == ePhase::Phase_1
			&& GetInfo().HP / GetInfo().MaxHP <= 0.25f)
		{
			m_CurPhase = ePhase::Phase_2;
		}
	}

	EntStateScript* EntScript::GetStateScript(EntStateScript::eState _State)
	{
		std::map<EntStateScript::eState, EntStateScript*>::iterator iter = m_State.find(_State);

		if (iter == m_State.end())
			return nullptr;

		return iter->second;
	}

	void EntScript::AddState(EntStateScript* _State)
	{
		if (nullptr == _State)
			return;

		_State->SetEntScript(this);
		_State->Initialize();
		EntStateScript::eState eState = _State->GetStateType();
		m_State.insert(std::make_pair(eState, _State));
	}

	void EntScript::CompleteAction()
	{
		ChangeState(EntStateScript::eState::Idle);

		if(!m_bWakeUp)
			m_bWakeUp = true;

		if (m_bHowling)
			m_bHowling = false;
	}

	void EntScript::PatternStart()
	{
		if (!m_bPatternStart)
			m_bPatternStart = true;
	}

	void EntScript::HowlingSFX()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"EntHowlingSFX", L"..\\Resources\\Sound\\SFX\\Monster\\Ent\\EntHowlingSFX.ogg"));
		pSFX->Play();
	}

	void EntScript::AttackSFX()
	{
	}

	void EntScript::DeadSFX()
	{
		AudioSource* pBGM = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetBGM();
		pBGM->Stop();

		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"EntDeadSFX", L"..\\Resources\\Sound\\SFX\\Monster\\Ent\\EntDeadSFX.ogg"));
		pSFX->Play();
	}

	void EntScript::Cheat()
	{
		if (Input::GetKeyDown(eKeyCode::O))
		{
			if (m_bRandomAttack)
				m_bRandomAttack = false;
			else
				m_bRandomAttack = true;
		}

		if (!m_bRandomAttack)
		{
			if (Input::GetKeyDown(eKeyCode::_5))
				m_AttackNumber = (int)eAttackType::Thorn;

			else if (Input::GetKeyDown(eKeyCode::_6))
				m_AttackNumber = (int)eAttackType::PoisonBreath;

			else if (Input::GetKeyDown(eKeyCode::_7))
				m_AttackNumber = (int)eAttackType::WindBreath;

			else if (Input::GetKeyDown(eKeyCode::_8))
				m_AttackNumber = (int)eAttackType::TomatoBoom;

			if (Input::GetKeyDown(eKeyCode::_9))
				m_AttackNumber = (int)eAttackType::Crater;
		}
	}

	void EntScript::CreateAnimation()
	{
		std::shared_ptr<Texture> pAtlas
			= Resources::Load<Texture>(L"EntSprite", L"..\\Resources\\Texture\\Monster\\Boss\\Boss_Ent.png");

		// Idle
		GetAnimator()->Create(L"Ent_Phase1_Idle", pAtlas, Vector2(0.f, 350.f), Vector2(322.f, 350.f), 3, Vector2(322.f, 350.f), Vector2::Zero, 0.5f);
		GetAnimator()->Create(L"Ent_Phase2_Idle", pAtlas, Vector2(0.f, 350.f), Vector2(322.f, 350.f), 3, Vector2(322.f, 350.f), Vector2::Zero, 0.5f);

		// Sleep
		GetAnimator()->Create(L"Ent_Sleep", pAtlas, Vector2(0.f, 0.f), Vector2(322.f, 350.f), 1, Vector2(322.f, 350.f));

		// WakeUp
		GetAnimator()->Create(L"Ent_WakeUp", pAtlas, Vector2(0.f, 0.f), Vector2(322.f, 350.f), 3, Vector2(322.f, 350.f), Vector2::Zero, 0.3f);
		GetAnimator()->CompleteEvent(L"Ent_WakeUp") = std::bind(&EntScript::CompleteAction, this);

		// Howling & Attack - Crater
		GetAnimator()->Create(L"Ent_Howling", pAtlas, Vector2(0.f, 700.f), Vector2(322.f, 350.f), 3, Vector2(322.f, 350.f), Vector2::Zero, 0.2f);
		GetAnimator()->StartEvent(L"Ent_Howling") = std::bind(&EntScript::HowlingSFX, this);
		GetAnimator()->CompleteEvent(L"Ent_Howling") = std::bind(&EntScript::CompleteAction, this);


		// 토마토 빼고 모든 공격과 공격 사이에 눈 질끈 감았다가 뜸
		// Attack - WindBreath
		GetAnimator()->Create(L"Ent_Attack_WindBreath_Phase1", pAtlas, Vector2(644.f, 1050.f), Vector2(322.f, 350.f), 1, Vector2(322.f, 350.f));
		GetAnimator()->Create(L"Ent_Attack_WindBreath_Phase2", pAtlas, Vector2(2898.f, 1050.f), Vector2(322.f, 350.f), 1, Vector2(322.f, 350.f));

		// Attack - TomatoBoom (2번째 프레임에서 멈춰있어야 함.. 끝나면 다시 반대로 출력해야함..)
		// 반복 false로 재생했을 때 마지막 프레임에서 안멈추면 2번째 프레임만 있는 애니메이션 만들어야함
		// 이거에다가 차징 = 가시 공격
		GetAnimator()->Create(L"Ent_Attack_TomatoBoom_Phase1_Start", pAtlas, Vector2(0.f, 700.f), Vector2(322.f, 350.f), 2, Vector2(322.f, 350.f), Vector2::Zero, 0.3f);
		GetAnimator()->Create(L"Ent_Attack_TomatoBoom_Phase2_Start", pAtlas, Vector2(0.f, 2080.f), Vector2(322.f, 350.f), 2, Vector2(322.f, 350.f), Vector2::Zero, 0.3f);
		GetAnimator()->Create(L"Ent_Attack_TomatoBoom_Phase1_End", pAtlas, Vector2(0.f, 700.f), Vector2(322.f, 350.f), 2, Vector2(322.f, 350.f), Vector2::Zero, 0.3f, true);
		GetAnimator()->Create(L"Ent_Attack_TomatoBoom_Phase2_End", pAtlas, Vector2(0.f, 2080.f), Vector2(322.f, 350.f), 2, Vector2(322.f, 350.f), Vector2::Zero, 0.3f, true);

		// Attack - PoisonBreath (왼->오 혹은 오->왼 다 출력해야함..)
		GetAnimator()->Create(L"Ent_Attack_PoisonBreath_Phase1_Left", pAtlas, Vector2(0.f, 1050.f), Vector2(322.f, 350.f), 3, Vector2(322.f, 350.f), Vector2::Zero, 0.5f);
		GetAnimator()->Create(L"Ent_Attack_PoisonBreath_Phase1_Right", pAtlas, Vector2(0.f, 1050.f), Vector2(322.f, 350.f), 3, Vector2(322.f, 350.f), Vector2::Zero, 0.5f, true);
		GetAnimator()->Create(L"Ent_Attack_PoisonBreath_Phase2_Left", pAtlas, Vector2(0.f, 3150.f), Vector2(322.f, 350.f), 3, Vector2(322.f, 350.f), Vector2::Zero, 0.5f);
		GetAnimator()->Create(L"Ent_Attack_PoisonBreath_Phase2_Right", pAtlas, Vector2(0.f, 3150.f), Vector2(322.f, 350.f), 3, Vector2(322.f, 350.f), Vector2::Zero, 0.5f, true);


		// Dead
		GetAnimator()->Create(L"Ent_Dead", pAtlas, Vector2(0.f, 2800.f), Vector2(322.f, 350.f), 2, Vector2(322.f, 350.f), Vector2::Zero, 1.f);
		GetAnimator()->StartEvent(L"Ent_Dead") = std::bind(&EntScript::DeadSFX, this);
		GetAnimator()->CompleteEvent(L"Ent_Dead") = std::bind(&EntScript::CompleteAction, this);
	}

	void EntScript::AnimationUpdate()
	{
		EntStateScript::eState eCurState = m_CurState->GetStateType();

		if (!m_bWakeUp || m_bHowling)
			return;

		switch (eCurState)
		{
		case EntStateScript::eState::Idle:
		{
			switch (m_CurPhase)
			{
			case Lu::EntScript::ePhase::Phase_1:
				GetAnimator()->PlayAnimation(L"Ent_Phase1_Idle", true);
				break;
			case Lu::EntScript::ePhase::Phase_2:
				GetAnimator()->PlayAnimation(L"Ent_Phase2_Idle", true);
				break;
			case Lu::EntScript::ePhase::End:
				break;
			default:
				break;
			}
		}
			break;
		case EntStateScript::eState::Dead:
			GetAnimator()->PlayAnimation(L"Ent_Dead", true);
			break;
		}
	}

	void EntScript::ChangeIdleState()
	{
		ChangeState(EntStateScript::eState::Idle);
	}

	void EntScript::ChangeDeadState()
	{
		ChangeState(EntStateScript::eState::Dead);
	}

	void EntScript::ChangeState(EntStateScript::eState _NextState)
	{
		EntStateScript* pNextState = GetStateScript(_NextState);

		if (pNextState->GetStateType() == m_CurState->GetStateType())
			return;

		m_CurState->Exit();
		m_PrevState = m_CurState->GetStateType();
		m_CurState = pNextState;
		m_CurState->Enter();
	}

	void EntScript::WakeUp()
	{
		if(!m_bWakeUp)
			GetAnimator()->PlayAnimation(L"Ent_WakeUp", true);
	}

	void EntScript::Howling()
	{
		if (!m_bHowling)
		{
			m_bHowling = true;
			GetAnimator()->PlayAnimation(L"Ent_Howling", true);
		}
	}
}