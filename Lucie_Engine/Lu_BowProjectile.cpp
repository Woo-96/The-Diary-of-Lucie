#include "Lu_BowProjectile.h"
#include "Lu_AudioSource.h"
#include "Lu_Resources.h"
#include "Lu_GameObject.h"

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

	void BowProjectile::Update()
	{
		ProjectileScript::Update();
	}

	void BowProjectile::OnCollisionEnter(Collider2D* other)
	{
		AudioSource* pAudio = GetSFX()->GetComponent<AudioSource>();
		pAudio->SetClip(Resources::Load<AudioClip>(L"BowSFX", L"..\\Resources\\Sound\\SFX\\Player\\BowSFX.ogg"));
		pAudio->Play();
		ProjectileScript::OnCollisionEnter(other);
	}

	void BowProjectile::OnCollisionStay(Collider2D* other)
	{

	}

	void BowProjectile::OnCollisionExit(Collider2D* other)
	{

	}
}