#include "Lu_EquipLabelScript.h"
#include "Lu_FontWrapper.h"
#include "Lu_GameObject.h"
#include "Lu_SceneManager.h"
#include "Lu_Camera.h"

namespace Lu
{
	EquipLabelScript::EquipLabelScript()
	{
		SetName(L"EquipLabelScript");
	}

	EquipLabelScript::~EquipLabelScript()
	{
	}

	void EquipLabelScript::Render()
	{ 
		Vector3 vCenterPos = GetOwner()->GetComponent<Transform>()->GetPosition();
		Vector2 vFontPos = SceneManager::GetActiveScene()->GetUICam()->WorldToScreen(vCenterPos);
		vFontPos.x -= 10.f;

		std::wstring wstrFont = L"E";
		wchar_t Font[256];
		wcscpy_s(Font, wstrFont.c_str());
		FontWrapper::DrawFont(Font, vFontPos.x, vFontPos.y, 10.f, FONT_RGBA(255, 255, 255, 255));
	}
}
