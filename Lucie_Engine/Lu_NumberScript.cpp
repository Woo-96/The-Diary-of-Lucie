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
		SetName(L"NumberScript");
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
		Vector2 vLT = m_arrLT[m_CurNumber] / m_TexResolution;
		Vector2 vSlice = m_SliceSize / m_TexResolution;
		GetOwner()->GetComponent<MeshRenderer>()->GetMaterial()->SetScalarParam(Lu::graphics::SCALAR_PARAM::VEC2_0, &vLT);
		GetOwner()->GetComponent<MeshRenderer>()->GetMaterial()->SetScalarParam(Lu::graphics::SCALAR_PARAM::VEC2_1, &vSlice);
	}

	void NumberScript::SetActive(bool _b)
	{
		GetOwner()->SetActive(_b);
	}
}