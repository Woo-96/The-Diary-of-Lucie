#include "Lu_LayoutScript.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_PlayerScript.h"
#include "Lu_FontWrapper.h"
#include "Lu_Time.h"

namespace Lu
{
	LayoutScript::LayoutScript()
		: m_PlayerScript(nullptr)
		, m_arrParts{}
		, m_arrSkill{}
		, m_SkillCount(0)
		, m_CurSlot(0)
	{
		SetName(L"LayoutScript");
	}

	LayoutScript::~LayoutScript()
	{
	}

	void LayoutScript::Initialize()
	{
		MeshRenderer* pMeshRender;

		m_arrParts[(int)eParts::ElementIcon] = object::Instantiate<GameObject>(Vector3(-8.f, -293.f, 160.f), Vector3(28.f, 28.f, 100.f), eLayerType::UI);
		m_arrParts[(int)eParts::ElementIcon]->SetName(L"ElementIcon");
		SceneManager::DontDestroyOnLoad(m_arrParts[(int)eParts::ElementIcon]);
		pMeshRender = m_arrParts[(int)eParts::ElementIcon]->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		pMeshRender->SetMaterial(Resources::Find<Material>(L"CurrentSkill_None_Mtrl"));

		m_arrParts[(int)eParts::SkillSlot_1] = object::Instantiate<GameObject>(Vector3(-70.f, -235.f, 160.f), Vector3(32.f, 32.f, 100.f), eLayerType::UI);
		m_arrParts[(int)eParts::SkillSlot_1]->SetName(L"SkillSlot_1");
		SceneManager::DontDestroyOnLoad(m_arrParts[(int)eParts::SkillSlot_1]);
		pMeshRender = m_arrParts[(int)eParts::SkillSlot_1]->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));

		m_arrParts[(int)eParts::SkillSlot_2] = object::Instantiate<GameObject>(Vector3(-33.f, -205.f, 160.f), Vector3(32.f, 32.f, 100.f), eLayerType::UI);
		m_arrParts[(int)eParts::SkillSlot_2]->SetName(L"SkillSlot_2");
		SceneManager::DontDestroyOnLoad(m_arrParts[(int)eParts::SkillSlot_2]);
		pMeshRender = m_arrParts[(int)eParts::SkillSlot_2]->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));

		m_arrParts[(int)eParts::SkillSlot_3] = object::Instantiate<GameObject>(Vector3(13.f, -205.f, 160.f), Vector3(32.f, 32.f, 100.f), eLayerType::UI);
		m_arrParts[(int)eParts::SkillSlot_3]->SetName(L"SkillSlot_3");
		SceneManager::DontDestroyOnLoad(m_arrParts[(int)eParts::SkillSlot_3]);
		pMeshRender = m_arrParts[(int)eParts::SkillSlot_3]->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));

		m_arrParts[(int)eParts::SkillSlot_4] = object::Instantiate<GameObject>(Vector3(50.f, -235.f, 160.f), Vector3(32.f, 32.f, 100.f), eLayerType::UI);
		m_arrParts[(int)eParts::SkillSlot_4]->SetName(L"SkillSlot_4");
		SceneManager::DontDestroyOnLoad(m_arrParts[(int)eParts::SkillSlot_4]);
		pMeshRender = m_arrParts[(int)eParts::SkillSlot_4]->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));

		m_arrParts[(int)eParts::AroundSkillSlot] = object::Instantiate<GameObject>(Vector3(50.f, -235.f, 170.f), Vector3(60.f, 60.f, 100.f), eLayerType::UI);
		m_arrParts[(int)eParts::AroundSkillSlot]->SetName(L"AroundSkillSlot");
		m_arrParts[(int)eParts::AroundSkillSlot]->SetActive(false);
		SceneManager::DontDestroyOnLoad(m_arrParts[(int)eParts::AroundSkillSlot]);
		pMeshRender = m_arrParts[(int)eParts::AroundSkillSlot]->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		pMeshRender->SetMaterial(Resources::Find<Material>(L"CurrentSkill_Mtrl"));

		m_arrParts[(int)eParts::CurSkillIcon] = object::Instantiate<GameObject>(Vector3(-8.f, -265.f, 200.f), Vector3(48.f, 48.f, 100.f), eLayerType::UI);
		m_arrParts[(int)eParts::CurSkillIcon]->SetName(L"CurSkillIcon");
		SceneManager::DontDestroyOnLoad(m_arrParts[(int)eParts::CurSkillIcon]);
		pMeshRender = m_arrParts[(int)eParts::CurSkillIcon]->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
	}

	void LayoutScript::Render()
	{
		if (0 < m_CurSlot && m_CurSlot < 5)
		{
			int NeedMana = m_arrSkill[(int)m_CurSlot - 1]->NeedMana;
			if (0 != NeedMana)
			{
				std::wstring ManaCount = std::to_wstring(NeedMana);
				wchar_t Font[256];
				wcscpy_s(Font, ManaCount.c_str());
				FontWrapper::DrawFont(Font, 705.f, 687.f, 25.f, FONT_RGBA(255, 215, 0, 255));
			}
		}

		// 스킬 쿨타임 관리
		for (int i = 0; i < MaxSkill; ++i)
		{
			if (m_arrSkill[i] && m_arrSkill[i]->CurCoolTime > 0.f)
			{
				m_arrSkill[i]->CurCoolTime -= (float)Time::DeltaTime();

				int CoolTime = (int)m_arrSkill[i]->CurCoolTime;
				std::wstring Cool = std::to_wstring(CoolTime);
				wchar_t Font[256];
				wcscpy_s(Font, Cool.c_str());

				if (CoolTime >= 0)
				{
					Vector2 vSlotPos = Vector2::Zero;
					switch (i)
					{
					case 0:
						vSlotPos = Vector2(644.f, 630.f);
						break;
					case 1:
						vSlotPos = Vector2(681.f, 601.f);
						break;
					case 2:
						vSlotPos = Vector2(727.f, 601.f);
						break;
					case 3:
						vSlotPos = Vector2(764.f, 630.f);
						break;
					default:
						break;
					}

					if (CoolTime <= 5)
					{
						FontWrapper::DrawFont(Font, vSlotPos.x, vSlotPos.y, 20.f, FONT_RGBA(255, 255, 255, 255));
					}
					else if (CoolTime >= 10)
					{
						FontWrapper::DrawFont(Font, vSlotPos.x - 6.f, vSlotPos.y, 20.f, FONT_RGBA(255, 255, 255, 255));
					}
					else
					{
						FontWrapper::DrawFont(Font, vSlotPos.x, vSlotPos.y, 20.f, FONT_RGBA(255, 0, 0, 255));
					}
				}

				if (m_PlayerScript->GetCurSkill() == m_arrSkill[i])
				{
					if (CoolTime <= 5)
					{
						FontWrapper::DrawFont(Font, 703.f, 653.f, 30.f, FONT_RGBA(255, 255, 255, 255));
					}
					else if (CoolTime >= 10)
					{
						FontWrapper::DrawFont(Font, 703.f - 7.f, 653.f, 30.f, FONT_RGBA(255, 0, 0, 255));
					}
					else
					{
						FontWrapper::DrawFont(Font, 703.f, 653.f, 30.f, FONT_RGBA(255, 0, 0, 255));
					}
				}

				if (m_arrSkill[i]->CurCoolTime <= 0.f)
					m_arrSkill[i]->CurCoolTime = 0.f;
			}
		}
	}

	void LayoutScript::SelectSlot(int _Num)
	{
		if (m_CurSlot == _Num)
			return;

		if (!m_arrSkill[(int)_Num - 1])
			return;

		if (0 < _Num && _Num < 5)
		{
			// 현재 슬롯으로 지정
			m_CurSlot = _Num;
			
			// 슬롯에 저장되어 있던 스킬의 아이콘 설정 및 원소 색상 결정
			std::wstring Name = m_arrSkill[(int)m_CurSlot - 1]->IconMaterialName;
			m_arrParts[(int)eParts::CurSkillIcon]->GetComponent<MeshRenderer>()->SetMaterial(Resources::Find<Material>(Name));

			switch (m_arrSkill[(int)m_CurSlot - 1]->ElmentType)
			{
			case Lu::eElementType::None:
				m_arrParts[(int)eParts::ElementIcon]->GetComponent<MeshRenderer>()->SetMaterial(Resources::Find<Material>(L"CurrentSkill_None_Mtrl"));
				break;
			case Lu::eElementType::Fire:
				m_arrParts[(int)eParts::ElementIcon]->GetComponent<MeshRenderer>()->SetMaterial(Resources::Find<Material>(L"CurrentSkill_Fire_Mtrl"));
				break;
			case Lu::eElementType::Ice:
				m_arrParts[(int)eParts::ElementIcon]->GetComponent<MeshRenderer>()->SetMaterial(Resources::Find<Material>(L"CurrentSkill_Ice_Mtrl"));
				break;
			case Lu::eElementType::Wind:
				break;
			case Lu::eElementType::Thunder:
				break;
			case Lu::eElementType::White:
				break;
			case Lu::eElementType::Dark:
				break;
			case Lu::eElementType::End:
				break;
			default:
				break;
			}

			// 테두리 위치 변경
			m_arrParts[(int)eParts::AroundSkillSlot]->SetActive(true);
			Vector3 vSlotPos = m_arrParts[m_CurSlot]->GetComponent<Transform>()->GetPosition();
			m_arrParts[(int)eParts::AroundSkillSlot]->GetComponent<Transform>()->SetPosition(vSlotPos);

			m_PlayerScript->SetCurSkill(m_arrSkill[(int)m_CurSlot - 1]);
		}
	}

	void LayoutScript::LearnSkill(tSkill* _Skill)
	{
		if (m_SkillCount + 1 > MaxSkill)
			return;

		if (!m_arrSkill[m_SkillCount])
		{
			m_arrSkill[m_SkillCount] = _Skill;
			std::wstring Name = m_arrSkill[m_SkillCount]->IconMaterialName;
			m_arrParts[++m_SkillCount]->GetComponent<MeshRenderer>()->SetMaterial(Resources::Find<Material>(Name));
		}
	}

	void LayoutScript::ResetLayout()
	{
		m_arrParts[(int)eParts::ElementIcon]->GetComponent<MeshRenderer>()->SetMaterial(Resources::Find<Material>(L"CurrentSkill_None_Mtrl"));
		m_arrParts[(int)eParts::AroundSkillSlot]->SetActive(false);
		m_arrParts[(int)eParts::SkillSlot_1]->GetComponent<MeshRenderer>()->SetMaterial(nullptr);
		m_arrParts[(int)eParts::SkillSlot_2]->GetComponent<MeshRenderer>()->SetMaterial(nullptr);
		m_arrParts[(int)eParts::SkillSlot_3]->GetComponent<MeshRenderer>()->SetMaterial(nullptr);
		m_arrParts[(int)eParts::SkillSlot_4]->GetComponent<MeshRenderer>()->SetMaterial(nullptr);
		m_arrParts[(int)eParts::CurSkillIcon]->GetComponent<MeshRenderer>()->SetMaterial(nullptr);


		for (int i = 0; i < MaxSkill; ++i)
		{
			if(m_arrSkill[i])
			{
				m_arrSkill[i]->CurCoolTime = 0.f;
				m_arrSkill[i] = nullptr;
			}
		}

		m_SkillCount = 0;
		m_CurSlot = 0;
	}

	void LayoutScript::SetActive(bool _b)
	{
		for (int i = 0; i < (int)eParts::End; ++i)
		{
			if (i == (int)eParts::AroundSkillSlot && 0 == m_CurSlot)
				continue;
				
			if (m_arrParts[i])
				m_arrParts[i]->SetActive(_b);
		}

		GetOwner()->SetActive(_b);
	}
}