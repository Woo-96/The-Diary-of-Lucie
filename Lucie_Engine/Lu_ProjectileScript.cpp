#include "Lu_ProjectileScript.h"
#include "Lu_GameObject.h"
#include "Lu_Transform.h"
#include "Lu_Time.h"
#include "Lu_Object.h"

namespace Lu
{
	ProjectileScript::ProjectileScript()
		: m_PlayerScript(nullptr)
		, m_Transform(nullptr)
		, m_AttackSpeed(1000.f)
		, m_AttackDir(Vector3::Zero)
		, m_Time(0.f)
		, m_Duration(0.3f)
		, m_WeaponType(eWeaponType::None)
		, m_SkillType(eSkillType::None)
	{
		SetName(L"ProjectileScript");
	}

	ProjectileScript::~ProjectileScript()
	{

	}

	void ProjectileScript::Update()
	{
		m_Time += (float)Time::DeltaTime();

		if (m_Time >= m_Duration)
		{
			object::Destroy(GetOwner());
		}

		Vector3 vPos = m_Transform->GetPosition();
		float Zpos = vPos.z;
		vPos += m_AttackDir * m_AttackSpeed * (float)Time::DeltaTime();
		vPos.z = Zpos;
		m_Transform->SetPosition(vPos);
	}

	void ProjectileScript::OnCollisionEnter(Collider2D* _Other)
	{
		object::Destroy(GetOwner());
	}

	void ProjectileScript::OnCollisionStay(Collider2D* _Other)
	{

	}

	void ProjectileScript::OnCollisionExit(Collider2D* _Other)
	{

	}
}