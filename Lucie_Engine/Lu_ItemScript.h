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
		std::wstring	m_ItemName;
		eItemType		m_ItemType;
		eItemState		m_ItemState;
		Transform*		m_Transform;
		float			m_Time;
		float			m_MoveDir;
		float			m_MoveSpeed;
		int				m_ItemSlotNumber;
		int				m_ItemOption;

	public:
		void SetItemName(std::wstring _Name)
		{
			m_ItemName = _Name;
		}

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

		void SetItemSlotNumber(int _Num)
		{
			m_ItemSlotNumber = _Num;
		}

		void SetItemOption(int _Option)
		{
			m_ItemOption = _Option;
		}

	public:
		std::wstring GetItemName()
		{
			return m_ItemName;
		}

		eItemType GetItemType()	const
		{
			return m_ItemType;
		}

		eItemState GetItemState()	const
		{
			return m_ItemState;
		}

		int GetItemSlotNumber()	const
		{
			return m_ItemSlotNumber;
		}

		int GetItemOption()	const
		{
			return m_ItemOption;
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