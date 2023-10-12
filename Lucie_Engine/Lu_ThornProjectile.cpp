#include "Lu_ThornProjectile.h"
#include "Lu_Animator.h"
#include "Lu_Resources.h"
#include "Lu_Object.h"
#include "Lu_Time.h"

namespace Lu
{
	ThornProjectile::ThornProjectile()
		: m_bFirst(false)
	{
		SetName(L"ThornProjectileScript");
	}

	Lu::ThornProjectile::~ThornProjectile()
	{
	}

	void ThornProjectile::Initialize()
	{
		MonsterProjectileScript::Initialize();
	}

	void ThornProjectile::Update()
	{
		if (!m_bFirst)
		{
			SetTime(GetTime() + (float)Time::DeltaTime());
			if (GetTime() >= 0.6f)
			{
				GetOwner()->AddComponent<Collider2D>()->SetSize(Vector2(0.5f, 0.5f));
				m_bFirst = true;
			}
		}
	}

	void ThornProjectile::CreateProjectileAnimation()
	{
		std::shared_ptr<Texture> pAtlas
			= Resources::Load<Texture>(L"Thorn_TEX", L"..\\Resources\\Texture\\Monster\\Boss\\Thorn.png");

		GetAnimator()->Create(L"Thorn", pAtlas, Vector2(0.f, 0.f), Vector2(270.f, 260.f), 13, Vector2(270.f, 260.f));
		GetAnimator()->CompleteEvent(L"Thorn") = std::bind(&ThornProjectile::ThornComplete, this);

		GetAnimator()->PlayAnimation(L"Thorn", true);
	}

	void ThornProjectile::ThornComplete()
	{
		object::Destroy(GetOwner());
	}
}