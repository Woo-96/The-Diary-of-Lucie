#include "Lu_WandProjectile.h"
#include "Lu_AudioSource.h"
#include "Lu_Resources.h"
#include "Lu_GameObject.h"

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

	void WandProjectile::Update()
	{
		ProjectileScript::Update();
	}

	void WandProjectile::OnCollisionEnter(Collider2D* other)
	{
		AudioSource* pAudio = GetSFX()->GetComponent<AudioSource>();
		pAudio->SetClip(Resources::Load<AudioClip>(L"WandSFX", L"..\\Resources\\Sound\\SFX\\Player\\WandSFX.ogg"));
		pAudio->Play();
		ProjectileScript::OnCollisionEnter(other);
	}

	void WandProjectile::OnCollisionStay(Collider2D* other)
	{
	}

	void WandProjectile::OnCollisionExit(Collider2D* other)
	{
	}
}