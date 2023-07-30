#pragma once
#include "Lu_Component.h"
#include "Lu_Transform.h"

namespace Lu
{
	class Collider2D : public Component
	{
	public:
		Collider2D();
		virtual ~Collider2D();

	private:
		static UINT		m_ColliderNumber;
		UINT			m_ColliderID;

		eColliderType	m_Type;

		Vector3			m_Position;
		Vector2			m_Size;
		Vector2			m_Center;

		int             m_CollisionCount;

		bool			m_bRender;

	public:
		void SetType(eColliderType _Type) 
		{ 
			m_Type = _Type; 
		}

		void SetSize(Vector2 _Size) 
		{ 
			m_Size = _Size; 
		}

		void SetCenter(Vector2 _Size) 
		{ 
			m_Center = _Size; 
		}

	public:
		UINT GetColliderID() 
		{ 
			return m_ColliderID; 
		}

		eColliderType GetType()
		{
			return m_Type;
		}

		Vector3 GetPosition()
		{
			return m_Position;
		}

		Vector2 GetSize()
		{
			return m_Size;
		}

		Vector2 GetCenter()
		{
			return m_Center;
		}

	public:
		virtual void Update() override;
		virtual void LateUpdate() override;

	public:
		void OnCollisionEnter(Collider2D* _Other);
		void OnCollisionStay(Collider2D* _Other);
		void OnCollisionExit(Collider2D* _Other);
	};
}
