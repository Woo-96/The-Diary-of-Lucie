#pragma once
#include "Lucie_Engine.h"

#include "..\\External\\FW1FontWrapper\\Include\\FW1FontWrapper.h"
#include "..\\External\\FW1FontWrapper\\Include\\FW1CompileSettings.h"
//#include "..\\External\\Include\\Fmod\\common_platform.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\External\\FW1FontWrapper\\Library\\Debug\\FW1FontWrapperL.lib")
#else
#pragma comment(lib, "..\\External\\FW1FontWrapper\\Library\\Release\\FW1FontWrapper.lib")
#endif

//#include <SpriteFont.h>
//#include <SpriteBatch.h>

namespace Lu
{
#define FONT_RGBA(r, g, b, a) (((((BYTE)a << 24 ) | (BYTE)b << 16) | (BYTE)g << 8) | (BYTE)r)

	class FontWrapper
	{
	private:
		static IFW1Factory*			m_FW1Factory;
		static IFW1FontWrapper*		m_FontWrapper;

	public:
		static bool Initialize();
		static void DrawFont(const wchar_t* str, float x, float y, float size, UINT rgb);
		static void Release();
	};
}
