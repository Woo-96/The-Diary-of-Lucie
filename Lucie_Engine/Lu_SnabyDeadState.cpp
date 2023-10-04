#include "Lu_SnabyDeadState.h"
#include "Lu_Object.h"
#include "Lu_SnabyScript.h"
#include "Lu_Resources.h"
#include "Lu_AudioSource.h"
#include "Lu_SoundManager.h"
#include "Lu_PlayerScript.h"
#include "Lu_MeshRenderer.h"
#include "Lu_CoinScript.h"

namespace Lu
{
	SnabyDeadState::SnabyDeadState()
	{
		SetName(L"SnabyDeadStateScript");
		SetStateType(eState::Dead);
	}

	SnabyDeadState::~SnabyDeadState()
	{

	}

	void SnabyDeadState::Enter()
	{
		if (!(GetSnabyScript()->GetOwner()->DeleteComponent<Collider2D>()))
			assert(nullptr);
	}

	void SnabyDeadState::Exit()
	{
		// 경험치 생성
		GetTarget()->GetEXP(50);

		// 코인 드롭
		DropItem();

		// 객체 소멸
		object::Destroy(GetSnabyScript()->GetOwner());
	}

	void SnabyDeadState::DropItem()
	{
		Vector3 vSpawnPos = GetTransform()->GetPosition();
		vSpawnPos.y -= 30.f;

		GameObject* pCoin = object::Instantiate<GameObject>(vSpawnPos, Vector3(24.f, 24.f, 100.f), eLayerType::Item);
		pCoin->SetName(L"Coin");

		MeshRenderer* pMeshRender = pCoin->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		pMeshRender->SetMaterial(Resources::Find<Material>(L"CoinAnimation_Mtrl"));

		pCoin->AddComponent<Collider2D>();

		Animator* pAnimator = pCoin->AddComponent<Animator>();

		CoinScript* pCoinScript = pCoin->AddComponent<CoinScript>();
		pCoinScript->SetTransform(pCoin->GetComponent<Transform>());
		pCoinScript->SetAnimator(pAnimator);

		// 코인 타입 결정
        int iCoinType = std::rand() % 3; // Generates a random number between 0 and 2
        switch (iCoinType)
        {
        case 0:
			pCoinScript->SetCoinType(CoinScript::eCoinType::Copper);
			break;
        case 1:
			pCoinScript->SetCoinType(CoinScript::eCoinType::Silver);
            break;
        case 2:
			pCoinScript->SetCoinType(CoinScript::eCoinType::Gold);
            break;
        }

		pCoinScript->CreateCoinAnimation();
	}
}