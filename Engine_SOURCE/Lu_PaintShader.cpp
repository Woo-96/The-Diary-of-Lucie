#include "Lu_PaintShader.h"
#include "Lu_Texture.h"

namespace Lu::graphics
{
	PaintShader::PaintShader()
		: m_Target(nullptr)
	{
	}

	PaintShader::~PaintShader()
	{
	}

	void PaintShader::Binds()
	{
		m_Target->BindUnorderedAccessViews(0);
		 
		m_GroupX = m_Target->GetWidth() / m_ThreadGroupCountX + 1;
		m_GroupY = m_Target->GetHeight() / m_ThreadGroupCountY + 1;
		m_GroupZ = 1;
	}

	void PaintShader::Clear()
	{
		m_Target->ClearUnorderedAccessViews(0);
	}
}