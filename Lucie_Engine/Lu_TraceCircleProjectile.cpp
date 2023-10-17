#include "Lu_TraceCircleProjectile.h"
#include "Lu_Time.h"
#include "Lu_Object.h"

namespace Lu
{
	TraceCircleProjectile::TraceCircleProjectile()
	{
		SetName(L"TraceCircleProjectileScript");
		SetDuration(5.f);
	}

	TraceCircleProjectile::~TraceCircleProjectile()
	{

	}

	void TraceCircleProjectile::Update()
	{
		SetTime(GetTime() + (float)Time::DeltaTime());

		if (GetTime() >= GetDuration())
		{
			object::Destroy(GetOwner());
		}
		else if (GetTime() >= 0.5f)
		{
			Vector3 vPos = GetTransform()->GetPosition();
			float Zpos = vPos.z;

			Vector3 vMoveDir = GetDir();
			vMoveDir.Normalize();

			Vector3 vMove = vMoveDir * GetSpeed() * (float)Time::DeltaTime();
			vPos += vMove;

			vPos.z = Zpos;
			GetTransform()->SetPosition(vPos);
		}
	}

	void TraceCircleProjectile::OnCollisionEnter(Collider2D* other)
	{
		MonsterProjectileScript::OnCollisionEnter(other);
	}
}