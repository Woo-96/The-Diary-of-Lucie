#include "Lu_BigSlimeScript.h"
#include "Lu_GameObject.h"
#include "Lu_Animator.h"
#include "Lu_Resources.h"
#include "Lu_PlayerScript.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Time.h"
#include "Lu_Object.h"
#include "Lu_CircleProjectile.h"

#include "Lu_BigSlimeIdleState.h"
#include "Lu_BigSlimeTraceState.h"
#include "Lu_BigSlimeAttackState.h"
#include "Lu_BigSlimeDeadState.h"

namespace Lu
{
	BigSlimeScript::BigSlimeScript()
		: m_CurState(nullptr)
		, m_PrevState(BigSlimeStateScript::eState::End)
		, m_Target(nullptr)
		, m_HPFrame(nullptr)
		, m_HPBar(nullptr)
		, m_Time(3.f)
		, m_bAttack(false)
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
		CircleAttack();
		AnimationUpdate();

		m_PrevState = m_CurState->GetStateType();
		SetPrevDir(GetCurDir());
	}

	void BigSlimeScript::OnCollisionEnter(Collider2D* _Other)
	{
		if ((int)eLayerType::Immovable == _Other->GetOwner()->GetLayerIndex())
			ChangeState(BigSlimeStateScript::eState::Idle);

		if ((int)eLayerType::PlayerProjectile == _Other->GetOwner()->GetLayerIndex())
		{
			if (BigSlimeStateScript::eState::Dead == m_CurState->GetStateType())
				return;

			GetInfo().HP -= 50;

			if (GetInfo().HP <= 0.f)
			{
				ChangeState(BigSlimeStateScript::eState::Dead);
			}
		}
	}

	void BigSlimeScript::OnCollisionStay(Collider2D* _Other)
	{
	}

	void BigSlimeScript::OnCollisionExit(Collider2D* _Other)
	{
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
		GetAnimator()->CompleteEvent(L"BigSlime_Jump_Left") = std::bind(&BigSlimeScript::CompleteAction, this);
		GetAnimator()->Create(L"BigSlime_Jump_Right", pAtlas, Vector2(0.f, 3600.f), Vector2(360.f, 360.f), 3, Vector2(360.f, 360.f), Vector2::Zero, 0.2f);
		GetAnimator()->CompleteEvent(L"BigSlime_Jump_Right") = std::bind(&BigSlimeScript::CompleteAction, this);
		GetAnimator()->Create(L"BigSlime_Jump_Up", pAtlas, Vector2(0.f, 3960.f), Vector2(360.f, 360.f), 3, Vector2(360.f, 360.f), Vector2::Zero, 0.2f);
		GetAnimator()->CompleteEvent(L"BigSlime_Jump_Up") = std::bind(&BigSlimeScript::CompleteAction, this);
		GetAnimator()->Create(L"BigSlime_Jump_Down", pAtlas, Vector2(0.f, 2880.f), Vector2(360.f, 360.f), 3, Vector2(360.f, 360.f), Vector2::Zero, 0.2f);
		GetAnimator()->CompleteEvent(L"BigSlime_Jump_Down") = std::bind(&BigSlimeScript::CompleteAction, this);

		// Dead
		GetAnimator()->Create(L"BigSlime_Dead", pAtlas, Vector2(0.f, 7200.f), Vector2(360.f, 360.f), 1, Vector2(360.f, 360.f), Vector2::Zero, 1.f);
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

	void BigSlimeScript::CircleAttack()
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
			// 첫 공격 이후 3초마다 Circle 발사
			m_Time += (float)Time::DeltaTime();
			if (m_Time >= 3.f)
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