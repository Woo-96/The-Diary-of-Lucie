#include "Lu_BubbleProjectile.h"
#include "Lu_Time.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_CircleProjectile.h"

namespace Lu
{
	BubbleProjectile::BubbleProjectile()
	{
		SetName(L"BubbleProjectileScript");
	}

	BubbleProjectile::~BubbleProjectile()
	{

	}

	void BubbleProjectile::Initialize()
	{

	}

	//void BubbleProjectile::Update()
	//{
	//	Vector3 vPos = GetTransform()->GetPosition();
	//	Vector3 moveVector = GetDir() * GetSpeed() * (float)Time::DeltaTime();
	//	vPos += moveVector;
	//	GetTransform()->SetPosition(vPos);

	//	// �����鼭 ��Ŭ ȸ���� �߻�
	//	SetTime(GetTime() + (float)Time::DeltaTime());

	//	if (GetTime() >= 2.5f)
	//	{
	//		const int numSets = 8; // ¦�� ����
	//		const int numProjectilesPerSet = 3;
	//		const float totalAngle = 360.0f;
	//		const float angleIncrement = totalAngle / numSets;
	//		const float radius = 100.0f;

	//		for (int i = 0; i < numSets; ++i)
	//		{
	//			float baseAngle = i * angleIncrement;

	//			for (int j = 0; j < numProjectilesPerSet; ++j)
	//			{
	//				float angle = baseAngle + (angleIncrement / numProjectilesPerSet) * j;
	//				float angleInRadians = DegreeToRadian(angle);

	//				Vector3 newPosition;
	//				newPosition.x = vPos.x + radius * cos(angleInRadians);
	//				newPosition.y = vPos.y + radius * sin(angleInRadians);
	//				newPosition.z = vPos.z;

	//				GameObject* pProjectile = object::Instantiate<GameObject>(newPosition, Vector3(80.f, 80.f, 100.f), eLayerType::MonsterProjectile);
	//				pProjectile->SetName(L"BossCircleProjectile");

	//				MeshRenderer* pMeshRender = pProjectile->AddComponent<MeshRenderer>();
	//				pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
	//				pMeshRender->SetMaterial(Resources::Find<Material>(L"MonsterProjectile_BossCircle_Mtrl"));

	//				Collider2D* pCollider = pProjectile->AddComponent<Collider2D>();
	//				pCollider->SetType(eColliderType::Rect);
	//				pCollider->SetSize(Vector2(0.6f, 0.6f));

	//				BossCircleProjectile* pProjectileScript = pProjectile->AddComponent<BossCircleProjectile>();
	//				pProjectileScript->SetMonsterScript((MonsterScript*)this);
	//				pProjectileScript->SetTransform(pProjectile->GetComponent<Transform>());
	//				pProjectileScript->SetSpeed(300.f);
	//				pProjectileScript->SetCenterPos(vPos);
	//			}
	//		}

	//		object::Destroy(GetOwner());
	//	}
	//}

	void BubbleProjectile::Update()
	{
		Vector3 vPos = GetTransform()->GetPosition();
		float Zpos = vPos.z;
		vPos += GetDir() * GetSpeed() * (float)Time::DeltaTime();
		vPos.z = Zpos;
		GetTransform()->SetPosition(vPos);

		// �����鼭 ��Ŭ ȸ���� �߻�
		SetTime(GetTime() + (float)Time::DeltaTime());

		if (GetTime() >= 2.5f)
		{
			const int numProjectiles = 8;
			const float angleIncrement = 360.0f / numProjectiles; // 8�������� �������� ���� ���� ���

			for (int i = 0; i < numProjectiles; ++i)
			{
				GameObject* pProjectile = object::Instantiate<GameObject>(vPos, Vector3(80.f, 80.f, 100.f), eLayerType::MonsterProjectile);
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

			// ������ ����
			object::Destroy(GetOwner());
		}
	}

	void BubbleProjectile::OnCollisionEnter(Collider2D* other)
	{
		MonsterProjectileScript::OnCollisionEnter(other);
	}

	void BubbleProjectile::OnCollisionStay(Collider2D* other)
	{

	}

	void BubbleProjectile::OnCollisionExit(Collider2D* other)
	{

	}
}