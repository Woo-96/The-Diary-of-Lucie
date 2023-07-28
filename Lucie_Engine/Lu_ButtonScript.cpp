#include "Lu_ButtonScript.h"
#include "Lu_Renderer.h"

Lu::ButtonScript::ButtonScript()
	: m_ButtonState(eButtonState::Normal)
	, m_arrLT{}
	, m_SliceSize(Vector2::Zero)
	, m_TexResolution(Vector2::Zero)
{
	SetName(L"ButtonScript");
}

Lu::ButtonScript::~ButtonScript()
{
}

void Lu::ButtonScript::SetButtonLT(Vector2 _arrLT[], int _Count)
{
	if ((int)eButtonState::End < _Count)
		return;

	for (int i = 0; i < _Count; ++i)
	{
		m_arrLT[i] = _arrLT[i];
	}
}

void Lu::ButtonScript::Update()
{
	// Disable이거나 End면 return
	if ((int)m_ButtonState < (int)eButtonState::Disable)
	{

	}
}

void Lu::ButtonScript::Binds()
{
	graphics::ConstantBuffer* cb
		= renderer::constantBuffer[(int)eCBType::Button];

	renderer::ButtonCB data;
	data.vLtUV = m_arrLT[(int)m_ButtonState] / m_TexResolution;
	data.vSliceUV = m_SliceSize / m_TexResolution;

	cb->SetData(&data);
	cb->Bind(eShaderStage::VS);
	cb->Bind(eShaderStage::PS);
}

void Lu::ButtonScript::CollisionMouseHoveredCallback(const Vector2& Pos)
{
}

void Lu::ButtonScript::CollisionMouseReleaseCallback()
{
}
