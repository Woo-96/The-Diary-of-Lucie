#include "Lu_DamageFontScript.h"
#include "Lu_GameObject.h"
#include "Lu_FontWrapper.h"
#include "Lu_Camera.h"
#include "Lu_Time.h"
#include "Lu_Object.h"
#include "Lu_Renderer.h"

namespace Lu
{
	DamageFontScript::DamageFontScript()
		: m_Transform(nullptr)
		, m_MainCam(nullptr)
		, m_Damage(0)
		, m_Time(0.f)
	{
	}

	DamageFontScript::~DamageFontScript()
	{
	}

	void DamageFontScript::Initialize()
	{
		m_Transform = GetOwner()->GetComponent<Transform>();
		m_MainCam = renderer::mainCamera;
	}

	void DamageFontScript::Update()
	{
		if (!m_Transform)
			return;

		m_Time += (float)Time::DeltaTime();

		if (m_Time >= 0.3f)
		{
			object::Destroy(GetOwner());
		}
		else
		{
			Vector3 vPos = m_Transform->GetPosition();
			vPos.x += 200.f * (float)Time::DeltaTime();
			vPos.y += 200.f * (float)Time::DeltaTime();
			m_Transform->SetPosition(vPos);
		}
	}

	void DamageFontScript::Render()
	{
		if (!m_MainCam)
			return;

		Vector3 vCenterPos = m_Transform->GetPosition();
		Vector2 vFontPos = m_MainCam->WorldToScreen(vCenterPos);

		std::wstring wstrFont = std::to_wstring(m_Damage);
		wchar_t Font[256];
		wcscpy_s(Font, wstrFont.c_str());
		FontWrapper::DrawFont(Font, vFontPos.x, vFontPos.y, 50.f, FONT_RGBA(255, 255, 255, 255));
	}
}