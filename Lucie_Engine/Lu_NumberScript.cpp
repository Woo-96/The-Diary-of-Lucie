#include "Lu_NumberScript.h"
#include "Lu_Renderer.h"
#include "Lu_Resources.h"
#include "Lu_GameObject.h"
#include "Lu_MeshRenderer.h"

namespace Lu
{
	NumberScript::NumberScript()
		: m_arrLT{}
		, m_SliceSize(Vector2::Zero)
		, m_CurNumber(-1)
		, m_TexResolution(Vector2::Zero)
	{

	}

	NumberScript::~NumberScript()
	{

	}

	void NumberScript::SetNumbersLT(Vector2 _arrLT[], int _Count)
	{
		if ((int)eNumber::End < _Count)
			return;

		for (int i = 0; i < _Count; ++i)
		{
			m_arrLT[i] = _arrLT[i];
		}
	}

	void NumberScript::SetCurNumber(int _CurNumber)
	{
		m_CurNumber = _CurNumber;
		Binds();
	}

	void NumberScript::Update()
	{

	}

	void NumberScript::Binds()
	{
		graphics::ConstantBuffer* cb
			= renderer::constantBuffer[(int)eCBType::Number];

		renderer::NumberCB data;
		data.vLtUV = m_arrLT[m_CurNumber] / m_TexResolution;
		data.vSliceUV = m_SliceSize / m_TexResolution;

		cb->SetData(&data);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::PS);
	}
}