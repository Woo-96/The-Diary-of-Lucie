#include "Lu_AttackState.h"
#include "Lu_Time.h"
#include "Lu_PlayerScript.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_BowProjectile.h"
#include "Lu_WandProjectile.h"
#include "Lu_Input.h"
#include "Lu_Renderer.h"
#include "Lu_AudioSource.h"

namespace Lu
{
	AttackState::AttackState()
		: m_AttackSFX(nullptr)
	{
		SetName(L"AttackStateScript");
		SetStateType(eState::Attack);
	}

	AttackState::~AttackState()
	{
		if (nullptr != m_AttackSFX)
		{
			delete m_AttackSFX;
			m_AttackSFX = nullptr;
		}
	}

	void AttackState::Initialize()
	{
		StateScript::Initialize();

		SetSpeed(50.f);

		m_AttackSFX = new GameObject;
		AudioSource* pAudio = m_AttackSFX->AddComponent<AudioSource>();
		pAudio->SetClip(Resources::Load<AudioClip>(L"AttackSFX", L"..\\Resources\\Sound\\SFX\\Player\\AttackSFX.ogg"));
	}

	void AttackState::Update()
	{
		Vector3 vPos = GetTransform()->GetPosition();
		float Zpos = vPos.z;
		vPos += GetDir() * GetSpeed() * (float)Time::DeltaTime();
		vPos.z = Zpos;
		GetTransform()->SetPosition(vPos);
	}

	void AttackState::Enter()
	{
		GetPlayerScript()->SetAction(true);
		CalDirToMouse();

		CreateProjectile();

		m_AttackSFX->GetComponent<AudioSource>()->Play();
	}

	void AttackState::Exit()
	{
		GetPlayerScript()->SetAction(false);
	}

	void AttackState::CreateProjectile()
	{
		switch (GetPlayerScript()->GetWeaponType())
		{
		case Lu::PlayerScript::eWeaponType::Bow:
		{
			Vector3 vDir = GetDir();
			float angle = std::atan2(vDir.y, vDir.x);

			GameObject* pProjectile = object::Instantiate<GameObject>(GetTransform()->GetPosition(), Vector3(50.f, 50.f, 100.f), Vector3(0.f, 0.f, angle + PI / 2.0f), eLayerType::PlayerProjectile);
			pProjectile->SetName(L"BowProjectile");

			MeshRenderer* pMeshRender = pProjectile->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"BowProjectile_Mtrl"));

			Collider2D* pCollider = pProjectile->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);
			pCollider->SetSize(Vector2(0.3f, 0.6f));

			BowProjectile* pBowProjectile = pProjectile->AddComponent<BowProjectile>();
			pBowProjectile->SetPlayerScript(GetPlayerScript());
			pBowProjectile->SetTransform(pProjectile->GetComponent<Transform>());
			pBowProjectile->SetDir(GetDir());
		}
			break;
		case Lu::PlayerScript::eWeaponType::Wand:
		{
			GameObject* pProjectile = object::Instantiate<GameObject>(GetTransform()->GetPosition(), Vector3(50.f, 50.f, 100.f), eLayerType::PlayerProjectile);
			pProjectile->SetName(L"WandProjectile");

			MeshRenderer* pMeshRender = pProjectile->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"WandProjectile_Mtrl"));

			Collider2D* pCollider = pProjectile->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);
			pCollider->SetSize(Vector2(0.6f, 0.6f));

			WandProjectile* pWandProjectile = pProjectile->AddComponent<WandProjectile>();
			pWandProjectile->SetPlayerScript(GetPlayerScript());
			pWandProjectile->SetTransform(pProjectile->GetComponent<Transform>());
			pWandProjectile->SetDir(GetDir());
		}
			break;
		}
	}
}