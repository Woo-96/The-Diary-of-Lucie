#include "Lu_CraterProjectile.h"
#include "Lu_Object.h"
#include "Lu_Resources.h"
#include "Lu_Animator.h"
#include "Lu_Time.h"

namespace Lu
{
	CraterProjectile::CraterProjectile()
		: m_bFirst(false)
	{
		SetName(L"CraterProjectileScript");
	}

	CraterProjectile::~CraterProjectile()
	{
	}

	void CraterProjectile::Initialize()
	{
		MonsterProjectileScript::Initialize();
	}

	void CraterProjectile::Update()
	{
		if (!m_bFirst)
		{
			SetTime(GetTime() + (float)Time::DeltaTime());
			if (GetTime() >= 0.3f)
			{
				GetOwner()->AddComponent<Collider2D>()->SetSize(Vector2(0.5f, 0.5f));
				m_bFirst = true;
			}
		}
	}

	void CraterProjectile::OnCollisionEnter(Collider2D* other)
	{
		//if (!(GetOwner()->DeleteComponent<Collider2D>()))
		//	assert(nullptr);
	}

	void CraterProjectile::CreateProjectileAnimation()
	{
		std::shared_ptr<Texture> pAtlas
			= Resources::Load<Texture>(L"Crater_TEX", L"..\\Resources\\Texture\\Monster\\Boss\\Crater.png");

		GetAnimator()->Create(L"Crater", pAtlas, Vector2(0.f, 0.f), Vector2(192, 192.f), 8, Vector2(192.f, 192.f));
		GetAnimator()->CompleteEvent(L"Crater") = std::bind(&CraterProjectile::AnimationComplete, this);
		GetAnimator()->PlayAnimation(L"Crater", true);
	}

	void CraterProjectile::AnimationComplete()
	{
		object::Destroy(GetOwner());
	}
}