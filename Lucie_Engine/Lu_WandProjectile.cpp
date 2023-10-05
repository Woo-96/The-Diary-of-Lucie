#include "Lu_WandProjectile.h"
#include "Lu_AudioSource.h"
#include "Lu_Resources.h"
#include "Lu_GameObject.h"
#include "Lu_SceneManager.h"
#include "Lu_SoundManager.h"
#include "Lu_Time.h"
#include "Lu_PlayerScript.h"

namespace Lu
{
	WandProjectile::WandProjectile()
		: m_bChargeProjectile(false)
		, m_bFullCharge(false)
		, m_SFXTime(0.f)
	{
		SetName(L"WandProjectileScript");
		SetWeaponType(eWeaponType::Wand);
	}

	WandProjectile::~WandProjectile()
	{
	}

	void WandProjectile::Update()
	{
		ProjectileScript::Update();
	}

	void WandProjectile::OnCollisionEnter(Collider2D* _Other)
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"WandSFX", L"..\\Resources\\Sound\\SFX\\Player\\WandSFX.ogg"));
		pSFX->Play();

		if (!m_bChargeProjectile)
			ProjectileScript::OnCollisionEnter(_Other);
	}

	void WandProjectile::OnCollisionStay(Collider2D* _Other)
	{
		if ((int)eLayerType::Monster == _Other->GetOwner()->GetLayerIndex())
		{
			m_SFXTime += (float)Time::DeltaTime();

			if (m_SFXTime >= 0.1f)
			{
				AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
				pSFX->SetClip(Resources::Load<AudioClip>(L"WandSFX", L"..\\Resources\\Sound\\SFX\\Player\\WandSFX.ogg"));
				pSFX->Play();

				m_SFXTime = 0.f;
			}
		}
	}

	int WandProjectile::GetProjectileDamage()
	{
		if (GetPlayerScript()->GetWeaponType() == eWeaponType::None)
			return GetPlayerScript()->GetPlayerDamage();
		else
		{
			if (m_bChargeProjectile)
			{
				if (m_bFullCharge)
					return GetPlayerScript()->GetPlayerDamage() / 1.5f;
				else
					return GetPlayerScript()->GetPlayerDamage() / 3;
			}
			else
				return GetPlayerScript()->GetPlayerDamage();
		}

		return 0;
	}
}