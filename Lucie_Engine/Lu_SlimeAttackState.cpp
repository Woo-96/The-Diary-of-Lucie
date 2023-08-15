#include "Lu_SlimeAttackState.h"
#include "Lu_SlimeScript.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_CircleProjectile.h"
#include "Lu_Resources.h"
#include "Lu_PlayerScript.h"
#include "Lu_Time.h"

namespace Lu
{
	SlimeAttackState::SlimeAttackState()
		: m_ChangeDist(300.f)
	{
		SetName(L"SlimeAttackStateScript");
		SetStateType(eState::Attack);
	}

	SlimeAttackState::~SlimeAttackState()
	{

	}

	void SlimeAttackState::Update()
	{
		// 어택
		Vector3 vPos = GetTransform()->GetPosition();
		float distanceToMove = GetSlimeScript()->GetInfo().MoveSpeed * (float)Time::DeltaTime();
		Vector3 moveVector = GetDir() * distanceToMove;
		vPos += moveVector;
		GetTransform()->SetPosition(vPos);


		// 거리가 멀어지면 어택 -> 아이들
		if (CalDirToPlayer().Length() > m_ChangeDist)
		{
			GetSlimeScript()->ChangeState(eState::Idle);
		}


		// 시간에 따라 어택 -> 아이들
		ChangeStateAfterTime(3.f, eState::Idle);
	}

	void SlimeAttackState::Enter()
	{
		// 방향 결정
		Vector3 vDir = Vector3(CalDirToPlayer().x, CalDirToPlayer().y, 1.f);
		vDir.Normalize();
		SetDir(vDir);

		// 애니메이션 방향 결정
		DetermineAnimDir(vDir);

		// 투사체 발사
		CreateProjectile(vDir);
	}

	void SlimeAttackState::Exit()
	{
		SetTime(0.f);
	}

	void SlimeAttackState::CreateProjectile(Vector3 _Dir)
	{
		// 기본 투사체 방향
		Vector3 baseDir = _Dir;
		baseDir.Normalize();

		// 각도 변환을 위한 라디안 계산
		float angle15 = math::DegreeToRadian(15.0f);

		// 투사체 생성과 설정
		for (int i = 0; i < 3; ++i)
		{
			GameObject* pProjectile = object::Instantiate<GameObject>(GetTransform()->GetPosition(), Vector3(50.f, 50.f, 100.f), eLayerType::MonsterProjectile);
			pProjectile->SetName(L"SlimeProjectile");

			MeshRenderer* pMeshRender = pProjectile->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"MonsterProjectile_Circle_Mtrl"));

			Collider2D* pCollider = pProjectile->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);
			pCollider->SetSize(Vector2(0.6f, 0.6f));

			CircleProjectile* pProjectileScript = pProjectile->AddComponent<CircleProjectile>();
			pProjectileScript->SetMonsterScript((MonsterScript*)GetSlimeScript());
			pProjectileScript->SetTransform(pProjectile->GetComponent<Transform>());

			// 첫 번째 투사체는 기본 방향으로 설정
			if (i == 0)
			{
				pProjectileScript->SetDir(baseDir);
			}
			else
			{
				// 나머지 투사체는 각도 조절하여 설정
				Vector3 dir = baseDir;
				if (i == 1)
				{
					// -15도 각도 조절
					dir = Vector3(std::cos(angle15) * baseDir.x - std::sin(angle15) * baseDir.y,
						std::sin(angle15) * baseDir.x + std::cos(angle15) * baseDir.y,
						0.0f);
				}
				else if (i == 2)
				{
					// +15도 각도 조절
					dir = Vector3(std::cos(-angle15) * baseDir.x - std::sin(-angle15) * baseDir.y,
						std::sin(-angle15) * baseDir.x + std::cos(-angle15) * baseDir.y,
						0.0f);
				}

				dir.Normalize();
				pProjectileScript->SetDir(dir);
			}
		}
	}
}