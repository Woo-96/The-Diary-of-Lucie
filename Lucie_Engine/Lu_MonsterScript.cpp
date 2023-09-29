#include "Lu_MonsterScript.h"
#include "Lu_GameObject.h"
#include "Lu_Animator.h"
#include "Lu_Renderer.h"
#include "Lu_PlayerScript.h"
#include "Lu_Time.h"

namespace Lu
{
	MonsterScript::MonsterScript()
		: m_Animator(nullptr)
		, m_Target(nullptr)
		, m_MonsterInfo{}
		, m_AnimDir(eAnimDir::Down)
		, m_PrevAnimDir(eAnimDir::None)
		, m_HitCoolTime(0.f)
	{
		SetName(L"MonsterScript");
	}

	MonsterScript::~MonsterScript()
	{

	}

	void MonsterScript::Initialize()
	{
		m_Animator = GetOwner()->GetComponent<Animator>();
		CreateAnimation();
	}

	void MonsterScript::Update()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();

		graphics::DebugMesh mesh = {};
		mesh.Position = pos;
		mesh.Radius = m_MonsterInfo.DetectRange;
		mesh.Type = enums::eColliderType::Circle;
		mesh.Color = Vector4(0.f, 0.f, 1.f, 1.f);

		renderer::PushDebugMeshAttribute(mesh);
	}

	void MonsterScript::OnCollisionEnter(Collider2D* _Other)
	{
		if ((int)eLayerType::Immovable == _Other->GetOwner()->GetLayerIndex())
			ChangeIdleState();

		if ((int)eLayerType::PlayerProjectile == _Other->GetOwner()->GetLayerIndex())
		{
			GetInfo().HP -= m_Target->GetPlayerDamage();

			if (GetInfo().HP <= 0.f)
			{
				ChangeDeadState();
			}
		}
	}

	void MonsterScript::OnCollisionStay(Collider2D* _Other)
	{
		if ((int)eLayerType::Immovable == _Other->GetOwner()->GetLayerIndex())
			ChangeIdleState();

		if ((int)eLayerType::PlayerProjectile == _Other->GetOwner()->GetLayerIndex())
		{
			m_HitCoolTime += (float)Time::DeltaTime();

			if (m_HitCoolTime >= 0.1f)
			{
				GetInfo().HP -= GetTarget()->GetPlayerDamage();

				if (GetInfo().HP <= 0.f)
				{
					ChangeDeadState();
				}

				m_HitCoolTime = 0;
			}
		}
	}
}