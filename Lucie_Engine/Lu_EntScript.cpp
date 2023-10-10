#include "Lu_EntScript.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_ProgressBarScript.h"
#include "Lu_SoundManager.h"
#include "Lu_AudioSource.h"
#include "Lu_Renderer.h"
#include "Lu_CameraScript.h"

#include "Lu_EntIdleState.h"
#include "Lu_EntDeadState.h"

namespace Lu
{
	EntScript::EntScript()
		: m_CurState(nullptr)
		, m_PrevState(EntStateScript::eState::End)
		, m_HPFrame(nullptr)
		, m_HPBar(nullptr)
		, m_CurPhase(ePhase::Phase_1)
		, m_bWakeUp(false)
		, m_bHowling(false)
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

		// ���� HP
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
			pHPBarScript->SetBossName(L"�̱׵���");
			pHPBarScript->SetTextPrint(true);
		}

		// ����
		AddState(new EntIdleState);
		//AddState(new EntAttackState);
		AddState(new EntDeadState);

		m_CurState = GetStateScript(EntStateScript::eState::Idle);
		m_CurState->Enter();
	}

	void EntScript::Update()
	{
		if (GetOwner()->IsDead())
			return;

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

	void EntScript::HowlingSFX()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"EntHowlingSFX", L"..\\Resources\\Sound\\SFX\\Monster\\Ent\\EntHowlingSFX.ogg"));
		pSFX->Play();

		// ī�޶� ����ŷ
		renderer::mainCamera->GetOwner()->GetComponent<CameraScript>()->RequestCameraShaking(10.f, 1.5f);
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
		GetAnimator()->Create(L"Ent_WakeUp_1", pAtlas, Vector2(0.f, 0.f), Vector2(322.f, 350.f), 3, Vector2(322.f, 350.f), Vector2::Zero, 0.3f);
		GetAnimator()->CompleteEvent(L"Ent_WakeUp_1") = std::bind(&EntScript::CompleteAction, this);
		GetAnimator()->Create(L"Ent_WakeUp_2", pAtlas, Vector2(0.f, 700.f), Vector2(322.f, 350.f), 3, Vector2(322.f, 350.f), Vector2::Zero, 0.2f);
		GetAnimator()->StartEvent(L"Ent_WakeUp_2") = std::bind(&EntScript::HowlingSFX, this);
		GetAnimator()->CompleteEvent(L"Ent_WakeUp_2") = std::bind(&EntScript::CompleteAction, this);


		// Attack
		GetAnimator()->Create(L"Ent_Phase1_Attack_Left", pAtlas, Vector2(0.f, 2800.f), Vector2(322.f, 350.f), 2, Vector2(322.f, 350.f), Vector2::Zero, 1.f);
		GetAnimator()->StartEvent(L"Ent_Phase1_Attack_Left") = std::bind(&EntScript::AttackSFX, this);
		GetAnimator()->CompleteEvent(L"Ent_Phase1_Attack_Left") = std::bind(&EntScript::CompleteAction, this);

		GetAnimator()->Create(L"Ent_Phase1_Attack_Right", pAtlas, Vector2(0.f, 2800.f), Vector2(322.f, 350.f), 2, Vector2(322.f, 350.f), Vector2::Zero, 1.f);
		GetAnimator()->StartEvent(L"Ent_Phase1_Attack_Right") = std::bind(&EntScript::AttackSFX, this);
		GetAnimator()->CompleteEvent(L"Ent_Phase1_Attack_Right") = std::bind(&EntScript::CompleteAction, this);

		GetAnimator()->Create(L"Ent_Phase2_Attack_Left", pAtlas, Vector2(0.f, 2800.f), Vector2(322.f, 350.f), 2, Vector2(322.f, 350.f), Vector2::Zero, 1.f);
		GetAnimator()->StartEvent(L"Ent_Phase2_Attack_Left") = std::bind(&EntScript::AttackSFX, this);
		GetAnimator()->CompleteEvent(L"Ent_Phase2_Attack_Left") = std::bind(&EntScript::CompleteAction, this);

		GetAnimator()->Create(L"Ent_Phase2_Attack_Right", pAtlas, Vector2(0.f, 2800.f), Vector2(322.f, 350.f), 2, Vector2(322.f, 350.f), Vector2::Zero, 1.f);
		GetAnimator()->StartEvent(L"Ent_Phase2_Attack_Right") = std::bind(&EntScript::AttackSFX, this);
		GetAnimator()->CompleteEvent(L"Ent_Phase2_Attack_Right") = std::bind(&EntScript::CompleteAction, this);


		// Dead
		GetAnimator()->Create(L"Ent_Dead", pAtlas, Vector2(0.f, 2800.f), Vector2(322.f, 350.f), 2, Vector2(322.f, 350.f), Vector2::Zero, 1.f);
		GetAnimator()->StartEvent(L"Ent_Dead") = std::bind(&EntScript::DeadSFX, this);
		GetAnimator()->CompleteEvent(L"Ent_Dead") = std::bind(&EntScript::CompleteAction, this);
	}

	void EntScript::AnimationUpdate()
	{
		EntStateScript::eState eCurState = m_CurState->GetStateType();
		eAnimDir eCurDir = GetCurDir();

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
		case EntStateScript::eState::Attack:
		{
			switch (m_CurPhase)
			{
			case Lu::EntScript::ePhase::Phase_1:
			{
				switch (eCurDir)
				{
				case Lu::MonsterScript::eAnimDir::Left:
					GetAnimator()->PlayAnimation(L"Ent_Phase1_Attack_Left", true);
					break;
				case Lu::MonsterScript::eAnimDir::Right:
					GetAnimator()->PlayAnimation(L"Ent_Phase1_Attack_Right", true);
					break;
				default:
					break;
				}
			}
			break;
			case Lu::EntScript::ePhase::Phase_2:
			{
				switch (eCurDir)
				{
				case Lu::MonsterScript::eAnimDir::Left:
					GetAnimator()->PlayAnimation(L"Ent_Phase2_Attack_Left", true);
					break;
				case Lu::MonsterScript::eAnimDir::Right:
					GetAnimator()->PlayAnimation(L"Ent_Phase2_Attack_Right", true);
					break;
				default:
					break;
				}
			}
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
			GetAnimator()->PlayAnimation(L"Ent_WakeUp_1", true);
	}

	void EntScript::Howling()
	{
		if (!m_bHowling)
		{
			m_bHowling = true;
			GetAnimator()->PlayAnimation(L"Ent_WakeUp_2", true);
		}
	}
}