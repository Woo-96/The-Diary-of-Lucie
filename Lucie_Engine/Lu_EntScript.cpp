#include "Lu_EntScript.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_ProgressBarScript.h"
#include "Lu_SoundManager.h"
#include "Lu_AudioSource.h"
#include "Lu_Input.h"
#include "Lu_Renderer.h"
#include "Lu_CameraScript.h"

#include "Lu_EntIdleState.h"
#include "Lu_ThornState.h"
#include "Lu_PoisonBreathState.h"
#include "Lu_WindBreathState.h"
#include "Lu_TomatoBombState.h"
#include "Lu_CraterState.h"
#include "Lu_EntRageState.h"
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
			m_HPFrame->SetActive(false);

			MeshRenderer* pMeshRender = m_HPFrame->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"BossHPFrame_Mtrl"));
		}

		{
			m_HPBar = object::Instantiate<GameObject>(Vector3(0.f, 350.f, 100.f), Vector3(720.f, 30.f, 100.f), eLayerType::UI);
			m_HPBar->SetName(L"Ent_HPBar");
			m_HPBar->SetActive(false);

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
		AddState(new TomatoBombState);
		AddState(new CraterState);
		AddState(new EntRageState);
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

		float fCurHP = GetInfo().HP;
		float fMaxHP = GetInfo().MaxHP;
		float fHPPer = (fCurHP / fMaxHP) * 100.0f;
		if (m_CurPhase == ePhase::Phase_1 && fHPPer < 25.f)
		{
			m_CurPhase = ePhase::Phase_2;
			ChangeState(EntStateScript::eState::Rage);
		}
	}

	void EntScript::OnCollisionStay(Collider2D* _Other)
	{
		MonsterScript::OnCollisionStay(_Other);

		m_HPBar->GetComponent<ProgressBarScript>()->SetCurValue(GetInfo().HP);
	
		float fCurHP = GetInfo().HP;
		float fMaxHP = GetInfo().MaxHP;
		float fHPPer = (fCurHP / fMaxHP) * 100.0f;
		if (m_CurPhase == ePhase::Phase_1 && fHPPer < 25.f)
		{
			m_CurPhase = ePhase::Phase_2;
			ChangeState(EntStateScript::eState::Rage);
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

		if (m_HPFrame)
			m_HPFrame->SetActive(true);

		if (m_HPBar)
			m_HPBar->SetActive(true);
	}

	void EntScript::HowlingSFX()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"EntHowlingSFX", L"..\\Resources\\Sound\\SFX\\Monster\\Ent\\EntHowlingSFX.ogg"));
		pSFX->Play();

		// 카메라 쉐이킹
		renderer::mainCamera->GetOwner()->GetComponent<CameraScript>()->RequestCameraShaking(10.f, 1.5f);
	}

	void EntScript::AttackPoisonBreathSFX()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"EntPoisonBreathSFX", L"..\\Resources\\Sound\\SFX\\Monster\\Ent\\EntPoisonBreathSFX.ogg"));
		pSFX->Play();
	}

	void EntScript::AttackWindBreathSFX()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"EntWindBreathSFX", L"..\\Resources\\Sound\\SFX\\Monster\\Ent\\EntWindBreathSFX.ogg"));
		pSFX->Play();
	}

	void EntScript::RageSFX()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"EntRageSFX", L"..\\Resources\\Sound\\SFX\\Monster\\Ent\\EntRageSFX.ogg"));
		pSFX->Play();
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
				m_AttackNumber = (int)eAttackType::TomatoBomb;

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
		GetAnimator()->Create(L"Ent_Phase2_Idle", pAtlas, Vector2(0.f, 2100.f), Vector2(322.f, 350.f), 3, Vector2(322.f, 350.f), Vector2::Zero, 0.5f);

		// Sleep
		GetAnimator()->Create(L"Ent_Sleep", pAtlas, Vector2(0.f, 0.f), Vector2(322.f, 350.f), 1, Vector2(322.f, 350.f));

		// WakeUp
		GetAnimator()->Create(L"Ent_WakeUp", pAtlas, Vector2(0.f, 0.f), Vector2(322.f, 350.f), 3, Vector2(322.f, 350.f), Vector2::Zero, 0.3f);
		GetAnimator()->CompleteEvent(L"Ent_WakeUp") = std::bind(&EntScript::CompleteAction, this);

		// Howling & Attack - Crater
		GetAnimator()->Create(L"Ent_Howling", pAtlas, Vector2(0.f, 700.f), Vector2(322.f, 350.f), 3, Vector2(322.f, 350.f), Vector2::Zero, 0.2f);
		GetAnimator()->StartEvent(L"Ent_Howling") = std::bind(&EntScript::HowlingSFX, this);
		GetAnimator()->CompleteEvent(L"Ent_Howling") = std::bind(&EntScript::CompleteAction, this);

		GetAnimator()->Create(L"Ent_Howling_Phase2", pAtlas, Vector2(0.f, 2800.f), Vector2(322.f, 350.f), 3, Vector2(322.f, 350.f), Vector2::Zero, 0.2f);
		GetAnimator()->StartEvent(L"Ent_Howling_Phase2") = std::bind(&EntScript::HowlingSFX, this);
		GetAnimator()->CompleteEvent(L"Ent_Howling_Phase2") = std::bind(&EntScript::CompleteAction, this);


		// Attack - TomatoBomb
		GetAnimator()->Create(L"Ent_Attack_TomatoBomb_Phase1_Start", pAtlas, Vector2(0.f, 700.f), Vector2(322.f, 350.f), 2, Vector2(322.f, 350.f), Vector2::Zero, 0.3f);
		GetAnimator()->Create(L"Ent_Attack_TomatoBomb_Phase2_Start", pAtlas, Vector2(0.f, 2800.f), Vector2(322.f, 350.f), 2, Vector2(322.f, 350.f), Vector2::Zero, 0.3f);
		GetAnimator()->Create(L"Ent_Attack_TomatoBomb_Phase1_End", pAtlas, Vector2(966.f, 700.f), Vector2(322.f, 350.f), 2, Vector2(322.f, 350.f), Vector2::Zero, 0.4f);
		GetAnimator()->CompleteEvent(L"Ent_Attack_TomatoBomb_Phase1_End") = std::bind(&EntScript::CompleteAction, this);
		GetAnimator()->Create(L"Ent_Attack_TomatoBomb_Phase2_End", pAtlas, Vector2(966.f, 2800.f), Vector2(322.f, 350.f), 2, Vector2(322.f, 350.f), Vector2::Zero, 0.4f);
		GetAnimator()->CompleteEvent(L"Ent_Attack_TomatoBomb_Phase2_End") = std::bind(&EntScript::CompleteAction, this);


		// Attack Change & Attack - Thorn
		GetAnimator()->Create(L"Ent_Attack_Change_Phase1", pAtlas, Vector2(0.f, 700.f), Vector2(322.f, 350.f), 2, Vector2(322.f, 350.f), Vector2::Zero, 0.2f);
		GetAnimator()->Create(L"Ent_Attack_Change_Phase2", pAtlas, Vector2(0.f, 2800.f), Vector2(322.f, 350.f), 2, Vector2(322.f, 350.f), Vector2::Zero, 0.2f);


		// Attack - PoisonBreath
		GetAnimator()->Create(L"Ent_Attack_PoisonBreath_Phase1_Left", pAtlas, Vector2(0.f, 1050.f), Vector2(322.f, 350.f), 5, Vector2(322.f, 350.f), Vector2::Zero, 1.2f);
		GetAnimator()->StartEvent(L"Ent_Attack_PoisonBreath_Phase1_Left") = std::bind(&EntScript::AttackPoisonBreathSFX, this);
		GetAnimator()->CompleteEvent(L"Ent_Attack_PoisonBreath_Phase1_Left") = std::bind(&EntScript::CompleteAction, this);
		GetAnimator()->Create(L"Ent_Attack_PoisonBreath_Phase1_Right", pAtlas, Vector2(0.f, 1400.f), Vector2(322.f, 350.f), 5, Vector2(322.f, 350.f), Vector2::Zero, 1.2f);
		GetAnimator()->StartEvent(L"Ent_Attack_PoisonBreath_Phase1_Right") = std::bind(&EntScript::AttackPoisonBreathSFX, this);
		GetAnimator()->CompleteEvent(L"Ent_Attack_PoisonBreath_Phase1_Right") = std::bind(&EntScript::CompleteAction, this);
		GetAnimator()->Create(L"Ent_Attack_PoisonBreath_Phase2_Left", pAtlas, Vector2(0.f, 3150.f), Vector2(322.f, 350.f), 5, Vector2(322.f, 350.f), Vector2::Zero, 1.2f);
		GetAnimator()->StartEvent(L"Ent_Attack_PoisonBreath_Phase2_Left") = std::bind(&EntScript::AttackPoisonBreathSFX, this);
		GetAnimator()->CompleteEvent(L"Ent_Attack_PoisonBreath_Phase2_Left") = std::bind(&EntScript::CompleteAction, this);
		GetAnimator()->Create(L"Ent_Attack_PoisonBreath_Phase2_Right", pAtlas, Vector2(0.f, 3500.f), Vector2(322.f, 350.f), 5, Vector2(322.f, 350.f), Vector2::Zero, 1.2f);
		GetAnimator()->StartEvent(L"Ent_Attack_PoisonBreath_Phase2_Right") = std::bind(&EntScript::AttackPoisonBreathSFX, this);
		GetAnimator()->CompleteEvent(L"Ent_Attack_PoisonBreath_Phase2_Right") = std::bind(&EntScript::CompleteAction, this);


		// Attack - WindBreath
		GetAnimator()->Create(L"Ent_Attack_WindBreath_Phase1", pAtlas, Vector2(0.f, 700.f), Vector2(322.f, 350.f), 3, Vector2(322.f, 350.f), Vector2::Zero, 0.2f);
		GetAnimator()->StartEvent(L"Ent_Attack_WindBreath_Phase1") = std::bind(&EntScript::AttackWindBreathSFX, this);
		GetAnimator()->Create(L"Ent_Attack_WindBreath_Phase2", pAtlas, Vector2(0.f, 2800.f), Vector2(322.f, 350.f), 3, Vector2(322.f, 350.f), Vector2::Zero, 0.2f);
		GetAnimator()->StartEvent(L"Ent_Attack_WindBreath_Phase2") = std::bind(&EntScript::AttackWindBreathSFX, this);


		// Rage
		GetAnimator()->Create(L"Ent_Rage", pAtlas, Vector2(0.f, 1750.f), Vector2(322.f, 350.f), 3, Vector2(322.f, 350.f), Vector2::Zero, 0.5f);
		GetAnimator()->StartEvent(L"Ent_Rage") = std::bind(&EntScript::RageSFX, this);
		GetAnimator()->CompleteEvent(L"Ent_Rage") = std::bind(&EntScript::CompleteAction, this);


		// Dead
		GetAnimator()->Create(L"Ent_Dead", pAtlas, Vector2(644.f, 2450.f), Vector2(322.f, 350.f), 3, Vector2(322.f, 350.f), Vector2::Zero, 0.5f);
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
		case EntStateScript::eState::Rage:
			GetAnimator()->PlayAnimation(L"Ent_Rage", true);
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