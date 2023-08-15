#include "Lu_KingSlimeScript.h"
#include "Lu_GameObject.h"
#include "Lu_Animator.h"
#include "Lu_Resources.h"
#include "Lu_PlayerScript.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Time.h"
#include "Lu_Object.h"
#include "Lu_CircleProjectile.h"

#include "Lu_KingSlimeIdleState.h"
#include "Lu_KingSlimeTraceState.h"
#include "Lu_KingSlimeAttackState.h"
#include "Lu_KingSlimeDeadState.h"

namespace Lu
{
	KingSlimeScript::KingSlimeScript()
		: m_CurState(nullptr)
		, m_PrevState(KingSlimeStateScript::eState::End)
		, m_Target(nullptr)
		, m_Time(13.f)
		, m_bAttack(false)
	{
		SetName(L"KingSlimeScript");

		GetInfo().HP = 300;
		GetInfo().MaxHP = 300;
		GetInfo().DetectRange = 200.f;
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
		GetAnimator()->CompleteEvent(L"KingSlime_Jump_Left") = std::bind(&KingSlimeScript::CompleteAction, this);
		GetAnimator()->Create(L"KingSlime_Jump_Right", pAtlas, Vector2(0.f, 3600.f), Vector2(360.f, 360.f), 3, Vector2(360.f, 360.f), Vector2::Zero, 0.2f);
		GetAnimator()->CompleteEvent(L"KingSlime_Jump_Right") = std::bind(&KingSlimeScript::CompleteAction, this);
		GetAnimator()->Create(L"KingSlime_Jump_Up", pAtlas, Vector2(0.f, 3960.f), Vector2(360.f, 360.f), 3, Vector2(360.f, 360.f), Vector2::Zero, 0.2f);
		GetAnimator()->CompleteEvent(L"KingSlime_Jump_Up") = std::bind(&KingSlimeScript::CompleteAction, this);
		GetAnimator()->Create(L"KingSlime_Jump_Down", pAtlas, Vector2(0.f, 2880.f), Vector2(360.f, 360.f), 3, Vector2(360.f, 360.f), Vector2::Zero, 0.2f);
		GetAnimator()->CompleteEvent(L"KingSlime_Jump_Down") = std::bind(&KingSlimeScript::CompleteAction, this);

		// Dead
		GetAnimator()->Create(L"KingSlime_Dead", pAtlas, Vector2(0.f, 7200.f), Vector2(360.f, 360.f), 1, Vector2(360.f, 360.f), Vector2::Zero, 1.f);
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

	void KingSlimeScript::CircleAttack()
	{
		if (!m_bAttack)
		{
			// ���� ���� 13�� �� Circle ù ����
			m_Time -= (float)Time::DeltaTime();
			if (m_Time <= 0.f)
			{
				m_bAttack = true;
				m_Time = 0.f;
			}
		}
		else
		{
			// ù ���� ���� 3�ʸ��� Circle �߻�
			m_Time += (float)Time::DeltaTime();
			if (m_Time >= 3.f)
			{
				m_Time = 0.f;

				Vector3 vBossPos = GetOwner()->GetComponent<Transform>()->GetPosition();
				vBossPos.y -= 150.f;
				vBossPos.z = 600.f;

				const int numProjectiles = 8;
				const float angleIncrement = 360.0f / numProjectiles; // 8�������� �������� ���� ���� ���

				// ��Ŭ ����ü ����
				for (int i = 0; i < numProjectiles; ++i)
				{
					GameObject* pProjectile = object::Instantiate<GameObject>(vBossPos, Vector3(80.f, 80.f, 100.f), eLayerType::MonsterProjectile);
					pProjectile->SetName(L"CircleProjectile");

					MeshRenderer* pMeshRender = pProjectile->AddComponent<MeshRenderer>();
					pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
					pMeshRender->SetMaterial(Resources::Find<Material>(L"MonsterProjectile_BossCircle_Mtrl"));

					Collider2D* pCollider = pProjectile->AddComponent<Collider2D>();
					pCollider->SetType(eColliderType::Rect);
					pCollider->SetSize(Vector2(0.6f, 0.6f));

					CircleProjectile* pProjectileScript = pProjectile->AddComponent<CircleProjectile>();
					pProjectileScript->SetMonsterScript((MonsterScript*)this);
					pProjectileScript->SetTransform(pProjectile->GetComponent<Transform>());
					pProjectileScript->SetSpeed(300.f);

					float angle = i * angleIncrement; // ����ü�� ������ �����ϴ� ���� ���
					float angleInRadians = DegreeToRadian(angle); // ������ �������� ��ȯ
					// �ﰢ�Լ��� �̿��Ͽ� ���� ���� ���
					float cosAngle = cos(angleInRadians);
					float sinAngle = sin(angleInRadians);
					Vector3 forwardDirection(cosAngle, sinAngle, 0.f); // ����ü�� ������ ���� ���� ���
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