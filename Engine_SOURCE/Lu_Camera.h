#pragma once
#include "Lu_Component.h"
#include "Lu_Graphics.h"

namespace Lu
{
	class Camera : public Component
	{
	public:
		enum class eProjectionType
		{
			Perpective,
			OrthoGraphic,
			None,
		};

	public:
		Camera();
		virtual ~Camera();

	private:
		static Matrix		m_View;
		static Matrix		m_Projection;

		eProjectionType		m_Type;
		float				m_AspectRatio;
		float				m_Near;
		float				m_Far;
		float				m_Size;	// 시야 (클 수록 더 넓은 시야)

	public:
		static Matrix GetViewMatrix() 
		{ 
			return m_View; 
		}

		static Matrix GetProjectionMatrix() 
		{ 
			return m_Projection; 
		}

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

	public:
		bool CreateViewMatrix();
		bool CreateProjectionMatrix(eProjectionType _Type);
	};
}