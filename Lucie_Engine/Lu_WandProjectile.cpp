#include "Lu_WandProjectile.h"
#include "Lu_AudioSource.h"
#include "Lu_Resources.h"
#include "Lu_GameObject.h"
#include "Lu_SceneManager.h"
#include "Lu_SoundManager.h"

namespace Lu
{
	WandProjectile::WandProjectile()
		: m_bChargeProjectile(false)
	{
		SetName(L"WandProjectileScript");
		SetType(eWeaponType::Wand);
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
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"WandSFX", L"..\\Resources\\Sound\\SFX\\Player\\WandSFX.ogg"));
		pSFX->Play();

		if (!m_bChargeProjectile)
			ProjectileScript::OnCollisionEnter(other);
	}

	void WandProjectile::OnCollisionStay(Collider2D* other)
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"WandSFX", L"..\\Resources\\Sound\\SFX\\Player\\WandSFX.ogg"));
		pSFX->Play();
	}

	void WandProjectile::OnCollisionExit(Collider2D* other)
	{
	}
}