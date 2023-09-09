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
		static Matrix	View;
		static Matrix	Projection;

		Matrix			m_View;
		Matrix			m_Projection;

		eProjectionType m_Type;
		float			m_AspectRatio;
		float			m_Near;
		float			m_Far;
		float			m_Scale;

		std::bitset<(UINT)eLayerType::End>	m_LayerMask;
		std::vector<GameObject*>			m_OpaqueGameObjects;
		std::vector<GameObject*>			m_CutOutGameObjects;
		std::vector<GameObject*>			m_TransparentGameObjects;

	public:
		static void SetGpuViewMatrix(Matrix view)
		{
			View = view;
		}

		static void SetGpuProjectionMatrix(Matrix projection)
		{
			Projection = projection;
		}

		void SetScale(float _Scale)
		{
			m_Scale = _Scale;
		}

	public:
		static Matrix& GetGpuViewMatrix() 
		{ 
			return View; 
		}

		static Matrix& GetGpuProjectionMatrix() 
		{ 
			return Projection; 
		}

		float GetScale() 
		{ 
			return m_Scale; 
		}

		Matrix& GetViewMatrix() 
		{ 
			return m_View; 
		}

		Matrix& GetProjectionMatrix() 
		{ 
			return m_Projection; 
		}

	public:
		virtual void LateUpdate() override;
		virtual void Render() override;

	public:
		bool CreateViewMatrix();
		bool CreateProjectionMatrix(eProjectionType _Type);
		void RegisterCameraInRenderer();

		void TurnLayerMask(eLayerType _Type, bool _Enable = true);
		void EnableLayerMasks()
		{
			m_LayerMask.set();
		}

		void DisableLayerMasks()
		{
			m_LayerMask.reset();
		}

		void AlphaSortGameObjects();
		void ZSortTransparencyGameObjects();
		void DivideAlphaBlendGameObjects(const std::vector<GameObject*> _GameObjs);
		void RenderOpaque();
		void RenderCutOut();
		void RenderTransparent();

		void EnableDepthStencilState();
		void DisableDepthStencilState();

		Vector3 ScreenToWorld(Vector2 _MousePos);
		Vector2 WorldToScreen(Vector3 _WorldPos);
	};
}