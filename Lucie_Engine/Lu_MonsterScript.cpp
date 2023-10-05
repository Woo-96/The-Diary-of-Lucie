#include "Lu_MonsterScript.h"
#include "Lu_GameObject.h"
#include "Lu_Animator.h"
#include "Lu_Renderer.h"
#include "Lu_PlayerScript.h"
#include "Lu_Time.h"
#include "Lu_ProjectileScript.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_DamageFontScript.h"
#include "Lu_Resources.h"

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
			ProjectileScript* pProjectile = _Other->GetOwner()->GetComponent<ProjectileScript>();
			InflictDamage(pProjectile->GetProjectileDamage());

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
				ProjectileScript* pProjectile = _Other->GetOwner()->GetComponent<ProjectileScript>();
				InflictDamage(pProjectile->GetProjectileDamage());

				if (GetInfo().HP <= 0.f)
				{
					ChangeDeadState();
				}

				m_HitCoolTime = 0;
			}
		}
	}

	void MonsterScript::InflictDamage(int _Damage)
	{
		// 피해 적용
		GetInfo().HP -= _Damage;

		// 대미지 폰트 생성
		Vector3 vPos = GetOwner()->GetComponent<Collider2D>()->GetPosition();
		vPos.y += GetOwner()->GetComponent<Transform>()->GetScale().y / 2.f;
		vPos.z = 200.f;
		GameObject* pObject = object::Instantiate<GameObject>(vPos, Vector3(20.f, 20.f, 100.f), eLayerType::DamageFont);
		pObject->SetName(L"DamageFont");
		MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		pMeshRender->SetMaterial(Resources::Find<Material>(L"DamageFont_Mtrl"));
		DamageFontScript* pDamageFont = pObject->AddComponent<DamageFontScript>();
		pDamageFont->SetDamage(_Damage);
	}
}