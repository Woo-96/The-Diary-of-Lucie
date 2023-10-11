#include "Lu_CraterProjectile.h"
#include "Lu_Object.h"
#include "Lu_Resources.h"
#include "Lu_Animator.h"

namespace Lu
{
	CraterProjectile::CraterProjectile()
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
	}

	void CraterProjectile::OnCollisionEnter(Collider2D* other)
	{
	}

	void CraterProjectile::OnCollisionStay(Collider2D* other)
	{
	}

	void CraterProjectile::OnCollisionExit(Collider2D* other)
	{
	}

	void CraterProjectile::CreateProjectileAnimation()
	{
		std::shared_ptr<Texture> pAtlas
			= Resources::Load<Texture>(L"Crater_TEX", L"..\\Resources\\Texture\\Monster\\Boss\\Crater.png");

		GetAnimator()->Create(L"Crater", pAtlas, Vector2(0.f, 0.f), Vector2(192, 192.f), 8, Vector2(192.f, 192.f));
		//GetAnimator()->CompleteEvent(L"Crater") = std::bind(&CreateProjectileAnimation::ThornComplete, this);
	
		GetAnimator()->PlayAnimation(L"Crater", true);
	}
}