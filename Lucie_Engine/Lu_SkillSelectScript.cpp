#include "Lu_SkillSelectScript.h"
#include "Lu_PlayerScript.h"
#include "Lu_SkillScript.h"
#include "Lu_FontWrapper.h"
#include "Lu_Object.h"
#include "Lu_GameObject.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"

namespace Lu
{
    SkillSelectScript::SkillSelectScript()
        : m_PlayerScript(nullptr)
        , m_SkillUI(nullptr)
        , m_Skill{}
        , m_SkillIcon{}
    {
        SetName(L"SkillSelectScript");
    }

    SkillSelectScript::~SkillSelectScript()
    {
    }

    void SkillSelectScript::SetSkill(tSkill _Skill)
    {
        m_Skill = _Skill;

        Vector3 vPos = GetOwner()->GetComponent<Transform>()->GetPosition();
        vPos.y += 20.f;
        m_SkillIcon[(int)eParts::SkillIcon] = object::Instantiate<GameObject>(vPos, Vector3(72.f, 72.f, 100.f), eLayerType::UI);
        m_SkillIcon[(int)eParts::SkillIcon]->SetName(L"BtnSkillIcon");
        SceneManager::DontDestroyOnLoad(m_SkillIcon[(int)eParts::SkillIcon]);
        MeshRenderer* pMeshRender = m_SkillIcon[(int)eParts::SkillIcon]->AddComponent<MeshRenderer>();
        pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
        pMeshRender->SetMaterial(Resources::Find<Material>(m_Skill.IconMaterialName));


    }

    void SkillSelectScript::Update()
    {
        ButtonScript::Update();

        if (!(m_Skill.SkillType == eSkillType::None))
        {
            std::wstring wstrText = m_Skill.SkillName;
            wchar_t Font[256];
            wcscpy_s(Font, wstrText.c_str());
            FontWrapper::DrawFont(Font, 600.f, 400.f, 20.f, FONT_RGBA(255, 255, 0, 255));
        
            wstrText = m_Skill.SkillDescription;
            wcscpy_s(Font, wstrText.c_str());
            FontWrapper::DrawFont(Font, 550.f, 420.f, 20.f, FONT_RGBA(255, 255, 255, 255));
        }
    }

    void SkillSelectScript::ButtonNomalEvent()
    {

    }

    void SkillSelectScript::ButtonHoveredEvent()
    {

    }

    void SkillSelectScript::ButtonClickEvent()
    {
        if(m_SkillUI && !(m_Skill.SkillType == eSkillType::None))
            m_SkillUI->SelectSkill(m_Skill);
    }
}