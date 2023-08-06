#include "Lu_SnabyAttackState.h"
#include "Lu_SnabyScript.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_CircleProjectile.h"
#include "Lu_Resources.h"
#include "Lu_PlayerScript.h"
#include "Lu_Time.h"

namespace Lu
{
	SnabyAttackState::SnabyAttackState()
		: m_ChangeDist(300.f)
	{
		SetName(L"SnabyAttackStateScript");
		SetStateType(eState::Attack);
	}

	SnabyAttackState::~SnabyAttackState()
	{

	}

	void SnabyAttackState::Update()
	{
		// ����
		Vector3 vPos = GetTransform()->GetPosition();
		float distanceToMove = GetSnabyScript()->GetInfo().MoveSpeed * (float)Time::DeltaTime();
		Vector3 moveVector = GetDir() * distanceToMove;
		vPos += moveVector;
		GetTransform()->SetPosition(vPos);

		
		// �Ÿ��� �־����� ���� -> ���̵�
		if (CalDirToPlayer().Length() > m_ChangeDist)
		{
			GetSnabyScript()->ChangeState(eState::Idle);
		}


		// �ð��� ���� ���� -> ���̵�
		ChangeStateAfterTime(3.f, eState::Idle);
	}

	void SnabyAttackState::Enter()
	{
		// ���� ����
		Vector3 vDir = Vector3(CalDirToPlayer().x, CalDirToPlayer().y, 1.f);
		vDir.Normalize();
		SetDir(vDir);

		// �ִϸ��̼� ���� ����
		DetermineAnimDir(vDir);

		// ����ü �߻�
		CreateProjectile(vDir);
	}

	void SnabyAttackState::Exit()
	{
		SetTime(0.f);
	}

	void SnabyAttackState::CreateProjectile(Vector3 _Dir)
	{
		// �⺻ ����ü ����
		Vector3 baseDir = _Dir;
		baseDir.Normalize();

		// ���� ��ȯ�� ���� ���� ���
		float angle15 = math::DegreeToRadian(15.0f);

		// ����ü ������ ����
		for (int i = 0; i < 3; ++i)
		{
			GameObject* pProjectile = object::Instantiate<GameObject>(GetTransform()->GetPosition(), Vector3(50.f, 50.f, 100.f), eLayerType::MonsterProjectile);
			pProjectile->SetName(L"SnabyProjectile");

			MeshRenderer* pMeshRender = pProjectile->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"MonsterProjectile_Circle_Mtrl"));

			Collider2D* pCollider = pProjectile->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);
			pCollider->SetSize(Vector2(0.6f, 0.6f));

			CircleProjectile* pProjectileScript = pProjectile->AddComponent<CircleProjectile>();
			pProjectileScript->SetMonsterScript((MonsterScript*)GetSnabyScript());
			pProjectileScript->SetTransform(pProjectile->GetComponent<Transform>());

			// ù ��° ����ü�� �⺻ �������� ����
			if (i == 0)
			{
				pProjectileScript->SetDir(baseDir);
			}
			else
			{
				// ������ ����ü�� ���� �����Ͽ� ����
				Vector3 dir = baseDir;
				if (i == 1)
				{
					// -15�� ���� ����
					dir = Vector3(std::cos(angle15) * baseDir.x - std::sin(angle15) * baseDir.y,
						std::sin(angle15) * baseDir.x + std::cos(angle15) * baseDir.y,
						0.0f);
				}
				else if (i == 2)
				{
					// +15�� ���� ����
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