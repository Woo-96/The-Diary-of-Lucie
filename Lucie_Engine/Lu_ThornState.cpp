#include "Lu_ThornState.h"
#include "Lu_EntScript.h"
#include "Lu_Time.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_PlayerScript.h"
#include "Lu_Resources.h"
#include "Lu_ThornProjectile.h"
#include "Lu_SoundManager.h"
#include "Lu_AudioSource.h"

namespace Lu
{
	ThornState::ThornState()
		: m_bFirst(false)
		, m_FX(nullptr)
	{
		SetName(L"ThornStateScript");
		SetStateType(eState::Attack_Thorn);
	}

	ThornState::~ThornState()
	{
	}

	void ThornState::Initialize()
	{
		EntAttackState::Initialize();
		GetEntScript()->SetAttackType(EntScript::eAttackType::Thorn);
	}

	void ThornState::Update()
	{
		SetTime(GetTime() + (float)Time::DeltaTime());

		if (GetTime() >= 3.f)
		{
			GetEntScript()->ChangeState(eState::Idle);
		}
		else if (GetTime() >= 0.5f)
		{
			if (!m_bFirst)
			{
				Vector3 vSpawnPos = GetEntScript()->GetOwner()->GetComponent<Transform>()->GetPosition();
				vSpawnPos.y -= 300.f;
				m_FX = object::Instantiate<GameObject>(vSpawnPos, Vector3(480.f, 480.f, 100.f), eLayerType::FX);
				m_FX->SetName(L"EntThornFX");
				MeshRenderer* pMeshRender = m_FX->AddComponent<MeshRenderer>();
				pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
				pMeshRender->SetMaterial(Resources::Find<Material>(L"WandCharge_Mtrl"));
				Animator* pAnimator = m_FX->AddComponent<Animator>();
				pAnimator->Create(L"EntThornFX", Resources::Load<Texture>(L"WandCharge_Tex", L"..\\Resources\\Texture\\Player\\WandCharge.png")
					, Vector2(0.f, 0.f), Vector2(192.f, 192.f), 10, Vector2(192.f, 192.f), Vector2::Zero, 0.1f);
				pAnimator->StartEvent(L"EntThornFX") = std::bind(&ThornState::StartFX, this);
				pAnimator->CompleteEvent(L"EntThornFX") = std::bind(&ThornState::FinishedFX, this);

				pAnimator->PlayAnimation(L"EntThornFX", true);

				m_bFirst = true;
			}

			SetAttackCoolTime(GetAttackCoolTime() + (float)Time::DeltaTime());

			if (GetAttackCoolTime() >= 0.4f)
			{
				CreateProjectile();
				SetAttackCoolTime(0.f);
			}
		}
	}

	void ThornState::Enter()
	{
		EntAttackState::Enter();
	}

	void ThornState::Exit()
	{
		EntAttackState::Exit();

		m_bFirst = false;
	}

	void ThornState::ChangeAnimation()
	{
		switch (GetEntScript()->GetPhase())
		{
		case EntScript::ePhase::Phase_1:
			GetEntScript()->GetAnimator()->PlayAnimation(L"Ent_Attack_Change_Phase1", false);
			break;
		case EntScript::ePhase::Phase_2:
			GetEntScript()->GetAnimator()->PlayAnimation(L"Ent_Attack_Change_Phase2", false);
			break;
		}
	}

	void ThornState::CreateProjectile()
	{
		Vector3 vSpawnPos = GetTarget()->GetOwner()->GetComponent<Transform>()->GetPosition();
		vSpawnPos.y -= 30.f;

		GameObject* pProjectile = object::Instantiate<GameObject>(vSpawnPos, Vector3(270.f, 260.f, 100.f), eLayerType::MonsterProjectile);
		pProjectile->SetName(L"Thorn");

		MeshRenderer* pMeshRender = pProjectile->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		pMeshRender->SetMaterial(Resources::Find<Material>(L"Ent_Attack_Thorn_Mtrl"));

		pProjectile->AddComponent<Animator>();

		ThornProjectile* pProjectileScript = pProjectile->AddComponent<ThornProjectile>();
		pProjectileScript->SetMonsterScript((MonsterScript*)this);
		pProjectileScript->SetTransform(pProjectile->GetComponent<Transform>());
	
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"EntThornSFX", L"..\\Resources\\Sound\\SFX\\Monster\\Ent\\EntThornSFX.ogg"));
		pSFX->Play();
	}

	void ThornState::StartFX()
	{
		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"EntChargeSFX", L"..\\Resources\\Sound\\SFX\\Monster\\Ent\\EntChargeSFX.ogg"));
		pSFX->Play();
	}

	void ThornState::FinishedFX()
	{
		if (m_FX)
		{
			object::Destroy(m_FX);
			m_FX = nullptr;
		}
	}
}