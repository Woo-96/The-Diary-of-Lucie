#include "Lu_CircleProjectile.h"

namespace Lu
{
	CircleProjectile::CircleProjectile()
	{
		SetName(L"CircleProjectileScript");
	}

	CircleProjectile::~CircleProjectile()
	{

	}

	void CircleProjectile::Initialize()
	{

	}

	void CircleProjectile::Update()
	{
		MonsterProjectileScript::Update();
	}

	void CircleProjectile::OnCollisionEnter(Collider2D* other)
	{
		MonsterProjectileScript::OnCollisionEnter(other);
	}

	void CircleProjectile::OnCollisionStay(Collider2D* other)
	{

	}

	void CircleProjectile::OnCollisionExit(Collider2D* other)
	{

	}
}