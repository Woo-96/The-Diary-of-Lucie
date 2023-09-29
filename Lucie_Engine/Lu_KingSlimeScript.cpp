#include "Lu_KingSlimeScript.h"
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

#include "Lu_KingSlimeIdleState.h"
#include "Lu_KingSlimeTraceState.h"
#include "Lu_KingSlimeAttackState.h"
#include "Lu_KingSlimeDeadState.h"

namespace Lu
{
	KingSlimeScript::KingSlimeScript()
		: m_CurState(nullptr)
		, m_PrevState(KingSlimeStateScript::eState::End)
		, m_HPFrame(nullptr)
		, m_HPBar(nullptr)
		, m_Time(8.f)
		, m_bAttack(false)
	{
		SetName(L"KingSlimeScript");

		GetInfo().HP = 100;
		GetInfo().MaxHP = 100;
		GetInfo().MoveSpeed = 150.f;
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

		// 보스 HP
		{
			m_HPFrame = object::Instantiate<GameObject>(Vector3(0.f, 350.f, 100.f), Vector3(759.f, 48.f, 100.f), eLayerType::UI);
			m_HPFrame->SetName(L"KingSlime_HPFrame");

			MeshRenderer* pMeshRender = m_HPFrame->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"MidBossHPFrame_Mtrl"));
		}

		{
			m_HPBar = object::Instantiate<GameObject>(Vector3(0.f, 350.f, 100.f), Vector3(720.f, 30.f, 100.f), eLayerType::UI);
			m_HPBar->SetName(L"KingSlime_HPBar");

			MeshRenderer* pMeshRender = m_HPBar->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"KingSlime_BossHP_Mtrl"));
		
			ProgressBarScript* pHPBarScript = m_HPBar->AddComponent<ProgressBarScript>();
			pHPBarScript->SetTransform(m_HPBar->GetComponent<Transform>());
			pHPBarScript->SetMeshRender(pMeshRender);
			pHPBarScript->SetMaxValue(GetInfo().MaxHP);
			pHPBarScript->SetCurValue(GetInfo().HP);
			pHPBarScript->SetBossName(L"킹슬라임");
			pHPBarScript->SetTextPrint(true);
		}

		// 상태
		AddState(new KingSlimeIdleState);
		AddState(new KingSlimeTraceState);
		AddState(new KingSlimeAttackState);
		AddState(new KingSlimeDeadState);

		m_CurState = GetStateScript(KingSlimeStateScript::eState::Idle);
		m_CurState->Enter();
	}

	void KingSlimeScript::Update()
	{
		if (GetOwner()->IsDead())
			return;

		m_CurState->Update();
		CircleAttack();
		AnimationUpdate();

		m_PrevState = m_CurState->GetStateType();
		SetPrevDir(GetCurDir());
	}

	void KingSlimeScript::OnCollisionEnter(Collider2D* _Other)
	{
		MonsterScript::OnCollisionEnter(_Other);

		m_HPBar->GetComponent<ProgressBarScript>()->SetCurValue(GetInfo().HP);
	}

	void KingSlimeScript::OnCollisionStay(Collider2D* _Other)
	{
		MonsterScript::OnCollisionStay(_Other);

		m_HPBar->GetComponent<ProgressBarScript>()->SetCurValue(GetInfo().HP);
	}

	void KingSlimeScript::CreateAnimation()
	{
		std::shared_ptr<Texture> pAtlas
			= Resources::Load<Texture>(L"KingSlimeSprite", L"..\\Resources\\Texture\\Monster\\MidBoss\\Slime.png");

		// Idle
		GetAnimator()->Create(L"KingSlime_Idle_Left", pAtlas, Vector2(0.f, 360.f), Vector2(360.f, 360.f), 2, Vector2(360.f, 360.f), Vector2::Zero, 0.3f);
		GetAnimator()->Create(L"KingSlime_Idle_Right", pAtlas, Vector2(0.f, 720.f), Vector2(360.f, 360.f), 2, Vector2(360.f, 360.f), Vector2::Zero, 0.3f);
		GetAnimator()->Create(L"KingSlime_Idle_Up", pAtlas, Vector2(0.f, 1080.f), Vector2(360.f, 360.f), 2, Vector2(360.f, 360.f), Vector2::Zero, 0.3f);
		GetAnimator()->Create(L"KingSlime_Idle_Down", pAtlas, Vector2(0.f, 0.f), Vector2(360.f, 360.f), 3, Vector2(360.f, 360.f), Vector2::Zero, 0.3f);

		// Move
		GetAnimator()->Create(L"KingSlime_Move_Left", pAtlas, Vector2(0.f, 1800.f), Vector2(360.f, 360.f), 3, Vector2(360.f, 360.f), Vector2::Zero, 0.2f);
		GetAnimator()->Create(L"KingSlime_Move_Right", pAtlas, Vector2(0.f, 2160.f), Vector2(360.f, 360.f), 3, Vector2(360.f, 360.f), Vector2::Zero, 0.2f);
		GetAnimator()->Create(L"KingSlime_Move_Up", pAtlas, Vector2(0.f, 2520.f), Vector2(360.f, 360.f), 3, Vector2(360.f, 360.f), Vector2::Zero, 0.2f);
		GetAnimator()->Create(L"KingSlime_Move_Down", pAtlas, Vector2(0.f, 1440.f), Vector2(360.f, 360.f), 3, Vector2(360.f, 360.f), Vector2::Zero, 0.2f);

		// Jump
		GetAnimator()->Create(L"KingSlime_Jump_Left", pAtlas, Vector2(0.f, 3240.f), Vector2(360.f, 360.f), 3, Vector2(360.f, 360.f), Vector2::Zero, 0.2f);
		GetAnimator()->StartEvent(L"KingSlime_Jump_Left") = std::bind(&KingSlimeScript::JumpSFX, this);
		GetAnimator()->CompleteEvent(L"KingSlime_Jump_Left") = std::bind(&KingSlimeScript::AttackSFX, this);
		
		GetAnimator()->Create(L"KingSlime_Jump_Right", pAtlas, Vector2(0.f, 3600.f), Vector2(360.f, 360.f), 3, Vector2(360.f, 360.f), Vector2::Zero, 0.2f);
		GetAnimator()->StartEvent(L"KingSlime_Jump_Right") = std::bind(&KingSlimeScript::JumpSFX, this);
		GetAnimator()->CompleteEvent(L"KingSlime_Jump_Right") = std::bind(&KingSlimeScript::AttackSFX, this);
		
		GetAnimator()->Create(L"KingSlime_Jump_Up", pAtlas, Vector2(0.f, 3960.f), Vector2(360.f, 360.f), 3, Vector2(360.f, 360.f), Vector2::Zero, 0.2f);
		GetAnimator()->StartEvent(L"KingSlime_Jump_Up") = std::bind(&KingSlimeScript::JumpSFX, this);
		GetAnimator()->CompleteEvent(L"KingSlime_Jump_Up") = std::bind(&KingSlimeScript::AttackSFX, this);
		
		GetAnimator()->Create(L"KingSlime_Jump_Down", pAtlas, Vector2(0.f, 2880.f), Vector2(360.f, 360.f), 3, Vector2(360.f, 360.f), Vector2::Zero, 0.2f);
		GetAnimator()->StartEvent(L"KingSlime_Jump_Down") = std::bind(&KingSlimeScript::JumpSFX, this);
		GetAnimator()->CompleteEvent(L"KingSlime_Jump_Down") = std::bind(&KingSlimeScript::AttackSFX, this);

		// Dead
		GetAnimator()->Create(L"KingSlime_Dead", pAtlas, Vector2(0.f, 7200.f), Vector2(360.f, 360.f), 1, Vector2(360.f, 360.f), Vector2::Zero, 1.f);
		GetAnimator()->StartEvent(L"KingSlime_Dead") = std::bind(&KingSlimeScript::DeadSFX, this);
		GetAnimator()->CompleteEvent(L"KingSlime_Dead") = std::bind(&KingSlimeScript::CompleteAction, this);
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
		ChangeState(KingSlimeStateScript::eState::Trace);
	}

	void KingSlimeScript::JumpSFX()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"SlimeJumpSFX", L"..\\Resources\\Sound\\SFX\\Monster\\Slime\\SlimeJumpSFX.ogg"));
		pSFX->Play();
	}

	void KingSlimeScript::AttackSFX()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"SlimeBubbleSFX", L"..\\Resources\\Sound\\SFX\\Monster\\Slime\\SlimeBubbleSFX.ogg"));
		pSFX->Play();

		ChangeState(KingSlimeStateScript::eState::Trace);
	}

	void KingSlimeScript::DeadSFX()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"SlimeDeadSFX", L"..\\Resources\\Sound\\SFX\\Monster\\Slime\\SlimeDeadSFX.ogg"));
		pSFX->Play();
	}

	void KingSlimeScript::CircleAttack()
	{
		if (!m_bAttack)
		{
			// 최초 스폰 n초 후 Circle 첫 공격
			m_Time -= (float)Time::DeltaTime();
			if (m_Time <= 0.f)
			{
				m_bAttack = true;
				m_Time = 0.f;
			}
		}
		else
		{
			// 첫 공격 이후 n초마다 Circle 발사
			m_Time += (float)Time::DeltaTime();
			if (m_Time >= 1.5f)
			{
				m_Time = 0.f;

				Vector3 vBossPos = GetOwner()->GetComponent<Transform>()->GetPosition();
				vBossPos.y -= 150.f;
				vBossPos.z = 600.f;

				const int numProjectiles = 8;
				const float angleIncrement = 360.0f / numProjectiles; // 8방향으로 퍼지도록 각도 간격 계산

				// 써클 투사체 생성
				for (int i = 0; i < numProjectiles; ++i)
				{
					GameObject* pProjectile = object::Instantiate<GameObject>(vBossPos, Vector3(80.f, 80.f, 100.f), eLayerType::MonsterProjectile);
					pProjectile->SetName(L"CircleProjectile");

					MeshRenderer* pMeshRender = pProjectile->AddComponent<MeshRenderer>();
					pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
					pMeshRender->SetMaterial(Resources::Find<Material>(L"MonsterProjectile_SlimeCircle_Mtrl"));

					Collider2D* pCollider = pProjectile->AddComponent<Collider2D>();
					pCollider->SetType(eColliderType::Rect);
					pCollider->SetSize(Vector2(0.6f, 0.6f));

					CircleProjectile* pProjectileScript = pProjectile->AddComponent<CircleProjectile>();
					pProjectileScript->SetMonsterScript((MonsterScript*)this);
					pProjectileScript->SetTransform(pProjectile->GetComponent<Transform>());
					pProjectileScript->SetSpeed(300.f);

					float angle = i * angleIncrement; // 투사체의 방향을 결정하는 각도 계산
					float angleInRadians = DegreeToRadian(angle); // 각도를 라디안으로 변환
					// 삼각함수를 이용하여 방향 벡터 계산
					float cosAngle = cos(angleInRadians);
					float sinAngle = sin(angleInRadians);
					Vector3 forwardDirection(cosAngle, sinAngle, 0.f); // 투사체가 전진할 방향 벡터 계산
					pProjectileScript->SetDir(forwardDirection);
				}
			}
		}
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
				GetAnimator()->PlayAnimation(L"KingSlime_Idle_Left", true);
				break;
			case Lu::MonsterScript::eAnimDir::Right:
				GetAnimator()->PlayAnimation(L"KingSlime_Idle_Right", true);
				break;
			case Lu::MonsterScript::eAnimDir::Up:
				GetAnimator()->PlayAnimation(L"KingSlime_Idle_Up", true);
				break;
			case Lu::MonsterScript::eAnimDir::Down:
				GetAnimator()->PlayAnimation(L"KingSlime_Idle_Down", true);
				break;
			default:
				break;
			}
		}
		break;
		case KingSlimeStateScript::eState::Trace:
		{
			switch (eCurDir)
			{
			case Lu::MonsterScript::eAnimDir::Left:
				GetAnimator()->PlayAnimation(L"KingSlime_Move_Left", true);
				break;
			case Lu::MonsterScript::eAnimDir::Right:
				GetAnimator()->PlayAnimation(L"KingSlime_Move_Right", true);
				break;
			case Lu::MonsterScript::eAnimDir::Up:
				GetAnimator()->PlayAnimation(L"KingSlime_Move_Up", true);
				break;
			case Lu::MonsterScript::eAnimDir::Down:
				GetAnimator()->PlayAnimation(L"KingSlime_Move_Down", true);
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
				GetAnimator()->PlayAnimation(L"KingSlime_Jump_Left", true);
				break;
			case Lu::MonsterScript::eAnimDir::Right:
				GetAnimator()->PlayAnimation(L"KingSlime_Jump_Right", true);
				break;
			case Lu::MonsterScript::eAnimDir::Up:
				GetAnimator()->PlayAnimation(L"KingSlime_Jump_Up", true);
				break;
			case Lu::MonsterScript::eAnimDir::Down:
				GetAnimator()->PlayAnimation(L"KingSlime_Jump_Down", true);
				break;
			default:
				break;
			}
		}
		break;
		case KingSlimeStateScript::eState::Dead:
			GetAnimator()->PlayAnimation(L"KingSlime_Dead", true);
			break;
		}
	}

	void KingSlimeScript::ChangeIdleState()
	{
		ChangeState(KingSlimeStateScript::eState::Idle);
	}

	void KingSlimeScript::ChangeDeadState()
	{
		ChangeState(KingSlimeStateScript::eState::Dead);
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