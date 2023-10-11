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

	void CircleProjectile::Update()
	{
		MonsterProjectileScript::Update();
	}

	void CircleProjectile::OnCollisionEnter(Collider2D* other)
	{
		MonsterProjectileScript::OnCollisionEnter(other);
	}
}