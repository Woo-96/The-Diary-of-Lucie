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
		// ����
		Vector3 vPos = GetTransform()->GetPosition();
		float distanceToMove = GetSlimeScript()->GetInfo().MoveSpeed * (float)Time::DeltaTime();
		Vector3 moveVector = GetDir() * distanceToMove;
		vPos += moveVector;
		GetTransform()->SetPosition(vPos);


		// �Ÿ��� �־����� ���� -> ���̵�
		if (CalDirToPlayer().Length() > m_ChangeDist)
		{
			GetSlimeScript()->ChangeState(eState::Idle);
		}


		// �ð��� ���� ���� -> ���̵�
		ChangeStateAfterTime(3.f, eState::Idle);
	}

	void SlimeAttackState::Enter()
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

	void SlimeAttackState::Exit()
	{
		SetTime(0.f);
	}

	void SlimeAttackState::CreateProjectile(Vector3 _Dir)
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