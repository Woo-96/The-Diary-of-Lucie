#pragma once
#include "Lu_UIScript.h"
#include "Lu_ScriptEnums.h"

namespace Lu
{
	class PlayerScript;
	class GameObject;
	class SkillScript :	public UIScript
	{
	public:
		enum class eParts
		{
			LearnSkill,
			LearnSkillIcon,
			NewSkill_1,
			NewSkill_2,
			End,
		};

		enum class eUIState
		{
			None,
			Select,
			Learn,
		};

	public:
		SkillScript();
		virtual ~SkillScript();

	private:
		PlayerScript*	m_PlayerScript;
		GameObject*		m_arrParts[(int)eParts::End];
		tSkill*			m_arrSkill[(int)eSkillType::None];
		tSkill*			m_RecentSkill;
		eUIState		m_CurState;
		float			m_Time;

	public:
		void SetPlayerScript(PlayerScript* _PlayerScript)
		{
			m_PlayerScript = _PlayerScript;
		}

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render() override;

	public:
		void SkillUIOn();
		void SelectSkill(tSkill* _Skill);
		void ResetSkillUI();

	private:
		void CreateSkill();
	};
}


