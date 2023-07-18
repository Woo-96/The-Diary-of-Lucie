#pragma once
#include "Lu_Component.h"

namespace Lu
{
	class Transform : public Component
	{
	public:
		Transform();
		virtual ~Transform();

	private:
		Vector3		m_Position;
		Vector3		m_Rotation;
		Vector3		m_Scale;

		Vector3		m_Up;
		Vector3		m_Foward;
		Vector3		m_Right;

		Matrix		m_World;

		Transform*	m_Parent;

	public:
		void SetPosition(Vector3 position) 
		{ 
			m_Position = position; 
		}

		void SetRotation(Vector3 rotation) 
		{ 
			m_Rotation = rotation; 
		}

		void SetScale(Vector3 scale) 
		{ 
			m_Scale = scale; 
		}

		void SetPosition(float x, float y, float z)
		{ 
			m_Position = Vector3(x, y, z); 
		}

		void SetRotation(float x, float y, float z) 
		{ 
			m_Rotation = Vector3(x, y, z); 
		}

		void SetScale(float x, float y, float z) 
		{ 
			m_Scale = Vector3(x, y, z); 
		}

		void SetParent(Transform* _Transform) 
		{
			m_Parent = _Transform;
		}

	public:
		Vector3 GetPosition()
		{ 
			return m_Position; 
		}

		Vector3 GetRotation()
		{
			return m_Rotation;
		}

		Vector3 GetScale() 
		{
			return m_Scale; 
		}

		Vector3 Foward() 
		{ 
			return m_Foward; 
		}

		Vector3 Right() 
		{ 
			return m_Right; 
		}

		Vector3 Up() 
		{ 
			return m_Up; 
		}

		Transform* GetParent() 
		{ 
			return m_Parent;
		}

		Matrix& GetWorldMatrix()
		{
			return m_World;
		}

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

	public:
		void BindConstantBuffer();
	};
}