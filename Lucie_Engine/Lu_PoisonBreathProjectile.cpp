#include "Lu_PoisonBreathProjectile.h"
#include "Lu_Animator.h"
#include "Lu_Resources.h"
#include "Lu_Time.h"
#include "Lu_Object.h"

namespace Lu
{
	PoisonBreathProjectile::PoisonBreathProjectile()
		: m_Degree(0.f)
	{
		SetName(L"PoisonBreathProjectileScript");
		SetDuration(5.f);
		SetSpeed(300.f);
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
		SetTime(GetTime() + (float)Time::DeltaTime());

		if (GetTime() >= GetDuration())
		{
			object::Destroy(GetOwner());
		}
		
		Vector3 vPos = GetTransform()->GetPosition();
		float Zpos = vPos.z;


		float fRadian = (m_Degree * PI) / 180.f;

		vPos.x += GetSpeed() * cosf(fRadian) * Time::DeltaTime();
		vPos.y -= GetSpeed() * sinf(fRadian) * Time::DeltaTime();


		vPos.z = Zpos;
		GetTransform()->SetPosition(vPos);
	}

	void PoisonBreathProjectile::CreateProjectileAnimation()
	{
		std::shared_ptr<Texture> pAtlas
			= Resources::Load<Texture>(L"PoisonBreath_TEX", L"..\\Resources\\Texture\\Monster\\Boss\\PoisonBreath.png");

		GetAnimator()->Create(L"PoisonBreath", pAtlas, Vector2(0.f, 0.f), Vector2(160, 160.f), 10, Vector2(160.f, 160.f));
		GetAnimator()->PlayAnimation(L"PoisonBreath", true);
	}
}