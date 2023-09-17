#include "Lu_BowProjectile.h"
#include "Lu_AudioSource.h"
#include "Lu_Resources.h"
#include "Lu_GameObject.h"
#include "Lu_SceneManager.h"
#include "Lu_SoundManager.h"

namespace Lu
{
	BowProjectile::BowProjectile()
	{
		SetName(L"BowProjectileScript");
		SetType(eWeaponType::Bow);
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
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"BowSFX", L"..\\Resources\\Sound\\SFX\\Player\\BowSFX.ogg"));
		pSFX->Play();
		ProjectileScript::OnCollisionEnter(other);
	}

	void BowProjectile::OnCollisionStay(Collider2D* other)
	{

	}

	void BowProjectile::OnCollisionExit(Collider2D* other)
	{

	}
}