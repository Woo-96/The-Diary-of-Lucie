#include "Lu_Application.h"
#include "Lu_Input.h"
#include "Lu_Time.h"
#include "Lu_Renderer.h"
#include "Lu_SceneManager.h"
#include "Lu_CollisionManager.h"

namespace Lu
{
	Application::Application()
		: graphicDevice(nullptr)
		, m_Hwnd(NULL)
		, m_Width(-1)
		, m_Height(-1)
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		Update();
		LateUpdate();
		Render();
		Destroy();
	}

	void Application::Initialize()
	{
		Time::Initiailize();
		Input::Initialize();

		renderer::Initialize();
		SceneManager::Initialize();
	}

	void Application::Update()
	{
		Time::Update();
		Input::Update();
		SceneManager::Update();
	}

	void Application::LateUpdate()
	{
		SceneManager::LateUpdate();
		CollisionManager::Update();
	}

	void Application::Render()
	{
		Time::Render();

		graphicDevice->ClearTarget();
		graphicDevice->UpdateViewPort();

		renderer::Render();
		//graphicDevice->Draw();
	}

	void Application::Destroy()
	{
		SceneManager::Destroy();
	}

	void Application::Present()
	{
		graphicDevice->Present();
	}

	void Application::SetWindow(HWND _hWnd, UINT _Width, UINT _Height)
	{
		if (graphicDevice == nullptr)
		{
			m_Hwnd = _hWnd;
			m_Width = _Width;
			m_Height = _Height;

			// make_unique : unique_ptr을 생성하는 함수 (like. new)
			graphicDevice = std::make_unique<Lu::graphics::GraphicDevice_Dx11>();
			Lu::graphics::GetDevice() = graphicDevice.get();
		}

		RECT rt = { 0, 0, (LONG)_Width , (LONG)_Height };
		AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
		SetWindowPos(m_Hwnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
		ShowWindow(m_Hwnd, true);
		UpdateWindow(m_Hwnd);
	}
}
