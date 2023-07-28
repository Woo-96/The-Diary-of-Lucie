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
		, m_ProjectileType(Lu::PlayerScript::eWeaponType::None)
		, m_AttackSpeed(1000.f)
		, m_AttackDir(Vector3::Zero)
		, m_Time(0.f)
	{
		SetName(L"ProjectileScript");
	}

	ProjectileScript::~ProjectileScript()
	{
	}

	void ProjectileScript::Initialize()
	{

	}

	void ProjectileScript::Update()
	{
		m_Time += (float)Time::DeltaTime();

		if (m_Time >= 1.5f)
		{
			object::Destroy(GetOwner());
		}

		Vector3 vPos = m_Transform->GetPosition();

		float distanceToMove = m_AttackSpeed * (float)Time::DeltaTime();
		Vector3 moveVector = m_AttackDir * distanceToMove;

		vPos += moveVector;

		m_Transform->SetPosition(vPos);
	}

	void ProjectileScript::OnCollisionEnter(Collider2D* other)
	{
		object::Destroy(GetOwner());
	}

	void ProjectileScript::OnCollisionStay(Collider2D* other)
	{

	}

	void ProjectileScript::OnCollisionExit(Collider2D* other)
	{

	}
}