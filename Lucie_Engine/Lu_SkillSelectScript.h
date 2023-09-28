#pragma once
#include "Lu_ButtonScript.h"
#include "Lu_ScriptEnums.h"

namespace Lu
{
	class PlayerScript;
	class SkillScript;
	class GameObject;
    class SkillSelectScript : public ButtonScript
    {
	public:
		enum class eParts
		{
			SkillIcon,
			SkillElement,
			End,
		};

	public:
		SkillSelectScript();
		virtual ~SkillSelectScript();

	private:
		PlayerScript*	m_PlayerScript;
		SkillScript*	m_SkillUI;
		tSkill*			m_Skill;
		GameObject*		m_SkillIcon[(int)eParts::End];
		bool			m_bPrint;

	public:
		void SetPlayerScript(PlayerScript* _PlayerScript)
		{
			m_PlayerScript = _PlayerScript;
		}

		void SkillUI(SkillScript* _SkillUI)
		{
			m_SkillUI = _SkillUI;
		}

		void SetSkill(tSkill* _Skill);

	public:
		virtual void Update() override;
		virtual void Render() override;

	protected:
		virtual void ButtonNomalEvent() override;
		virtual void ButtonHoveredEvent() override;
		virtual void ButtonClickEvent() override;

	public:
		void SetActive(bool _b);
    };
}


