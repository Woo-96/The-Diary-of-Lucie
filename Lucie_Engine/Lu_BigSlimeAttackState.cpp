#include "Lu_BigSlimeAttackState.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_BubbleProjectile.h"
#include "Lu_JumpProjectile.h"
#include "Lu_BigSlimeScript.h"

namespace Lu
{
	BigSlimeAttackState::BigSlimeAttackState()
	{
		SetName(L"BigSlimeAttackStateScript");
		SetStateType(eState::Attack);
	}

	BigSlimeAttackState::~BigSlimeAttackState()
	{

	}

	void BigSlimeAttackState::Enter()
	{
		Vector3 vSpawnPos = GetBigSlimeScript()->GetOwner()->GetComponent<Transform>()->GetPosition();
		vSpawnPos.y -= 100.f;
		vSpawnPos.z = 600.f;

		GameObject* pProjectile = object::Instantiate<GameObject>(vSpawnPos, Vector3(1000.f, 1000.f, 100.f), eLayerType::MonsterProjectile);
		pProjectile->SetName(L"JumpProjectile");

		MeshRenderer* pMeshRender = pProjectile->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		pMeshRender->SetMaterial(Resources::Find<Material>(L"SlimeJumpAnimation_Mtrl"));

		Collider2D* pCollider = pProjectile->AddComponent<Collider2D>();
		pCollider->SetType(eColliderType::Rect);
		pCollider->SetSize(Vector2(0.5f, 0.3f));

		Animator* pAnimator = pProjectile->AddComponent<Animator>();
		std::shared_ptr<Texture> pAtlas
			= Resources::Load<Texture>(L"BigSlimeJump_TEX", L"..\\Resources\\Texture\\Monster\\MidBoss\\BigSlimeJump.png");

		pAnimator->Create(L"BigSlime_JumpProjectile", pAtlas, Vector2(0.f, 0.f), Vector2(192.f, 192.f), 7, Vector2(192.f, 192.f), Vector2::Zero, 0.1f);
		pAnimator->PlayAnimation(L"BigSlime_JumpProjectile", false);

		pProjectile->AddComponent<JumpProjectile>();
	}

	void BigSlimeAttackState::Exit()
	{

	}
}