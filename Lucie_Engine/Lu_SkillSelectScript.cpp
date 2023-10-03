#include "Lu_SkillSelectScript.h"
#include "Lu_PlayerScript.h"
#include "Lu_SkillScript.h"
#include "Lu_FontWrapper.h"
#include "Lu_Object.h"
#include "Lu_GameObject.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_FontScript.h"
#include "Lu_SoundManager.h"
#include "Lu_AudioSource.h"

namespace Lu
{
    SkillSelectScript::SkillSelectScript()
        : m_PlayerScript(nullptr)
        , m_SkillUI(nullptr)
        , m_Skill{}
        , m_SkillIcon{}
        , m_bPrint(false)
    {
        SetName(L"SkillSelectScript");
    }

    SkillSelectScript::~SkillSelectScript()
    {
    }

    void SkillSelectScript::SetSkill(tSkill* _Skill)
    {
        m_Skill = _Skill;

        Vector3 vPos = GetOwner()->GetComponent<Transform>()->GetPosition();
        vPos.y += 50.f;
        m_SkillIcon[(int)eParts::SkillIcon] = object::Instantiate<GameObject>(vPos, Vector3(85.f, 85.f, 100.f), eLayerType::UI);
        m_SkillIcon[(int)eParts::SkillIcon]->SetName(L"BtnSkillIcon");
        m_SkillIcon[(int)eParts::SkillIcon]->SetActive(false);
        SceneManager::DontDestroyOnLoad(m_SkillIcon[(int)eParts::SkillIcon]);
        MeshRenderer* pMeshRender = m_SkillIcon[(int)eParts::SkillIcon]->AddComponent<MeshRenderer>();
        pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
        pMeshRender->SetMaterial(Resources::Find<Material>(m_Skill->IconMaterialName));

        vPos.y -= 75.f;
        m_SkillIcon[(int)eParts::SkillElement] = object::Instantiate<GameObject>(vPos, Vector3(36.f, 36.f, 100.f), eLayerType::UI);
        m_SkillIcon[(int)eParts::SkillElement]->SetName(L"BtnSkillElement");
        m_SkillIcon[(int)eParts::SkillElement]->SetActive(false);
        SceneManager::DontDestroyOnLoad(m_SkillIcon[(int)eParts::SkillElement]);
        pMeshRender = m_SkillIcon[(int)eParts::SkillElement]->AddComponent<MeshRenderer>();
        pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));

        switch (m_Skill->ElmentType)
        {
        case Lu::eElementType::None:
            pMeshRender->SetMaterial(Resources::Find<Material>(L"CurrentSkill_None_Mtrl"));
            break;
        case Lu::eElementType::Fire:
            pMeshRender->SetMaterial(Resources::Find<Material>(L"CurrentSkill_Fire_Mtrl"));
            break;
        case Lu::eElementType::Ice:
            pMeshRender->SetMaterial(Resources::Find<Material>(L"CurrentSkill_Ice_Mtrl"));
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

        FontScript* pFont = m_SkillIcon[(int)eParts::SkillElement]->AddComponent<FontScript>();
        pFont->SetText(std::to_wstring(m_Skill->NeedMana));

        switch (m_Skill->SkillType)
        {
        case eSkillType::IceBall:
            pFont->SetPos(Vector2(571.f, 467.f));
            break;
        case eSkillType::FireStrike:
            pFont->SetPos(Vector2(830.f, 467.f));
            break;
        case eSkillType::None:
            break;
        default:
            break;
        }

        pFont->SetSize(30.f);
        pFont->SetRGB(FONT_RGBA(255, 215, 0, 255));
    }

    void SkillSelectScript::Update()
    {
        ButtonScript::Update();
    }

    void SkillSelectScript::Render()
    {
        // 스킬 이름 & 설명
        if (m_Skill)
        {
            // 스킬 이름
            std::wstring wstrText = m_Skill->SkillName;
            wchar_t Font[256];
            wcscpy_s(Font, wstrText.c_str());

            switch (m_Skill->SkillType)
            {
            case eSkillType::IceBall:
                FontWrapper::DrawFont(Font, 532.f, 510.f, 30.f, FONT_RGBA(255, 255, 255, 255));
                break;
            case eSkillType::FireStrike:
                FontWrapper::DrawFont(Font, 752.f, 510.f, 27.f, FONT_RGBA(255, 255, 255, 255));
                break;
            case eSkillType::None:
                break;
            default:
                break;
            }
            
            if (m_bPrint)
            {
                wstrText = m_Skill->SkillName;
                wcscpy_s(Font, wstrText.c_str());

                switch (m_Skill->SkillType)
                {
                case eSkillType::IceBall:
                    FontWrapper::DrawFont(Font, 660.f, 700.f, 30.f, FONT_RGBA(255, 215, 0, 255));
                    break;
                case eSkillType::FireStrike:
                    FontWrapper::DrawFont(Font, 620.f, 700.f, 30.f, FONT_RGBA(255, 215, 0, 255));
                    break;
                case eSkillType::None:
                    break;
                default:
                    break;
                }


                size_t lineBreakPos = m_Skill->SkillDescription.find(L"\n");
                std::wstring firstLine, secondLine;

                if (lineBreakPos != std::string::npos)
                {
                    firstLine = m_Skill->SkillDescription.substr(0, lineBreakPos);
                    secondLine = m_Skill->SkillDescription.substr(lineBreakPos + 1);
                }
                else
                {
                    firstLine = m_Skill->SkillDescription;
                    secondLine = L"";
                }

                wcscpy_s(Font, firstLine.c_str());

                switch (m_Skill->SkillType)
                {
                case eSkillType::IceBall:
                    FontWrapper::DrawFont(Font, 620.f, 740.f, 25.f, FONT_RGBA(255, 255, 255, 255));
                    break;
                case eSkillType::FireStrike:
                    FontWrapper::DrawFont(Font, 600.f, 740.f, 25.f, FONT_RGBA(255, 255, 255, 255));
                    break;
                case eSkillType::None:
                    break;
                default:
                    break;
                }

                wcscpy_s(Font, secondLine.c_str());

                switch (m_Skill->SkillType)
                {
                case eSkillType::IceBall:
                    FontWrapper::DrawFont(Font, 530.f, 770.f, 25.f, FONT_RGBA(255, 255, 255, 255));
                    break;
                case eSkillType::FireStrike:
                    FontWrapper::DrawFont(Font, 575.f, 770.f, 25.f, FONT_RGBA(255, 255, 255, 255));
                    break;
                case eSkillType::None:
                    break;
                default:
                    break;
                }
            }
        }
    }

    void SkillSelectScript::ButtonNomalEvent()
    {
        m_bPrint = false;
    }

    void SkillSelectScript::ButtonHoveredEvent()
    {
        m_bPrint = true;
    }

    void SkillSelectScript::ButtonClickEvent()
    {
        if (m_SkillUI && m_Skill)
        {
            m_SkillUI->SelectSkill(m_Skill);

            AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
            pSFX->SetClip(Resources::Load<AudioClip>(L"ButtonClickSFX", L"..\\Resources\\Sound\\SFX\\Player\\ButtonClickSFX.ogg"));
            pSFX->Play();
        }
    }

    void SkillSelectScript::SetActive(bool _b)
    {
        for (int i = 0; i < (int)eParts::End; ++i)
        {
            if(m_SkillIcon[i])
                m_SkillIcon[i]->SetActive(_b);
        }
    }
}