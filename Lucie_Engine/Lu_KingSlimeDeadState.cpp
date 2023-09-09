#include "Lu_KingSlimeDeadState.h"
#include "Lu_KingSlimeScript.h"
#include "Lu_GameObject.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_Collider2D.h"
#include "Lu_Animator.h"
#include "Lu_BigSlimeScript.h"
#include "Lu_BossHPScript.h"

namespace Lu
{
	KingSlimeDeadState::KingSlimeDeadState()
	{
		SetName(L"KingSlimeDeadStateScript");
		SetStateType(eState::Dead);
	}

	KingSlimeDeadState::~KingSlimeDeadState()
	{

	}

	void KingSlimeDeadState::Update()
	{

	}

	void KingSlimeDeadState::Enter()
	{
		if (!(GetKingSlimeScript()->GetOwner()->DeleteComponent<Collider2D>()))
			assert(nullptr);
	}

	void KingSlimeDeadState::Exit()
	{
		Vector3 vCurPos = GetTransform()->GetPosition();
		int Xposition = -1;	// ��Į�ڸ��Ϸ� �����ϱ� ���� ����

		// �� ������ 2���� ����
		for (int i = 1; i <= 2; ++i)
		{
			// ù ��° �������� 1, 2��° �������� -1
			Xposition *= -1;

			GameObject* pObject = object::Instantiate<GameObject>(Vector3(vCurPos.x + (100.f * Xposition), vCurPos.y - 200.f, 500.f), Vector3(360.f, 360.f, 100.f), eLayerType::Monster);
			pObject->SetName(L"BigSlime");

			MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			pMeshRender->SetMaterial(Resources::Find<Material>(L"SlimeAnimation_Mtrl"));

			Collider2D* pCollider = pObject->AddComponent<Collider2D>();
			pCollider->SetType(eColliderType::Rect);
			pCollider->SetCenter(Vector2(0.f, -100.f));
			pCollider->SetSize(Vector2(0.5f, 0.4f));

			Animator* pAnimator = pObject->AddComponent<Animator>();
			BigSlimeScript* pBigSlimeScript = pObject->AddComponent<BigSlimeScript>();
			pBigSlimeScript->SetTarget(GetTarget());

			// ������ ������ HP
			Vector3 vHPPos;
			std::wstring pHPBar;

			// ���� ������ HP
			if (1 == i)
			{
				vHPPos = Vector3(0.f, 350.f, 100.f);
				pHPBar = L"BigSlime1_BossHP_Mtrl";
			}
			else
			{
				vHPPos = Vector3(0.f, 300.f, 100.f);
				pHPBar = L"BigSlime2_BossHP_Mtrl";
			}

			{
				GameObject* pObject = object::Instantiate<GameObject>(vHPPos, Vector3(759.f, 48.f, 100.f), eLayerType::UI);
				pObject->SetName(L"BigSlime_HPFrame");

				MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
				pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
				pMeshRender->SetMaterial(Resources::Find<Material>(L"MidBossHPFrame_Mtrl"));

				pBigSlimeScript->SetHPFrame(pObject);
			}

			{
				GameObject* pObject = object::Instantiate<GameObject>(vHPPos, Vector3(720.f, 30.f, 100.f), eLayerType::UI);
				pObject->SetName(L"BigSlime_HPBar");

				MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
				pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
				pMeshRender->SetMaterial(Resources::Find<Material>(pHPBar));

				pBigSlimeScript->SetHPBar(pObject);

				BossHPScript* pHPScript = pObject->AddComponent<BossHPScript>();
				pHPScript->SetTransform(pObject->GetComponent<Transform>());
				pHPScript->SetMeshRender(pMeshRender);
				pHPScript->SetMaxHP(pBigSlimeScript->GetInfo().MaxHP);
				pHPScript->SetCurHP(pBigSlimeScript->GetInfo().HP);
				pHPScript->SetBossName(L"�򽽶���");
			}
		}

		object::Destroy(GetKingSlimeScript()->GetHPFrame());
		object::Destroy(GetKingSlimeScript()->GetHPBar());
		object::Destroy(GetKingSlimeScript()->GetOwner());
	}
}
