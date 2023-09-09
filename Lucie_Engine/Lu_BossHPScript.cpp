#include "Lu_BossHPScript.h"
#include "Lu_MeshRenderer.h"
#include "Lu_GameObject.h"
#include "Lu_FontWrapper.h"
#include "Lu_Scene.h"
#include "Lu_SceneManager.h"
#include "Lu_Camera.h"

namespace Lu
{
	BossHPScript::BossHPScript()
		: m_Transform(nullptr)
		, m_MeshRender(nullptr)
		, m_MaxHP(0.f)
		, m_CurHP(0.f)
	{
		SetName(L"BossHPScript");
	}

	BossHPScript::~BossHPScript()
	{
	}

	void BossHPScript::SetCurHP(int _HP)
	{
		m_CurHP = (float)_HP;
		float HPPercent = (float)(m_CurHP / m_MaxHP);

		m_MeshRender->GetMaterial()->SetScalarParam(Lu::graphics::SCALAR_PARAM::FLOAT_0, &HPPercent);
	}

	void BossHPScript::Render()
	{
		Vector3 vCenterPos = m_Transform->GetPosition();
		Vector2 vFontPos = SceneManager::GetActiveScene()->GetUICam()->WorldToScreen(vCenterPos);
		//Vector2 vResoultion = Vector2((float)application.GetWidth(), (float)application.GetHeight());
		//Vector2 vFontPos = Vector2(vCenterPos.x + vResoultion.x / 2.f, vCenterPos.y - vResoultion.y / 2.f);

		std::wstring bossName = m_BossName;
		wchar_t Font[256];
		wcscpy_s(Font, bossName.c_str());
		FontWrapper::DrawFont(Font, vFontPos.x, vFontPos.y, 20.f, FONT_RGBA(255, 255, 255, 255));
	}
}