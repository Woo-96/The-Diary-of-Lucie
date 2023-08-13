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
		, m_AttackSpeed(200.f)
		, m_AttackDir(Vector3::Zero)
		, m_Time(0.f)
	{
		SetName(L"MonsterProjectileScript");
	}

	MonsterProjectileScript::~MonsterProjectileScript()
	{
	}

	void MonsterProjectileScript::Initialize()
	{

	}

	void MonsterProjectileScript::Update()
	{
		m_Time += (float)Time::DeltaTime();

		if (m_Time >= 2.5f)
		{
			object::Destroy(GetOwner());
		}

		Vector3 vPos = m_Transform->GetPosition();
		Vector3 moveVector = m_AttackDir * m_AttackSpeed * (float)Time::DeltaTime();
		vPos += moveVector;
		m_Transform->SetPosition(vPos);
	}

	void MonsterProjectileScript::OnCollisionEnter(Collider2D* _Other)
	{
		object::Destroy(GetOwner());
	}

	void MonsterProjectileScript::OnCollisionStay(Collider2D* _Other)
	{

	}

	void MonsterProjectileScript::OnCollisionExit(Collider2D* _Other)
	{

	}
}