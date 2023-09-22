#include "Lu_FontWrapper.h"
#include "Lu_GraphicDevice_DX11.h"

namespace Lu
{
	IFW1Factory* FontWrapper::m_FW1Factory = nullptr;
	IFW1FontWrapper* FontWrapper::m_FontWrapper = nullptr;

	bool FontWrapper::Initialize()
	{
		if (FAILED(FW1CreateFactory(FW1_VERSION, &m_FW1Factory)))
			return false;

		ID3D11Device* pDevice = graphics::GetDevice()->GetID3D11Device();
		if (FAILED(m_FW1Factory->CreateFontWrapper(pDevice, L"배달의민족 주아", &m_FontWrapper)))
			return false;

		return true;
	}

	void FontWrapper::DrawFont(const wchar_t* str, float x, float y, float size, UINT rgb)
	{
		//RGB();
		ID3D11DeviceContext* pContext = graphics::GetDevice()->GetID3D11DeviceContext();
		m_FontWrapper->DrawString(
			pContext,
			str, // String
			size,// Font size
			x,// X position
			y,// Y position
			rgb,// Text color, 0xAaBbGgRr
			FW1_RESTORESTATE      // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
		);
	}

	void FontWrapper::Release()
	{
		m_FW1Factory->Release();
		m_FW1Factory = nullptr;

		m_FontWrapper->Release();
		m_FontWrapper = nullptr;
	}
}
