#pragma once
#include "..\\Engine_SOURCE\\Lu_Script.h"

namespace Lu
{
	class Transform;
	class ItemScript : public Script
	{
	public:
		enum class eItemType
		{
			Jelly,
			Coin,
			Weapon,
			End,
		};

		enum class eItemState
		{
			None,
			Drop,
			Icon,
		};

	public:
		ItemScript();
		virtual ~ItemScript();

	private:
		eItemType	m_ItemType;
		eItemState	m_ItemState;
		Transform*	m_Transform;
		float		m_Time;
		float		m_MoveDir;
		float		m_MoveSpeed;

	public:
		void SetItemType(eItemType _Type)
		{
			m_ItemType = _Type;
		}

		void SetTransform(Transform* _Transform)
		{
			m_Transform = _Transform;
		}

		void SetMoveSpeed(float _MoveSpeed)
		{
			m_MoveSpeed = _MoveSpeed;
		}

		void SetItemState(eItemState _State);

	public:
		eItemType GetItemType()	const
		{
			return m_ItemType;
		}

		eItemState GetItemState()	const
		{
			return m_ItemState;
		}

	public:
		virtual void Initialize() override {}
		virtual void Update() override;
		virtual void LateUpdate() override {}
		virtual void Render() override {}

	public:
		virtual void OnCollisionEnter(Collider2D* _Other) override {}
		virtual void OnCollisionStay(Collider2D* _Other) override {}
		virtual void OnCollisionExit(Collider2D* _Other) override {}

	public:
		virtual bool ItemEfficacy() = 0;
	};
}