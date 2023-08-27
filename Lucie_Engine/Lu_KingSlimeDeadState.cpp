#include "Lu_KingSlimeDeadState.h"
#include "Lu_KingSlimeScript.h"
#include "Lu_GameObject.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_Collider2D.h"
#include "Lu_Animator.h"
#include "Lu_BigSlimeScript.h"
#include "Lu_AudioSource.h"

namespace Lu
{
	KingSlimeDeadState::KingSlimeDeadState()
	{
		SetName(L"KingSlimeDeadStateScript");
		SetStateType(eState::Dead);

		m_SFX = new GameObject;
		m_SFX->AddComponent<AudioSource>();
	}

	KingSlimeDeadState::~KingSlimeDeadState()
	{
		if (nullptr != m_SFX)
		{
			delete m_SFX;
			m_SFX = nullptr;
		}
	}

	void KingSlimeDeadState::Update()
	{

	}

	void KingSlimeDeadState::Enter()
	{
		if (!(GetKingSlimeScript()->GetOwner()->DeleteComponent<Collider2D>()))
			assert(nullptr);

		AudioSource* pAudio = m_SFX->GetComponent<AudioSource>();
		pAudio->SetClip(Resources::Load<AudioClip>(L"SlimeDeadSFX", L"..\\Resources\\Sound\\SFX\\Monster\\Slime\\SlimeDeadSFX.ogg"));
		pAudio->Play();
	}

	void KingSlimeDeadState::Exit()
	{
		Vector3 vCurPos = GetTransform()->GetPosition();
		int Xposition = -1;	// 데칼코마니로 스폰하기 위한 변수

		// 빅 슬라임 2마리 스폰
		for (int i = 1; i <= 2; ++i)
		{
			// 첫 번째 슬라임은 1, 2번째 슬라임은 -1
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

			// 오른쪽 슬라임 HP
			Vector3 vHPPos = Vector3(0.f, 350.f, 100.f);

			// 왼쪽 슬라임 HP
			if (2 == i)
			{
				vHPPos = Vector3(0.f, 300.f, 100.f);
			}

			{
				GameObject* pObject = object::Instantiate<GameObject>(vHPPos, Vector3(759.f, 48.f, 100.f), eLayerType::UI);
				pObject->SetName(L"BigSlime_HP_Frame");

				MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
				pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
				pMeshRender->SetMaterial(Resources::Find<Material>(L"MidBossHPFrame_Mtrl"));

				pBigSlimeScript->SetHPFrame(pObject);
			}

			{
				GameObject* pObject = object::Instantiate<GameObject>(vHPPos, Vector3(720.f, 30.f, 100.f), eLayerType::UI);
				pObject->SetName(L"BigSlime_HP");

				MeshRenderer* pMeshRender = pObject->AddComponent<MeshRenderer>();
				pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
				pMeshRender->SetMaterial(Resources::Find<Material>(L"BossHP_Mtrl"));

				pBigSlimeScript->SetHPBar(pObject);
			}
		}

		object::Destroy(GetKingSlimeScript()->GetHPFrame());
		object::Destroy(GetKingSlimeScript()->GetHPBar());
		object::Destroy(GetKingSlimeScript()->GetOwner());
	}
}
