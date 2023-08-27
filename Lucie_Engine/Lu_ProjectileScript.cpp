#include "Lu_ProjectileScript.h"
#include "Lu_GameObject.h"
#include "Lu_Transform.h"
#include "Lu_Time.h"
#include "Lu_Object.h"
#include "Lu_AudioSource.h"

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

		m_SFX = new GameObject;
		m_SFX->AddComponent<AudioSource>();
	}

	ProjectileScript::~ProjectileScript()
	{
		if (nullptr != m_SFX)
		{
			delete m_SFX;
			m_SFX = nullptr;
		}
	}

	void ProjectileScript::Update()
	{
		m_Time += (float)Time::DeltaTime();

		if (m_Time >= 1.5f)
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