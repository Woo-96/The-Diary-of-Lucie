#include "Lu_WindBreathProjectile.h"
#include "Lu_Animator.h"
#include "Lu_Resources.h"

namespace Lu
{
	WindBreathProjectile::WindBreathProjectile()
	{
		SetName(L"WindBreathProjectileScript");
	}

	WindBreathProjectile::~WindBreathProjectile()
	{
	}

	void WindBreathProjectile::Initialize()
	{
		MonsterProjectileScript::Initialize();
	}

	void WindBreathProjectile::Update()
	{
	}

	void WindBreathProjectile::OnCollisionEnter(Collider2D* other)
	{
	}

	void WindBreathProjectile::OnCollisionStay(Collider2D* other)
	{
	}

	void WindBreathProjectile::OnCollisionExit(Collider2D* other)
	{
	}

	void WindBreathProjectile::CreateProjectileAnimation()
	{
		std::shared_ptr<Texture> pAtlas
			= Resources::Load<Texture>(L"WindBreath_TEX", L"..\\Resources\\Texture\\Monster\\Boss\\WindBreath.png");

		GetAnimator()->Create(L"WindBreath", pAtlas, Vector2(0.f, 0.f), Vector2(60, 60.f), 9, Vector2(60.f, 60.f));
		//GetAnimator()->CompleteEvent(L"Crater") = std::bind(&CreateProjectileAnimation::ThornComplete, this);
	
		GetAnimator()->PlayAnimation(L"WindBreath", true);
	}
}