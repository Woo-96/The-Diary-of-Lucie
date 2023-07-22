#include "Lu_Time.h"
#include "Lu_Application.h"

extern Lu::Application application;

namespace Lu
{
	double Time::m_DeltaTime = 0.0l;
	double Time::m_Second = 0.0f;
	LARGE_INTEGER Time::m_CpuFrequency = {};
	LARGE_INTEGER Time::m_PrevFrequency = {};
	LARGE_INTEGER Time::m_CurFrequency = {};

	void Time::Initiailize()
	{
		// CPU 고유 진동수 가져오기
		QueryPerformanceFrequency(&m_CpuFrequency);

		// 프로그램이 처음 시작할때 진동수
		QueryPerformanceCounter(&m_PrevFrequency);
	}

	void Time::Update()
	{
		QueryPerformanceCounter(&m_CurFrequency);

		double differnceFrequency = (double)(m_CurFrequency.QuadPart - m_PrevFrequency.QuadPart);

		m_DeltaTime = (differnceFrequency / m_CpuFrequency.QuadPart);
		
		m_PrevFrequency.QuadPart = m_CurFrequency.QuadPart;
	}

	void Time::Render()
	{
		m_Second += m_DeltaTime;

		if (m_Second > 1.0f)
		{
			HWND hWnd = application.GetHwnd();

			wchar_t szFloat[50] = {};
			float FPS = 1.0f / (float)m_DeltaTime;
			swprintf_s(szFloat, 50, L"FPS : %d", (UINT)FPS);
			//int iLen = wcsnlen_s(szFloat, 50);
			SetWindowText(hWnd, szFloat);

			//TextOut(hdc, 0, 0, szFloat, 20);
			m_Second = 0.0f;
		}
	}
}
