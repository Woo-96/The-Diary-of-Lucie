#include "Lu_CraterState.h"
#include "Lu_EntScript.h"
#include "Lu_Animator.h"
#include "Lu_Time.h"
#include "Lu_CraterProjectile.h"
#include "Lu_PlayerScript.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"

namespace Lu
{
	CraterState::CraterState()
		: m_bFirst(false)
	{
		SetName(L"CraterStateScript");
		SetStateType(eState::Attack_Crater);
	}

	CraterState::~CraterState()
	{
	}

	void CraterState::Initialize()
	{
		EntAttackState::Initialize();
		GetEntScript()->SetAttackType(EntScript::eAttackType::Crater);
	}

	void CraterState::Update()
	{
		SetTime(GetTime() + (float)Time::DeltaTime());

		if (GetTime() >= 0.3f)
		{
			if (!m_bFirst)
			{
				CreateProjectile();

				m_bFirst = true;
			}
		}
	}

	void CraterState::Enter()
	{
		EntAttackState::Enter();
	}

	void CraterState::Exit()
	{
		EntAttackState::Exit();

		m_bFirst = false;
	}

	void CraterState::ChangeAnimation()
	{
		switch (GetEntScript()->GetPhase())
		{
		case EntScript::ePhase::Phase_1:
			GetEntScript()->GetAnimator()->PlayAnimation(L"Ent_Howling", true);
			break;
		case EntScript::ePhase::Phase_2:
			GetEntScript()->GetAnimator()->PlayAnimation(L"Ent_Howling_Phase2", true);
			break;
		}
	}

	void CraterState::CreateProjectile()
	{
		Vector3 vSpawnPos = GetTarget()->GetOwner()->GetComponent<Transform>()->GetPosition();
		vSpawnPos.y -= 30.f;

		GameObject* pProjectile = object::Instantiate<GameObject>(vSpawnPos, Vector3(500.f, 500.f, 100.f), eLayerType::MonsterProjectile);
		pProjectile->SetName(L"Crater");

		MeshRenderer* pMeshRender = pProjectile->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		pMeshRender->SetMaterial(Resources::Find<Material>(L"Ent_Attack_Crater_Mtrl"));

		pProjectile->AddComponent<Animator>();

		CraterProjectile* pProjectileScript = pProjectile->AddComponent<CraterProjectile>();
		pProjectileScript->SetMonsterScript((MonsterScript*)this);
		pProjectileScript->SetTransform(pProjectile->GetComponent<Transform>());
	}
}