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

		SetSpeed(50.f);
	}

	void AttackState::Update()
	{
		Vector3 vPos = GetTransform()->GetPosition();

		float distanceToMove = GetSpeed() * (float)Time::DeltaTime();
		Vector3 moveVector = GetDir() * distanceToMove;

		vPos += moveVector;

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
		case Lu::PlayerScript::eWeaponType::Bow:
		{
			GameObject* pProjectile = object::Instantiate<GameObject>(GetTransform()->GetPosition(), eLayerType::PlayerProjectile);
			pProjectile->SetName(L"BowProjectile");

			Transform* pTransform = pProjectile->GetComponent<Transform>();
			pTransform->SetScale(Vector3(50.f, 50.f, 100.f));
		
			Vector3 rot = pTransform->GetRotation();

			switch (GetPlayerScript()->GetDir())
			{
			case Lu::PlayerScript::ePlayerDir::Up:
			{
				rot.z = float(0.0f + 1.56f);
				pTransform->SetRotation(rot);
			}
			break;
			case Lu::PlayerScript::ePlayerDir::LeftUp:
			{

				rot.z = float(0.78 * 1 + 1.56f);
				pTransform->SetRotation(rot);
			}
			break;
			case Lu::PlayerScript::ePlayerDir::Left:
			{
				rot.z = float(0.78 * 2 + 1.56f);
				pTransform->SetRotation(rot);
			}
			break;
			case Lu::PlayerScript::ePlayerDir::LeftDown:
			{
				rot.z = float(0.78 * 3 + 1.56f);
				pTransform->SetRotation(rot);
			}
			break;
			case Lu::PlayerScript::ePlayerDir::Down:
			{
				rot.z = float(0.78 * 4 + 1.56f);
				pTransform->SetRotation(rot);
			}
			break;
			case Lu::PlayerScript::ePlayerDir::RightDown:
			{
				rot.z = float(0.78 * 5 + 1.56f);
				pTransform->SetRotation(rot);
			}
			break;
			case Lu::PlayerScript::ePlayerDir::Right:
			{
				rot.z = float(0.78 * 6 + 1.56f);
				pTransform->SetRotation(rot);
			}
			break;
			case Lu::PlayerScript::ePlayerDir::RightUp:
			{
				rot.z = float(0.78 * 7 + 1.56f);
				pTransform->SetRotation(rot);
			}
			break;
			}


			MeshRenderer* pMeshRender = pProjectile->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"BowProjectile_Mtrl"));

			Collider2D* pCollider = pProjectile->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);
			pCollider->SetSize(Vector2(0.3f, 0.6f));

			BowProjectile* pBowProjectile = pProjectile->AddComponent<BowProjectile>();
			pBowProjectile->SetPlayerScript(GetPlayerScript());
			pBowProjectile->SetTransform(pTransform);
			pBowProjectile->SetDir(GetDir());
		}
			break;
		case Lu::PlayerScript::eWeaponType::Wand:
		{
			GameObject* pProjectile = object::Instantiate<GameObject>(GetTransform()->GetPosition(), eLayerType::PlayerProjectile);
			pProjectile->SetName(L"WandProjectile");

			Transform* pTransform = pProjectile->GetComponent<Transform>();
			pTransform->SetScale(Vector3(50.f, 50.f, 100.f));

			MeshRenderer* pMeshRender = pProjectile->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"WandProjectile_Mtrl"));

			Collider2D* pCollider = pProjectile->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);
			pCollider->SetSize(Vector2(0.6f, 0.6f));

			WandProjectile* pWandProjectile = pProjectile->AddComponent<WandProjectile>();
			pWandProjectile->SetPlayerScript(GetPlayerScript());
			pWandProjectile->SetTransform(pTransform);
			pWandProjectile->SetDir(GetDir());
		}
			break;
		}
	}
}