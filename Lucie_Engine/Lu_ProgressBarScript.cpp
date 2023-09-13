#include "Lu_ProgressBarScript.h"
#include "Lu_MeshRenderer.h"
#include "Lu_GameObject.h"
#include "Lu_FontWrapper.h"
#include "Lu_Scene.h"
#include "Lu_SceneManager.h"
#include "Lu_Camera.h"

namespace Lu
{
	ProgressBarScript::ProgressBarScript()
		: m_Transform(nullptr)
		, m_MeshRender(nullptr)
		, m_MaxValue(0.f)
		, m_CurValue(0.f)
	{
		SetName(L"ProgressBarScript");
	}

	ProgressBarScript::~ProgressBarScript()
	{
	}

	void ProgressBarScript::SetCurValue(int _Value)
	{
		if (!m_MeshRender)
			return;

		m_CurValue = (float)_Value;
		float ValuePercent = (float)(m_CurValue / m_MaxValue);

		m_MeshRender->GetMaterial()->SetScalarParam(Lu::graphics::SCALAR_PARAM::FLOAT_0, &ValuePercent);
	}

	void ProgressBarScript::SetCurValue(float _Value)
	{
		if (!m_MeshRender)
			return;

		m_CurValue = _Value;
		float ValuePercent = m_CurValue / m_MaxValue;

		m_MeshRender->GetMaterial()->SetScalarParam(Lu::graphics::SCALAR_PARAM::FLOAT_0, &ValuePercent);
	}


	//void ProgressBarScript::Render()
	//{
	//	if(!m_Transform)
	//		return;
	// 
	//	Vector3 vCenterPos = m_Transform->GetPosition();
	//	Vector2 vFontPos = SceneManager::GetActiveScene()->GetUICam()->WorldToScreen(vCenterPos);
	//	//Vector2 vResoultion = Vector2((float)application.GetWidth(), (float)application.GetHeight());
	//	//Vector2 vFontPos = Vector2(vCenterPos.x + vResoultion.x / 2.f, vCenterPos.y - vResoultion.y / 2.f);

	//	std::wstring bossName = m_BossName;
	//	wchar_t Font[256];
	//	wcscpy_s(Font, bossName.c_str());
	//	FontWrapper::DrawFont(Font, vFontPos.x, vFontPos.y, 20.f, FONT_RGBA(255, 255, 255, 255));
	//}
}