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

	void BossCircleProjectile::Update()
	{
		SetTime(GetTime() + (float)Time::DeltaTime());
		if (GetTime() >= 2.2f)
		{
			object::Destroy(GetOwner());
		}

		else if (GetTime() >= 1.3f)
		{
			Vector3 vPos = GetTransform()->GetPosition();
			float Zpos = vPos.z;
			Vector3 moveVector = -GetDir() * GetSpeed() * (float)Time::DeltaTime();
			vPos += moveVector;
			vPos.z = Zpos;
			GetTransform()->SetPosition(vPos);
		}

		else if (GetTime() >= 1.f)
		{
			Vector3 vPos = GetTransform()->GetPosition();
			GetTransform()->SetPosition(vPos);
		}

		else
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