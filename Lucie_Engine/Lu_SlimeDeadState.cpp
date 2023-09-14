#include "Lu_SlimeDeadState.h"
#include "Lu_Object.h"
#include "Lu_SlimeScript.h"
#include "Lu_Resources.h"
#include "Lu_PlayerScript.h"
#include "Lu_MeshRenderer.h"
#include "Lu_JellyScript.h"

namespace Lu
{
	SlimeDeadState::SlimeDeadState()
	{
		SetName(L"SlimeDeadStateScript");
		SetStateType(eState::Dead);
	}

	SlimeDeadState::~SlimeDeadState()
	{

	}

	void SlimeDeadState::Enter()
	{
		if (!(GetSlimeScript()->GetOwner()->DeleteComponent<Collider2D>()))
			assert(nullptr);
	}

	void SlimeDeadState::Exit()
	{
		// °æÇèÄ¡ »ý¼º
		GetTarget()->IncreaseEXP(30);

		// Á©¸® ¾ÆÀÌÅÛ µå·Ó
		CreateGreenJelly();

		// °´Ã¼ ¼Ò¸ê
		object::Destroy(GetSlimeScript()->GetOwner());
	}

	void SlimeDeadState::CreateGreenJelly()
	{
		Vector3 vSpawnPos = GetTransform()->GetPosition();
		vSpawnPos.y -= 60.f;

		GameObject* pJelly = object::Instantiate<GameObject>(vSpawnPos, Vector3(48.f, 48.f, 100.f), eLayerType::Item);
		pJelly->SetName(L"GreenJelly");

		MeshRenderer* pMeshRender = pJelly->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		pMeshRender->SetMaterial(Resources::Find<Material>(L"GreenJelly_Mtrl"));

		Collider2D* pCollider = pJelly->AddComponent<Collider2D>();
		pCollider->SetType(eColliderType::Rect);
		pCollider->SetSize(Vector2(0.7f, 0.5f));

		JellyScript* pJellyScript = pJelly->AddComponent<JellyScript>();
		pJellyScript->SetTransform(pJelly->GetComponent<Transform>());
		pJellyScript->SetItemState(ItemScript::eItemState::Drop);
	}
}