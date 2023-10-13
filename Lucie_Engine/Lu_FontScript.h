#pragma once
#include "Lu_UIScript.h"
#include "Lu_FontWrapper.h"

namespace Lu
{
	class FontScript : public UIScript
	{
	public:
		FontScript();
		virtual ~FontScript();

	private:
		std::wstring	m_Text;
		Vector2			m_Pos;
		float			m_Size;
		UINT			m_RGB;

	public:
		void SetText(std::wstring _Text)
		{
			m_Text = _Text;
		}

		void SetPos(Vector2 _Pos)
		{
			m_Pos = _Pos;
		}

		void SetSize(float _Size)
		{
			m_Size = _Size;
		}

		// #define FONT_RGBA(r, g, b, a) (((((BYTE)a << 24 ) | (BYTE)b << 16) | (BYTE)g << 8) | (BYTE)r)
		void SetRGB(UINT _RGB)
		{
			m_RGB = _RGB;
		}

	public:
		virtual void Render() override;

	public:
		virtual void SetActive(bool _b) override;
	};
}


