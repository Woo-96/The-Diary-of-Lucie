#include "Lu_BowProjectile.h"

namespace Lu
{
	BowProjectile::BowProjectile()
	{
		SetName(L"BowProjectileScript");
		SetType(Lu::PlayerScript::eWeaponType::Bow);
	}

	BowProjectile::~BowProjectile()
	{

	}

	void BowProjectile::Initialize()
	{

	}

	void BowProjectile::Update()
	{
		ProjectileScript::Update();
	}

	void BowProjectile::OnCollisionEnter(Collider2D* other)
	{
		ProjectileScript::OnCollisionEnter(other);
	}

	void BowProjectile::OnCollisionStay(Collider2D* other)
	{

	}

	void BowProjectile::OnCollisionExit(Collider2D* other)
	{

	}
}