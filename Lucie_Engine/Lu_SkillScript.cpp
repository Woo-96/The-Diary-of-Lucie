#include "Lu_SkillScript.h"
#include "Lu_FontWrapper.h"
#include "Lu_Object.h"
#include "Lu_GameObject.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_SkillSelectScript.h"
#include "Lu_PlayerScript.h"
#include "Lu_Time.h"

namespace Lu
{
	SkillScript::SkillScript()
		: m_PlayerScript(nullptr)
		, m_arrParts{}
		, m_arrSkill{}
		, m_RecentSkill(nullptr)
		, m_CurState(eUIState::None)
		, m_Time(0.f)
	{
		SetName(L"SkillScript");
	}

	SkillScript::~SkillScript()
	{
		for (int i = 0; i < (int)eSkillType::None; ++i) 
		{
			if(m_arrSkill[i])
			{
				delete m_arrSkill[i];
				m_arrSkill[i] = nullptr;
			}
		}
	}

	void SkillScript::Initialize()
	{
		// 스킬 생성
		CreateSkill();

		// 스킬 UI 생성
		MeshRenderer* pMeshRender;

		m_arrParts[(int)eParts::LearnSkill] = object::Instantiate<GameObject>(Vector3(0.f, 265.f, 150.f), Vector3(600.f, 169.5f, 100.f), eLayerType::UI);
		m_arrParts[(int)eParts::LearnSkill]->SetName(L"UI_LearnSkill");
		m_arrParts[(int)eParts::LearnSkill]->SetActive(false);
		SceneManager::DontDestroyOnLoad(m_arrParts[(int)eParts::LearnSkill]);
		pMeshRender = m_arrParts[(int)eParts::LearnSkill]->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		pMeshRender->SetMaterial(Resources::Find<Material>(L"LearnSkillFrame_Mtrl"));

		m_arrParts[(int)eParts::LearnSkillIcon] = object::Instantiate<GameObject>(Vector3(-120.f, 310.f, 150.f), Vector3(40.f, 40.f, 100.f), eLayerType::UI);
		m_arrParts[(int)eParts::LearnSkillIcon]->SetName(L"UI_LearnSkillIcon");
		m_arrParts[(int)eParts::LearnSkillIcon]->SetActive(false);
		SceneManager::DontDestroyOnLoad(m_arrParts[(int)eParts::LearnSkillIcon]);
		pMeshRender = m_arrParts[(int)eParts::LearnSkillIcon]->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));


		m_arrParts[(int)eParts::NewSkill_1] = object::Instantiate<GameObject>(Vector3(-140.f, -50.f, 100.f), Vector3(226.5f, 256.5f, 100.f), eLayerType::UI);
		m_arrParts[(int)eParts::NewSkill_1]->SetName(L"UI_LearnSkill1");
		m_arrParts[(int)eParts::NewSkill_1]->SetActive(false);
		SceneManager::DontDestroyOnLoad(m_arrParts[(int)eParts::NewSkill_1]);
		pMeshRender = m_arrParts[(int)eParts::NewSkill_1]->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		pMeshRender->SetMaterial(Resources::Find<Material>(L"SkillBoxFrame_Mtrl"));
		SkillSelectScript* pSkillBtn = m_arrParts[(int)eParts::NewSkill_1]->AddComponent<SkillSelectScript>();
		pSkillBtn->SetPlayerScript(m_PlayerScript);
		pSkillBtn->SkillUI(this);
		pSkillBtn->SetSkill(m_arrSkill[(int)eSkillType::IceBall]);

		m_arrParts[(int)eParts::NewSkill_2] = object::Instantiate<GameObject>(Vector3(120.f, -50.f, 100.f), Vector3(226.5f, 256.5f, 100.f), eLayerType::UI);
		m_arrParts[(int)eParts::NewSkill_2]->SetName(L"UI_LearnSkill2");
		m_arrParts[(int)eParts::NewSkill_2]->SetActive(false);
		SceneManager::DontDestroyOnLoad(m_arrParts[(int)eParts::NewSkill_2]);
		pMeshRender = m_arrParts[(int)eParts::NewSkill_2]->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		pMeshRender->SetMaterial(Resources::Find<Material>(L"SkillBoxFrame_Mtrl"));
		pSkillBtn = m_arrParts[(int)eParts::NewSkill_2]->AddComponent<SkillSelectScript>();
		pSkillBtn->SetPlayerScript(m_PlayerScript);
		pSkillBtn->SkillUI(this);
		pSkillBtn->SetSkill(m_arrSkill[(int)eSkillType::FireStrike]);
	}

	void SkillScript::Update()
	{
		if (m_CurState == eUIState::Select)
		{
			m_PlayerScript->Stop(true);
		}
			

		if (m_CurState == eUIState::Learn)
		{
			m_Time += (float)Time::DeltaTime();
			if (m_Time >= 3.f)
			{
				m_Time = 0.f;
				m_arrParts[(int)eParts::LearnSkill]->SetActive(false);
				m_arrParts[(int)eParts::LearnSkillIcon]->SetActive(false);
				m_CurState = eUIState::None;
			}
		}
	}

	void SkillScript::Render()
	{
		if (m_CurState == eUIState::Select)
		{
			std::wstring wstrText = L"새로운 스킬을 선택하세요!";
			wchar_t Font[256];
			wcscpy_s(Font, wstrText.c_str());
			FontWrapper::DrawFont(Font, 470.f, 250.f, 50.f, FONT_RGBA(255, 255, 255, 255));
		}
		else if (m_CurState == eUIState::Learn)
		{
			std::wstring wstrText = L"새로운 스킬 획득!";
			wchar_t Font[256];
			wcscpy_s(Font, wstrText.c_str());
			FontWrapper::DrawFont(Font, 622.f, 30.f, 30.f, FONT_RGBA(255, 255, 255, 255));
		
			wstrText = L"Lv.1 ";
			wstrText +=	m_RecentSkill->SkillName;
			wcscpy_s(Font, wstrText.c_str());

			switch (m_RecentSkill->SkillType)
			{
			case eSkillType::IceBall:
				FontWrapper::DrawFont(Font, 640.f, 80.f, 30.f, FONT_RGBA(255, 255, 255, 255));
				break;
			case eSkillType::FireStrike:
				FontWrapper::DrawFont(Font, 600.f, 80.f, 30.f, FONT_RGBA(255, 255, 255, 255));
				break;
			case eSkillType::None:
				break;
			default:
				break;
			}


			size_t lineBreakPos = m_RecentSkill->SkillDescription.find(L"\n");
			std::wstring firstLine, secondLine;

			if (lineBreakPos != std::string::npos)
			{
				firstLine = m_RecentSkill->SkillDescription.substr(0, lineBreakPos);
				secondLine = m_RecentSkill->SkillDescription.substr(lineBreakPos + 1);
			}
			else
			{
				firstLine = m_RecentSkill->SkillDescription;
				secondLine = L"";
			}

			wcscpy_s(Font, firstLine.c_str());

			switch (m_RecentSkill->SkillType)
			{
			case eSkillType::IceBall:
				FontWrapper::DrawFont(Font, 620.f, 135.f, 25.f, FONT_RGBA(255, 255, 255, 255));
				break;
			case eSkillType::FireStrike:
				FontWrapper::DrawFont(Font, 605.f, 135.f, 25.f, FONT_RGBA(255, 255, 255, 255));
				break;
			case eSkillType::None:
				break;
			default:
				break;
			}


			wcscpy_s(Font, secondLine.c_str());

			switch (m_RecentSkill->SkillType)
			{
			case eSkillType::IceBall:
				FontWrapper::DrawFont(Font, 530.f, 170.f, 25.f, FONT_RGBA(255, 255, 255, 255));
				break;
			case eSkillType::FireStrike:
				FontWrapper::DrawFont(Font, 575.f, 170.f, 25.f, FONT_RGBA(255, 255, 255, 255));
				break;
			case eSkillType::None:
				break;
			default:
				break;
			}
		}
	}

	void SkillScript::SkillUIOn()
	{
		m_CurState = eUIState::Select;

		m_arrParts[(int)eParts::NewSkill_1]->SetActive(true);
		m_arrParts[(int)eParts::NewSkill_1]->GetComponent<SkillSelectScript>()->SetActive(true);
		m_arrParts[(int)eParts::NewSkill_2]->SetActive(true);
		m_arrParts[(int)eParts::NewSkill_2]->GetComponent<SkillSelectScript>()->SetActive(true);

		GetOwner()->GetComponent<MeshRenderer>()->SetMaterial(Resources::Find<Material>(L"SkillBG_Mtrl"));
	}

	void SkillScript::SelectSkill(tSkill* _Skill)
	{
		m_CurState = eUIState::Learn;
		m_RecentSkill = _Skill;

		m_PlayerScript->Stop(false);

		m_arrParts[(int)eParts::NewSkill_1]->SetActive(false);
		m_arrParts[(int)eParts::NewSkill_1]->GetComponent<SkillSelectScript>()->SetActive(false);
		m_arrParts[(int)eParts::NewSkill_2]->SetActive(false);
		m_arrParts[(int)eParts::NewSkill_2]->GetComponent<SkillSelectScript>()->SetActive(false);

		m_arrParts[(int)eParts::LearnSkill]->SetActive(true);
		m_arrParts[(int)eParts::LearnSkillIcon]->SetActive(true);
		m_arrParts[(int)eParts::LearnSkillIcon]->GetComponent<MeshRenderer>()->SetMaterial(Resources::Find<Material>(m_RecentSkill->IconMaterialName));
		switch (m_RecentSkill->SkillType)
		{
		case eSkillType::IceBall:
			m_arrParts[(int)eParts::LearnSkillIcon]->GetComponent<Transform>()->SetPosition(Vector3(-120.f, 310.f, 150.f));
			break;
		case eSkillType::FireStrike:
			m_arrParts[(int)eParts::LearnSkillIcon]->GetComponent<Transform>()->SetPosition(Vector3(-157.f, 310.f, 150.f));
			break;
		case eSkillType::None:
			break;
		default:
			break;
		}

		GetOwner()->GetComponent<MeshRenderer>()->SetMaterial(Resources::Find<Material>(L"SkillBG_Transparent_Mtrl"));

		m_PlayerScript->LearnSkill(_Skill);
	}

	void SkillScript::ResetSkillUI()
	{
		GetOwner()->GetComponent<MeshRenderer>()->SetMaterial(Resources::Find<Material>(L"SkillBG_Transparent_Mtrl"));

		m_arrParts[(int)eParts::LearnSkill]->SetActive(false);
		m_arrParts[(int)eParts::LearnSkillIcon]->SetActive(false);
		m_arrParts[(int)eParts::LearnSkillIcon]->GetComponent<MeshRenderer>()->SetMaterial(nullptr);

		m_arrParts[(int)eParts::NewSkill_1]->SetActive(false);
		m_arrParts[(int)eParts::NewSkill_1]->GetComponent<SkillSelectScript>()->SetActive(false);
		m_arrParts[(int)eParts::NewSkill_2]->SetActive(false);
		m_arrParts[(int)eParts::NewSkill_2]->GetComponent<SkillSelectScript>()->SetActive(false);
		m_RecentSkill = nullptr;
		m_CurState = eUIState::None;
		m_Time = 0.f;
	}

	void SkillScript::CreateSkill()
	{
		// 아이스볼
		m_arrSkill[(int)eSkillType::IceBall] = new tSkill;
		m_arrSkill[(int)eSkillType::IceBall]->SkillType = eSkillType::IceBall;;
		m_arrSkill[(int)eSkillType::IceBall]->ElmentType = eElementType::Ice;
		m_arrSkill[(int)eSkillType::IceBall]->SkillCoolTime = 10.f;
		m_arrSkill[(int)eSkillType::IceBall]->CurCoolTime = 0.f;
		m_arrSkill[(int)eSkillType::IceBall]->NeedMana = 3;
		m_arrSkill[(int)eSkillType::IceBall]->IconMaterialName = L"IceBallIcon_Mtrl";
		m_arrSkill[(int)eSkillType::IceBall]->SkillName = L"아이스볼";
		m_arrSkill[(int)eSkillType::IceBall]->SkillDescription = L"스킬을 유지하는 동안\n얼음 덩어리를 발사해 마법대미지를 준다.";

		// 파이어스트라이크
		m_arrSkill[(int)eSkillType::FireStrike] = new tSkill;
		m_arrSkill[(int)eSkillType::FireStrike]->SkillType = eSkillType::FireStrike;;
		m_arrSkill[(int)eSkillType::FireStrike]->ElmentType = eElementType::Fire;
		m_arrSkill[(int)eSkillType::FireStrike]->SkillCoolTime = 25.f;
		m_arrSkill[(int)eSkillType::FireStrike]->CurCoolTime = 0.f;
		m_arrSkill[(int)eSkillType::FireStrike]->NeedMana = 4;
		m_arrSkill[(int)eSkillType::FireStrike]->IconMaterialName = L"FireStrikeIcon_Mtrl";
		m_arrSkill[(int)eSkillType::FireStrike]->SkillName = L"파이어 스트라이크";
		m_arrSkill[(int)eSkillType::FireStrike]->SkillDescription = L"바닥에서 화염을 끌어올려\n넓은 범위에 마법대미지를 준다.";
	}
}