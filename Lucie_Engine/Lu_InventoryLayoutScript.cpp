#include "Lu_InventoryLayoutScript.h"
#include "Lu_FontWrapper.h"
#include "Lu_InventoryScript.h"
#include "Lu_SceneManager.h"
#include "Lu_PlayerScript.h"

namespace Lu
{
	InventoryLayoutScript::InventoryLayoutScript()
		: m_Inventory(nullptr)
		, m_Text(L"�κ��丮")
		, m_bTextPrint(false)
		, m_TextPos(Vector2(1195.f, 290.f))
	{
	}

	InventoryLayoutScript::~InventoryLayoutScript()
	{
	}

	void InventoryLayoutScript::Render()
	{
		TextPrint();
		PlayerInfoPrint();

		// �κ��丮 ���� ���� ���
		if (m_Inventory)
		{
			std::wstring wstrText = L"/";
			wchar_t Font[256];
			wcscpy_s(Font, wstrText.c_str());
			FontWrapper::DrawFont(Font, 1135.f, 610.f, 25.f, FONT_RGBA(51, 25, 0, 255));

			int iCurCount = m_Inventory->GetCurCount();
			wstrText = std::to_wstring(iCurCount);
			wcscpy_s(Font, wstrText.c_str());
			FontWrapper::DrawFont(Font, 1120.f, 610.f, 25.f, FONT_RGBA(51, 25, 0, 255));

			int iMaxCount = m_Inventory->GetMaxCount();
			wstrText = std::to_wstring(iMaxCount);
			wcscpy_s(Font, wstrText.c_str());
			FontWrapper::DrawFont(Font, 1150.f, 610.f, 25.f, FONT_RGBA(51, 25, 0, 255));
		}
	}

	void InventoryLayoutScript::TextPrint()
	{
		if (m_bTextPrint)
		{
			wchar_t Font[256];
			wcscpy_s(Font, m_Text.c_str());

			float fTextPosX = m_TextPos.x;

			switch ((int)m_Text.length())
			{
			case 2:
				fTextPosX = 1220.f;
				break;
			case 5:
				fTextPosX = 1185.f;
				break;
			default:
				break;
			}

			FontWrapper::DrawFont(Font, fTextPosX, m_TextPos.y, 30.f, FONT_RGBA(255, 165, 0, 255));
		}
		else
		{
			m_Text = L"�κ��丮";
			wchar_t Font[256];
			wcscpy_s(Font, m_Text.c_str());
			FontWrapper::DrawFont(Font, m_TextPos.x, m_TextPos.y, 30.f, FONT_RGBA(255, 165, 0, 255));
		}

		// ���ڸ� ��������� �ٽ� false�� �ٲ���
		m_bTextPrint = false;
	}

	void InventoryLayoutScript::PlayerInfoPrint()
	{
		GameObject* pPlayer = SceneManager::FindPlayer();
		PlayerScript* pPlayerScript = pPlayer->GetComponent<PlayerScript>();


		float fLeftTextX = 1105.f;
		float fRightTextX = 1242.f;
		float fY = 480.f;

		// �ִ� ü��
		std::wstring wstrText = L"�ִ�ü��";
		wchar_t Font[256];
		wcscpy_s(Font, wstrText.c_str());
		FontWrapper::DrawFont(Font, fLeftTextX, fY, 25.f, FONT_RGBA(255, 255, 255, 255));

		int iNum = pPlayerScript->GetPlayerInfo().MaxHP;
		wstrText = std::to_wstring(iNum);
		wcscpy_s(Font, wstrText.c_str());
		FontWrapper::DrawFont(Font, fLeftTextX + 110.f, fY, 25.f, FONT_RGBA(255, 255, 255, 255));


		// �ִ� ����
		wstrText = L"�ִ븶��";
		wcscpy_s(Font, wstrText.c_str());
		FontWrapper::DrawFont(Font, fRightTextX, fY, 25.f, FONT_RGBA(255, 255, 255, 255));

		iNum = pPlayerScript->GetPlayerInfo().MaxMP;
		wstrText = std::to_wstring(iNum);
		wcscpy_s(Font, wstrText.c_str());
		FontWrapper::DrawFont(Font, fRightTextX + 123.f, fY, 25.f, FONT_RGBA(255, 255, 255, 255));



		fY += 25.f;

		// �ִ� ���׹̳�?
		wstrText = L"���׹̳�";
		wcscpy_s(Font, wstrText.c_str());
		FontWrapper::DrawFont(Font, fLeftTextX, fY, 25.f, FONT_RGBA(255, 255, 255, 255));

		iNum = (int)pPlayerScript->GetPlayerInfo().MaxTP;
		wstrText = std::to_wstring(iNum);
		wcscpy_s(Font, wstrText.c_str());
		FontWrapper::DrawFont(Font, fLeftTextX + 100.f, fY, 25.f, FONT_RGBA(255, 255, 255, 255));

		
		// �뷱�� (���� �ɼ�..)
		wstrText = L"�뷱��";
		wcscpy_s(Font, wstrText.c_str());
		FontWrapper::DrawFont(Font, fRightTextX, fY, 25.f, FONT_RGBA(255, 255, 255, 255));

		wstrText = std::to_wstring(100);
		wstrText += L"%";
		wcscpy_s(Font, wstrText.c_str());
		FontWrapper::DrawFont(Font, fRightTextX + 78.f, fY, 25.f, FONT_RGBA(255, 255, 255, 255));


		fY += 25.f;


		// ���ݷ�
		wstrText = L"���ݷ�";
		wcscpy_s(Font, wstrText.c_str());
		FontWrapper::DrawFont(Font, fLeftTextX, fY, 25.f, FONT_RGBA(255, 255, 255, 255));

		iNum = pPlayerScript->GetPlayerInfo().Attack;
		wstrText = std::to_wstring(iNum);
		wcscpy_s(Font, wstrText.c_str());
		FontWrapper::DrawFont(Font, fLeftTextX + 100.f, fY, 25.f, FONT_RGBA(255, 255, 255, 255));


		// ���� (�ֱ��ѵ�..)
		wstrText = L"���ݼӵ�";
		wcscpy_s(Font, wstrText.c_str());
		FontWrapper::DrawFont(Font, fRightTextX, fY, 25.f, FONT_RGBA(255, 255, 255, 255));

		wstrText = std::to_wstring(25);
		wstrText += L"%";
		wcscpy_s(Font, wstrText.c_str());
		FontWrapper::DrawFont(Font, fRightTextX + 90.f, fY, 25.f, FONT_RGBA(255, 255, 255, 255));



		fY += 25.f;


		// ������
		wstrText = L"������";
		wcscpy_s(Font, wstrText.c_str());
		FontWrapper::DrawFont(Font, fLeftTextX, fY, 25.f, FONT_RGBA(255, 255, 255, 255));

		iNum = pPlayerScript->GetPlayerInfo().Magic;
		wstrText = std::to_wstring(iNum);
		wcscpy_s(Font, wstrText.c_str());
		FontWrapper::DrawFont(Font, fLeftTextX + 100.f, fY, 25.f, FONT_RGBA(255, 255, 255, 255));


		// ���� ���� (���� �ɼ�..)
		wstrText = L"���밨��";
		wcscpy_s(Font, wstrText.c_str());
		FontWrapper::DrawFont(Font, fRightTextX, fY, 25.f, FONT_RGBA(255, 255, 255, 255));

		wstrText = std::to_wstring(0);
		wstrText += L"%";
		wcscpy_s(Font, wstrText.c_str());
		FontWrapper::DrawFont(Font, fRightTextX + 105.f, fY, 25.f, FONT_RGBA(255, 255, 255, 255));



		fY += 25.f;


		// �̵� �ӵ� (�ֱ��ѵ� �ø� �� ����..)
		wstrText = L"�̵��ӵ�";
		wcscpy_s(Font, wstrText.c_str());
		FontWrapper::DrawFont(Font, fLeftTextX, fY, 25.f, FONT_RGBA(255, 255, 255, 255));

		wstrText = std::to_wstring(1);
		wstrText += L".";
		wstrText += std::to_wstring(0);
		wcscpy_s(Font, wstrText.c_str());
		FontWrapper::DrawFont(Font, fLeftTextX + 95.f, fY, 25.f, FONT_RGBA(255, 255, 255, 255));



		// ġ��Ÿ (���� �ɼ�..)
		wstrText = L"ġ��Ÿ";
		wcscpy_s(Font, wstrText.c_str());
		FontWrapper::DrawFont(Font, fRightTextX, fY, 25.f, FONT_RGBA(255, 255, 255, 255));

		wstrText = std::to_wstring(0);
		wstrText += L"%";
		wcscpy_s(Font, wstrText.c_str());
		FontWrapper::DrawFont(Font, fRightTextX + 105.f, fY, 25.f, FONT_RGBA(255, 255, 255, 255));



		// ���� ���
		iNum = pPlayerScript->GetPlayerInfo().CurGold;
		wstrText = std::to_wstring(iNum);
		wcscpy_s(Font, wstrText.c_str());
		FontWrapper::DrawFont(Font, 1340.f, 610.f, 25.f, FONT_RGBA(255, 191, 0, 255));
	}
}