#include "Lu_WandProjectile.h"

namespace Lu
{
	WandProjectile::WandProjectile()
	{
		SetName(L"WandProjectileScript");
		SetType(Lu::PlayerScript::eWeaponType::Wand);
	}

	WandProjectile::~WandProjectile()
	{
	}

	void WandProjectile::Initialize()
	{

	}

	void WandProjectile::Update()
	{
		ProjectileScript::Update();
	}

	void WandProjectile::OnCollisionEnter(Collider2D* other)
	{
		ProjectileScript::OnCollisionEnter(other);
	}

	void WandProjectile::OnCollisionStay(Collider2D* other)
	{
	}

	void WandProjectile::OnCollisionExit(Collider2D* other)
	{
	}
}