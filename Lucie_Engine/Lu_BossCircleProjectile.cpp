#include "Lu_BossCircleProjectile.h"
#include "Lu_Time.h"
#include "Lu_Object.h"

namespace Lu
{
	BossCircleProjectile::BossCircleProjectile()
	{
		SetName(L"BossCircleProjectileScript");
	}

	BossCircleProjectile::~BossCircleProjectile()
	{

	}

	void BossCircleProjectile::Initialize()
	{

	}

	//void BossCircleProjectile::Update()
	//{
	//	// ��ȯ �� ������ ������ ���·� �̵� (������ ��� ��������?)
	//	Vector3 vPos = GetTransform()->GetPosition();



	//	GetTransform()->SetPosition(vPos);


	//	// 2.5���� �ٽ� ���� ��ġ�� �ǵ��ư�
	//	SetTime(GetTime() + (float)Time::DeltaTime());

	//	if (GetTime() >= 2.5f)
	//	{




	//		// 5���� �Ҹ� (���� �ӵ��� ���������ٰ� �ǵ��ƿ��ϱ� 2.5�� �̵� �� 2.5�� �ǵ��ƿ��� ���� �����̰���?)
	//		if (GetTime() >= 5.f)
	//		{
	//			object::Destroy(GetOwner());
	//		}
	//	}
	//}

	void BossCircleProjectile::Update()
	{
		SetTime(GetTime() + (float)Time::DeltaTime());
		if (GetTime() >= 5.f)
		{
			object::Destroy(GetOwner());
		}

		else if (GetTime() >= 3.5f)
		{
			Vector3 vPos = GetTransform()->GetPosition();
			float Zpos = vPos.z;
			Vector3 moveVector = -GetDir() * GetSpeed() * (float)Time::DeltaTime();
			vPos += moveVector;
			vPos.z = Zpos;
			GetTransform()->SetPosition(vPos);
		}

		else if (GetTime() >= 3.f)
		{
			Vector3 vPos = GetTransform()->GetPosition();
			GetTransform()->SetPosition(vPos);
		}

		else if (GetTime() >= 2.5f)
		{
			Vector3 vPos = GetTransform()->GetPosition();
			float Zpos = vPos.z;
			vPos += GetDir() * GetSpeed() * (float)Time::DeltaTime();
			vPos.z = Zpos;
			GetTransform()->SetPosition(vPos);
		}
	}



	void BossCircleProjectile::OnCollisionEnter(Collider2D* other)
	{
		MonsterProjectileScript::OnCollisionEnter(other);
	}

	void BossCircleProjectile::OnCollisionStay(Collider2D* other)
	{

	}

	void BossCircleProjectile::OnCollisionExit(Collider2D* other)
	{

	}
}