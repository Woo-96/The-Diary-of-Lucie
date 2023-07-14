#pragma once
#include "Lucie_Engine.h"
#include "Lu_GraphicDevice_Dx11.h"
#include "Lu_Scene.h"

namespace Lu
{
	class Application
	{
	public:
		Application();
		~Application();

	private:
		bool m_bInitialize = false;

		std::unique_ptr<Lu::graphics::GraphicDevice_Dx11> graphicDevice;	// unique_ptr : ������ �Ѱ��� ��ü�� ����� �ִ� ����Ʈ ������ (like. static)

		// HDC mHdc; -> GPU API
		HWND m_Hwnd;
		UINT m_Width;
		UINT m_Height;

	public:
		UINT GetWidth()	const
		{ 
			return m_Width; 
		}

		UINT GetHeight() const
		{ 
			return m_Height; 
		}

		HWND GetHwnd() const
		{ 
			return m_Hwnd; 
		}
		
	public:
		void Run();

		void Initialize();
		void Update();
		void LateUpdate();
		void Render();
		void Destroy();
		void Present();

	public:
		void SetWindow(HWND _hWnd, UINT _Width, UINT _Height);
	};
}
