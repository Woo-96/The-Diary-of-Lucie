#include "Lu_ImmovableScript.h"
#include "Lu_GameObject.h"
#include "Lu_PlayerScript.h"

namespace Lu
{
	ImmovableScript::ImmovableScript()
	{
		SetName(L"ImmovableScript");
	}

	ImmovableScript::~ImmovableScript()
	{
	}

	void ImmovableScript::OnCollisionEnter(Collider2D* _Other)
	{
		int iLayer = _Other->GetOwner()->GetLayerIndex();

		if ((int)eLayerType::Player == iLayer
			|| (int)eLayerType::Monster == _Other->GetOwner()->GetLayerIndex())
		{
			if ((int)eLayerType::Player == iLayer)
			{
				if (!(L"ImmovableCollider" == _Other->GetName()))
					return;

				PlayerScript* pPlayerScript = _Other->GetOwner()->GetComponent<PlayerScript>();
				pPlayerScript->SetAction(true);
				pPlayerScript->ChangeState(StateScript::eState::Idle);
			}

			Transform* pThisTransform = GetOwner()->GetComponent<Transform>();
			Transform* pOtherTransform = _Other->GetOwner()->GetComponent<Transform>();

			Vector3 vThisPos = pThisTransform->GetPosition();
			Vector3 vOtherPos = pOtherTransform->GetPosition();
			float Zpos = vOtherPos.z;

			Vector3 vCollisionDir = vOtherPos - vThisPos;
			vCollisionDir.Normalize();

			float pushDistance = 0.0001f; // 밀어낼 양
			Vector3 pushedPosition = vOtherPos + vCollisionDir * pushDistance;
			pushedPosition.z = Zpos;
			pOtherTransform->SetPosition(pushedPosition);
		}
	}

	void ImmovableScript::OnCollisionStay(Collider2D* _Other)
	{
		int PlayerLayer = _Other->GetOwner()->GetLayerIndex();

		if ((int)eLayerType::Player == PlayerLayer
			|| (int)eLayerType::Monster == _Other->GetOwner()->GetLayerIndex())
		{
			if ((int)eLayerType::Player == PlayerLayer)
			{
				if (!(L"ImmovableCollider" == _Other->GetName()))
					return;

				PlayerScript* pPlayerScript = _Other->GetOwner()->GetComponent<PlayerScript>();
				pPlayerScript->SetAction(true);
				pPlayerScript->ChangeState(StateScript::eState::Idle);
			}

			Transform* pThisTransform = GetOwner()->GetComponent<Transform>();
			Transform* pOtherTransform = _Other->GetOwner()->GetComponent<Transform>();

			Vector3 vThisPos = pThisTransform->GetPosition();
			Vector3 vOtherPos = pOtherTransform->GetPosition();
			float Zpos = vOtherPos.z;

			Vector3 vCollisionDir = vOtherPos - vThisPos;
			vCollisionDir.Normalize();

			float pushDistance = 0.0001f; // 밀어낼 양
			Vector3 pushedPosition = vOtherPos + vCollisionDir * pushDistance;
			pushedPosition.z = Zpos;
			pOtherTransform->SetPosition(pushedPosition);
		}
	}

	void ImmovableScript::OnCollisionExit(Collider2D* _Other)
	{
		int PlayerLayer = _Other->GetOwner()->GetLayerIndex();

		if ((int)eLayerType::Player == PlayerLayer)
		{
			if (!(L"ImmovableCollider" == _Other->GetName()))
				return;

			PlayerScript* pPlayerScript = _Other->GetOwner()->GetComponent<PlayerScript>();
			pPlayerScript->SetAction(false);
		}
	}
}