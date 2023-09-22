#include "Lu_ItemScript.h"
#include "Lu_Time.h"
#include "Lu_GameObject.h"
#include "Lu_SceneManager.h"

namespace Lu
{
	ItemScript::ItemScript()
		: m_ItemType(eItemType::End)
		, m_ItemState(eItemState::None)
		, m_Transform(nullptr)
		, m_Time(0.f)
		, m_MoveDir(1.f)
		, m_MoveSpeed(30.f)
		, m_ItemSlotNumber(-1)
		, m_ItemOption(0)
	{
		SetName(L"ItemScript");
	}

	ItemScript::~ItemScript()
	{

	}

	void ItemScript::SetItemState(eItemState _State)
	{
		if (_State == m_ItemState)
			return;

		m_ItemState = _State;

		// 초기 설정이거나, Coin 아이템(아이콘으로 변경 불가)일 경우 예외처리
		if (m_ItemState == eItemState::None
			|| m_ItemType == eItemType::Coin)
		{
			return;
		}

		Scene* pCurScene = SceneManager::GetActiveScene();
		pCurScene->RemoveGameObject((eLayerType)GetOwner()->GetLayerIndex(), GetOwner());

		if (m_ItemState == eItemState::Drop)
		{
			pCurScene->AddGameObject(eLayerType::Item, GetOwner());
			GetOwner()->SetLayerIndex((int)eLayerType::Item);
			SceneManager::RemoveFromDontDestroyOnLoad(GetOwner());

			Collider2D* pCollider = GetOwner()->GetComponent<Collider2D>();
			if (!pCollider)
			{
				Collider2D* pCollider = GetOwner()->AddComponent<Collider2D>();
				pCollider->SetType(eColliderType::Rect);

				switch (m_ItemType)
				{
				case Lu::ItemScript::eItemType::Weapon:
					break;
				case Lu::ItemScript::eItemType::Jelly:
				{
					pCollider->SetSize(Vector2(0.7f, 0.5f));
				}
				break;
				case Lu::ItemScript::eItemType::End:
					break;
				default:
					break;
				}
			}
		}
		else
		{
			Collider2D* pCollider = GetOwner()->GetComponent<Collider2D>();
			if (pCollider)
			{
				if (!GetOwner()->DeleteComponent<Collider2D>())
					return;
			}

			pCurScene->AddGameObject(eLayerType::UI, GetOwner());
			GetOwner()->SetLayerIndex((int)eLayerType::UI);
			SceneManager::DontDestroyOnLoad(GetOwner());
		}
	}

	void ItemScript::Update()
	{
		switch (m_ItemState)
		{
		case Lu::ItemScript::eItemState::None:
			break;
		case Lu::ItemScript::eItemState::Drop:
		{
			if (!m_Transform)
				return;

			m_Time += (float)Time::DeltaTime();

			if (m_Time >= 0.5f)
			{
				m_MoveDir *= -1.f;
				m_Time = 0.f;
			}

			Vector3 vPos = m_Transform->GetPosition();
			vPos.y += m_MoveDir * m_MoveSpeed * (float)Time::DeltaTime();
			m_Transform->SetPosition(vPos);
		}
			break;
		case Lu::ItemScript::eItemState::Icon:
			break;
		default:
			break;
		}
	}
}