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
	//	// 소환 후 오른쪽 포물선 형태로 이동 (각도를 어떻게 설정하지?)
	//	Vector3 vPos = GetTransform()->GetPosition();



	//	GetTransform()->SetPosition(vPos);


	//	// 2.5초후 다시 시작 위치로 되돌아감
	//	SetTime(GetTime() + (float)Time::DeltaTime());

	//	if (GetTime() >= 2.5f)
	//	{




	//		// 5초후 소멸 (같은 속도로 퍼져나갔다가 되돌아오니까 2.5초 이동 후 2.5초 되돌아오면 시작 지점이겠지?)
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