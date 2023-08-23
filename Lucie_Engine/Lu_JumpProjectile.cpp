#include "Lu_JumpProjectile.h"
#include "Lu_Time.h"
#include "Lu_Object.h"

namespace Lu
{
	JumpProjectile::JumpProjectile()
	{
	}

	JumpProjectile::~JumpProjectile()
	{
	}

	void JumpProjectile::Initialize()
	{
	}

	void JumpProjectile::Update()
	{
		SetTime(GetTime() + (float)Time::DeltaTime());

		if (GetTime() >= 0.7f)
		{
			object::Destroy(GetOwner());
		}
	}

	void JumpProjectile::OnCollisionEnter(Collider2D* other)
	{
	}

	void JumpProjectile::OnCollisionStay(Collider2D* other)
	{
	}

	void JumpProjectile::OnCollisionExit(Collider2D* other)
	{
	}
}