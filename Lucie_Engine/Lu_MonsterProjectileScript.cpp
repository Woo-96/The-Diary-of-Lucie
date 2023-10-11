#include "Lu_MonsterProjectileScript.h"
#include "Lu_GameObject.h"
#include "Lu_Transform.h"
#include "Lu_Time.h"
#include "Lu_Object.h"

namespace Lu
{
	MonsterProjectileScript::MonsterProjectileScript()
		: m_MonsterScript(nullptr)
		, m_Transform(nullptr)
		, m_Animator(nullptr)
		, m_AttackSpeed(200.f)
		, m_AttackDir(Vector3::Zero)
		, m_Time(0.f)
		, m_Duration(2.5f)
	{
		SetName(L"MonsterProjectileScript");
	}

	MonsterProjectileScript::~MonsterProjectileScript()
	{
	}

	void MonsterProjectileScript::Initialize()
	{
		m_Animator = GetOwner()->GetComponent<Animator>();

		if (m_Animator)
			CreateProjectileAnimation();
	}

	void MonsterProjectileScript::Update()
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

	void MonsterProjectileScript::OnCollisionEnter(Collider2D* _Other)
	{
		object::Destroy(GetOwner());
	}
}