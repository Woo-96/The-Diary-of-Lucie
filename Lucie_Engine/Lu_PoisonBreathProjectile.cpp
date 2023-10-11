#include "Lu_PoisonBreathProjectile.h"
#include "Lu_Animator.h"
#include "Lu_Resources.h"

namespace Lu
{
	PoisonBreathProjectile::PoisonBreathProjectile()
	{
		SetName(L"PoisonBreathProjectileScript");
	}

	PoisonBreathProjectile::~PoisonBreathProjectile()
	{
	}

	void PoisonBreathProjectile::Initialize()
	{
		MonsterProjectileScript::Initialize();
	}

	void PoisonBreathProjectile::Update()
	{
	}

	void PoisonBreathProjectile::OnCollisionEnter(Collider2D* other)
	{
	}

	void PoisonBreathProjectile::OnCollisionStay(Collider2D* other)
	{
	}

	void PoisonBreathProjectile::OnCollisionExit(Collider2D* other)
	{
	}

	void PoisonBreathProjectile::CreateProjectileAnimation()
	{
		std::shared_ptr<Texture> pAtlas
			= Resources::Load<Texture>(L"PoisonBreath_TEX", L"..\\Resources\\Texture\\Monster\\Boss\\PoisonBreath.png");

		GetAnimator()->Create(L"PoisonBreath", pAtlas, Vector2(0.f, 0.f), Vector2(160, 160.f), 10, Vector2(160.f, 160.f));
		//GetAnimator()->CompleteEvent(L"Crater") = std::bind(&CreateProjectileAnimation::ThornComplete, this);

		GetAnimator()->PlayAnimation(L"WindBreath", true);
	}
}