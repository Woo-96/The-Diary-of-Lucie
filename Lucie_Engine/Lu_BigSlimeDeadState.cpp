#include "Lu_BigSlimeDeadState.h"
#include "Lu_BigSlimeScript.h"
#include "Lu_GameObject.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_SlimeScript.h"

namespace Lu
{
	BigSlimeDeadState::BigSlimeDeadState()
	{
		SetName(L"BigSlimeDeadStateScript");
		SetStateType(eState::Dead);
	}

	BigSlimeDeadState::~BigSlimeDeadState()
	{

	}

	void BigSlimeDeadState::Update()
	{

	}

	void BigSlimeDeadState::Enter()
	{
		if (!(GetBigSlimeScript()->GetOwner()->DeleteComponent<Collider2D>()))
			assert(nullptr);
	}

	void BigSlimeDeadState::Exit()
	{
		Vector3 vCurPos = GetTransform()->GetPosition();
		int Xposition = -1;	// 데칼코마니로 스폰하기 위한 변수

		//슬라임 2마리 스폰
		for (int i = 1; i <= 2; ++i)
		{
			// 첫 번째 슬라임은 1, 2번째 슬라임은 -1
			Xposition *= -1;

			GameObject* pObject = object::Instantiate<GameObject>(Vector3(vCurPos.x + (50.f * Xposition), vCurPos.y - 100.f, 500.f), Vector3(180.f, 180.f, 100.f), eLayerType::Monster);
			pObject->SetName(L"Slime");

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"SlimeAnimation_Mtrl"));

			Collider2D* pCollider = pObject->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);
			pCollider->SetCenter(Vector2(0.f, -50.f));
			pCollider->SetSize(Vector2(0.5f, 0.4f));

			Animator* pAnimator = pObject->AddComponent<Animator>();
			SlimeScript* pSlimeScript = pObject->AddComponent<SlimeScript>();
			pSlimeScript->SetTarget(GetTarget());
		}

		object::Destroy(GetBigSlimeScript()->GetHPFrame());
		object::Destroy(GetBigSlimeScript()->GetHPBar());
		object::Destroy(GetBigSlimeScript()->GetOwner());
	}
}
