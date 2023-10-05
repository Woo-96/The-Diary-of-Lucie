#include "Lu_BowProjectile.h"
#include "Lu_AudioSource.h"
#include "Lu_Resources.h"
#include "Lu_GameObject.h"
#include "Lu_SceneManager.h"
#include "Lu_SoundManager.h"
#include "Lu_PlayerScript.h"

namespace Lu
{
	BowProjectile::BowProjectile()
	{
		SetName(L"BowProjectileScript");
		SetWeaponType(eWeaponType::Bow);
	}

	BowProjectile::~BowProjectile()
	{

	}

	void BowProjectile::Update()
	{
		ProjectileScript::Update();
	}

	void BowProjectile::OnCollisionEnter(Collider2D* _Other)
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"BowSFX", L"..\\Resources\\Sound\\SFX\\Player\\BowSFX.ogg"));
		pSFX->Play();
		ProjectileScript::OnCollisionEnter(_Other);
	}

	int BowProjectile::GetProjectileDamage()
	{
		return GetPlayerScript()->GetPlayerDamage();
	}
}