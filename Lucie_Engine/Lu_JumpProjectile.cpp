#include "Lu_JumpProjectile.h"
#include "Lu_Time.h"
#include "Lu_Object.h"
#include "Lu_Resources.h"
#include "Lu_Animator.h"

namespace Lu
{
	JumpProjectile::JumpProjectile()
	{
		SetName(L"JumpProjectileScript");
	}

	JumpProjectile::~JumpProjectile()
	{
	}

	void JumpProjectile::Initialize()
	{
		MonsterProjectileScript::Initialize();
	}

	void JumpProjectile::Update()
	{
		SetTime(GetTime() + (float)Time::DeltaTime());

		if (GetTime() >= 0.7f)
		{
			object::Destroy(GetOwner());
		}
	}

	void JumpProjectile::CreateProjectileAnimation()
	{
		std::shared_ptr<Texture> pAtlas
			= Resources::Load<Texture>(L"BigSlimeJump_TEX", L"..\\Resources\\Texture\\Monster\\MidBoss\\BigSlimeJump.png");

		GetAnimator()->Create(L"BigSlime_JumpProjectile", pAtlas, Vector2(0.f, 0.f), Vector2(192.f, 192.f), 7, Vector2(192.f, 192.f), Vector2::Zero, 0.1f);
		GetAnimator()->PlayAnimation(L"BigSlime_JumpProjectile", false);
	}
}