#include "Lu_FontScript.h"
#include "Lu_GameObject.h"

namespace Lu
{
	FontScript::FontScript()
		: m_Text{}
		, m_Pos(Vector2::Zero)
		, m_Size(0.f)
		, m_RGB(FONT_RGBA(255, 255, 255, 255))
	{
		SetName(L"FontScript");
	}

	FontScript::~FontScript()
	{
	}

	void FontScript::Render()
	{
		wchar_t Font[256];
		wcscpy_s(Font, m_Text.c_str());
		FontWrapper::DrawFont(Font, m_Pos.x, m_Pos.y, m_Size, m_RGB);
	}

	void FontScript::SetActive(bool _b)
	{
		GetOwner()->SetActive(_b);
	}
}
