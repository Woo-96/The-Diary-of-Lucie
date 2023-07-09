#pragma once
#include "Lucie_Engine.h"

namespace Lu
{
	class Time
	{
	private:
		static double			m_DeltaTime;
		static double			m_Second;
		static LARGE_INTEGER	m_CpuFrequency;
		static LARGE_INTEGER	m_PrevFrequency;
		static LARGE_INTEGER	m_CurFrequency;

	public:
		static void Initiailize();
		static void Update();
		static void Render();

	public:
		__forceinline static double DeltaTime() 
		{ 
			return m_DeltaTime; 
		}
	};
}
