#include "Lu_SkillCastScript.h"
#include "Lu_Animator.h"
#include "Lu_Resources.h"
#include "Lu_Object.h"
#include "Lu_GameObject.h"
#include "Lu_SoundManager.h"
#include "Lu_AudioSource.h"
#include "Lu_MeshRenderer.h"
#include "Lu_FireStrikeScript.h"

namespace Lu
{
	SkillCastScript::SkillCastScript()
		: m_Animator(nullptr)
		, m_SpawnPos(Vector3::Zero)
	{
		SetName(L"SkillCastScript");
	}

	SkillCastScript::~SkillCastScript()
	{
	}

	void SkillCastScript::Initialize()
	{
		// 애니메이션 생성
		m_Animator = GetOwner()->GetComponent<Animator>();
		if (m_Animator)
		{
			m_Animator->Create(L"CastFX", Resources::Load<Texture>(L"CastFX_Tex", L"..\\Resources\\Texture\\Player\\CastFX.png")
				, Vector2::Zero, Vector2(192.f, 192.f), 13, Vector2(192.f, 192.f), Vector2::Zero, 0.1f);
			m_Animator->CompleteEvent(L"CastFX") = std::bind(&SkillCastScript::CompleteAnimation, this);
			m_Animator->PlayAnimation(L"CastFX", true);
		}
	}

	void SkillCastScript::CompleteAnimation()
	{
		// 파이어 스트라이크 투사체 생성
		GameObject* pProjectile = object::Instantiate<GameObject>(m_SpawnPos, Vector3(384.f, 384.f, 100.f), eLayerType::PlayerProjectile);
		pProjectile->SetName(L"FireStrike");
		MeshRenderer* pMeshRender = pProjectile->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		pMeshRender->SetMaterial(Resources::Find<Material>(L"FireStrike_Mtrl"));
		Collider2D* pCollider = pProjectile->AddComponent<Collider2D>();
		pCollider->SetSize(Vector2(0.5f, 0.3f));
		pCollider->SetCenter(Vector2(0.f, -50.f));
		pProjectile->AddComponent<Animator>();
		pProjectile->AddComponent<FireStrikeScript>()->SetPlayerScript(SceneManager::FindPlayer()->GetComponent<PlayerScript>());

		AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
		pSFX->SetClip(Resources::Load<AudioClip>(L"FireStrikeSFX", L"..\\Resources\\Sound\\SFX\\Player\\FireStrikeSFX.ogg"));
		pSFX->Play();

		// 캐스트 이펙트 제거
		object::Destroy(GetOwner());
	}
}