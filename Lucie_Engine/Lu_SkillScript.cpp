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
		// ��ų ����
		CreateSkill();

		// ��ų UI ����
		MeshRenderer* pMeshRender;

		m_arrParts[(int)eParts::LearnSkill] = object::Instantiate<GameObject>(Vector3(0.f, 200.f, 150.f), Vector3(600.f, 169.5f, 100.f), eLayerType::UI);
		m_arrParts[(int)eParts::LearnSkill]->SetName(L"UI_LearnSkill");
		m_arrParts[(int)eParts::LearnSkill]->SetActive(false);
		SceneManager::DontDestroyOnLoad(m_arrParts[(int)eParts::LearnSkill]);
		pMeshRender = m_arrParts[(int)eParts::LearnSkill]->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		pMeshRender->SetMaterial(Resources::Find<Material>(L"LearnSkillFrame_Mtrl"));

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
	}

	void SkillScript::Update()
	{
		if(m_CurState == eUIState::Select)
			m_PlayerScript->SetCantHit(true);

		if (m_CurState == eUIState::Learn)
		{
			m_Time += (float)Time::DeltaTime();
			if (m_Time >= 3.f)
			{
				m_Time = 0.f;
				m_arrParts[(int)eParts::LearnSkill]->SetActive(false);
			}
		}
	}

	void SkillScript::Render()
	{
		if (m_CurState == eUIState::Select)
		{
			std::wstring wstrText = L"���ο� ��ų�� �����ϼ���!";
			wchar_t Font[256];
			wcscpy_s(Font, wstrText.c_str());
			FontWrapper::DrawFont(Font, 470.f, 250.f, 50.f, FONT_RGBA(255, 255, 255, 255));
		}
		else
		{

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

		m_PlayerScript->SetCantHit(false);

		m_arrParts[(int)eParts::NewSkill_1]->SetActive(false);
		m_arrParts[(int)eParts::NewSkill_1]->GetComponent<SkillSelectScript>()->SetActive(false);
		m_arrParts[(int)eParts::NewSkill_2]->SetActive(false);
		m_arrParts[(int)eParts::NewSkill_2]->GetComponent<SkillSelectScript>()->SetActive(false);

		m_arrParts[(int)eParts::LearnSkill]->SetActive(true);

		GetOwner()->GetComponent<MeshRenderer>()->SetMaterial(nullptr);

		m_PlayerScript->LearnSkill(_Skill);
	}

	void SkillScript::CreateSkill()
	{
		// ���̽���
		m_arrSkill[(int)eSkillType::IceBall] = new tSkill;
		m_arrSkill[(int)eSkillType::IceBall]->SkillType = eSkillType::IceBall;;
		m_arrSkill[(int)eSkillType::IceBall]->ElmentType = eElementType::Ice;
		m_arrSkill[(int)eSkillType::IceBall]->SkillCoolTime = 10.f;
		m_arrSkill[(int)eSkillType::IceBall]->CurCoolTime = 0.f;
		m_arrSkill[(int)eSkillType::IceBall]->NeedMana = 3;
		m_arrSkill[(int)eSkillType::IceBall]->IconMaterialName = L"IceBallIcon_Mtrl";
		m_arrSkill[(int)eSkillType::IceBall]->SkillName = L"���̽���";
		m_arrSkill[(int)eSkillType::IceBall]->SkillDescription = L"��ų�� �����ϴ� ����\n���� ����� �߻��� ����������� �ش�.";
	}
}