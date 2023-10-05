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

namespace Lu
{
	AttackState::AttackState()
	{
		SetName(L"AttackStateScript");
		SetStateType(eState::Attack);
	}

	AttackState::~AttackState()
	{

	}

	void AttackState::Initialize()
	{
		StateScript::Initialize();
	}

	void AttackState::Update()
	{
		Vector3 vPos = GetTransform()->GetPosition();
		float diagonalFactor = 1.0f / sqrt(2.0f);

		if (Input::GetKey(eKeyCode::A)
			&& Input::GetKey(eKeyCode::D))
		{
			SetSpeed(50.f);

			float Zpos = vPos.z;
			vPos += GetDir() * GetSpeed() * (float)Time::DeltaTime();
			vPos.z = Zpos;
		}
		else if (Input::GetKey(eKeyCode::W)
			&& Input::GetKey(eKeyCode::S))
		{
			SetSpeed(50.f);

			float Zpos = vPos.z;
			vPos += GetDir() * GetSpeed() * (float)Time::DeltaTime();
			vPos.z = Zpos;
		}
		else if (Input::GetKey(eKeyCode::A)
			&& Input::GetKey(eKeyCode::W))
		{
			SetSpeed(100.f);

			vPos.x -= GetSpeed() * diagonalFactor * (float)Time::DeltaTime();
			vPos.y += GetSpeed() * diagonalFactor * (float)Time::DeltaTime();
		}
		else if (Input::GetKey(eKeyCode::A)
			&& Input::GetKey(eKeyCode::S))
		{
			SetSpeed(100.f);

			vPos.x -= GetSpeed() * diagonalFactor * (float)Time::DeltaTime();
			vPos.y -= GetSpeed() * diagonalFactor * (float)Time::DeltaTime();
		}
		else if (Input::GetKey(eKeyCode::D)
			&& Input::GetKey(eKeyCode::S))
		{
			SetSpeed(100.f);

			vPos.x += GetSpeed() * diagonalFactor * (float)Time::DeltaTime();
			vPos.y -= GetSpeed() * diagonalFactor * (float)Time::DeltaTime();
		}
		else if (Input::GetKey(eKeyCode::D)
			&& Input::GetKey(eKeyCode::W))
		{
			SetSpeed(100.f);

			vPos.x += GetSpeed() * diagonalFactor * (float)Time::DeltaTime();
			vPos.y += GetSpeed() * diagonalFactor * (float)Time::DeltaTime();
		}
		else if (Input::GetKey(eKeyCode::A))
		{
			SetSpeed(100.f);

			vPos.x -= GetSpeed() * (float)Time::DeltaTime();
		}
		else if (Input::GetKey(eKeyCode::D))
		{
			SetSpeed(100.f);

			vPos.x += GetSpeed() * (float)Time::DeltaTime();
		}
		else if (Input::GetKey(eKeyCode::S))
		{
			SetSpeed(100.f);

			vPos.y -= GetSpeed() * (float)Time::DeltaTime();
		}
		else if (Input::GetKey(eKeyCode::W))
		{
			SetSpeed(100.f);

			vPos.y += GetSpeed() * (float)Time::DeltaTime();
		}
		else
		{
			SetSpeed(50.f);

			float Zpos = vPos.z;
			vPos += GetDir() * GetSpeed() * (float)Time::DeltaTime();
			vPos.z = Zpos;
		}

		GetTransform()->SetPosition(vPos);
	}

	void AttackState::Enter()
	{
		GetPlayerScript()->SetAction(true);
		CalDirToMouse();

		CreateProjectile();
	}

	void AttackState::Exit()
	{
		GetPlayerScript()->SetAction(false);
	}

	void AttackState::CreateProjectile()
	{
		switch (GetPlayerScript()->GetWeaponType())
		{
		case eWeaponType::Bow:
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
			pBowProjectile->SetTransform(pProjectile->GetComponent<Transform>());
			pBowProjectile->SetPlayerScript(GetPlayerScript());
			pBowProjectile->SetDir(GetDir());

			GetPlayerScript()->UseStamina(1.f);
		}
		break;
		case eWeaponType::None:
		case eWeaponType::Wand:
		{
			float fChargeGauge = GetPlayerScript()->GetChargeGauge();

			if (fChargeGauge <= 0.167f)
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
				pWandProjectile->SetTransform(pProjectile->GetComponent<Transform>());
				pWandProjectile->SetPlayerScript(GetPlayerScript());
				pWandProjectile->SetDir(GetDir());
				pWandProjectile->SetDuration(0.4f);

				GetPlayerScript()->UseStamina(1.f);
			}
			else if (fChargeGauge >= 1.f)
			{
				GameObject* pProjectile = object::Instantiate<GameObject>(GetTransform()->GetPosition(), Vector3(120.f, 120.f, 100.f), eLayerType::PlayerProjectile);
				pProjectile->SetName(L"WandProjectile");

				MeshRenderer* pMeshRender = pProjectile->AddComponent<MeshRenderer>();
				pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
				pMeshRender->SetMaterial(Resources::Find<Material>(L"WandChanneling_B_Mtrl"));

				Collider2D* pCollider = pProjectile->AddComponent<Collider2D>();
				pCollider->SetType(eColliderType::Rect);
				pCollider->SetSize(Vector2(0.6f, 0.6f));

				WandProjectile* pWandProjectile = pProjectile->AddComponent<WandProjectile>();
				pWandProjectile->SetTransform(pProjectile->GetComponent<Transform>());
				pWandProjectile->SetPlayerScript(GetPlayerScript());
				pWandProjectile->SetDir(GetDir());
				pWandProjectile->SetChargeProjectile(true);
				pWandProjectile->SetFullCharge(true);
				pWandProjectile->SetSpeed(500.f);
				pWandProjectile->SetDuration(1.f);
			}
			else
			{
				Vector2 vScale = Vector2(120.f, 120.f) * fChargeGauge;

				GameObject* pProjectile = object::Instantiate<GameObject>(GetTransform()->GetPosition(), Vector3(vScale.x, vScale.y, 100.f), eLayerType::PlayerProjectile);
				pProjectile->SetName(L"WandProjectile");

				MeshRenderer* pMeshRender = pProjectile->AddComponent<MeshRenderer>();
				pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
				pMeshRender->SetMaterial(Resources::Find<Material>(L"WandChanneling_A_Mtrl"));

				Collider2D* pCollider = pProjectile->AddComponent<Collider2D>();
				pCollider->SetType(eColliderType::Rect);
				pCollider->SetSize(Vector2(0.6f, 0.6f));

				WandProjectile* pWandProjectile = pProjectile->AddComponent<WandProjectile>();
				pWandProjectile->SetTransform(pProjectile->GetComponent<Transform>());
				pWandProjectile->SetPlayerScript(GetPlayerScript());
				pWandProjectile->SetDir(GetDir());
				pWandProjectile->SetChargeProjectile(true);
				pWandProjectile->SetSpeed(500.f);
				pWandProjectile->SetDuration(1.f);
			}

			GetPlayerScript()->ResetChargeGauge();
		}
		break;
		}
	}
}