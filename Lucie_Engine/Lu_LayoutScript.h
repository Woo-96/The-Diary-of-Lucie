#pragma once
#include "Lu_UIScript.h"
#include "Lu_ScriptEnums.h"

namespace Lu
{
	class PlayerScript;
	class LayoutScript : public UIScript
	{
	private:
		enum class eParts
		{
			ElementIcon,
			SkillSlot_1,
			SkillSlot_2,
			SkillSlot_3,
			SkillSlot_4,
			AroundSkillSlot,
			CurSkillIcon,
			CoolTime,
			SlotCoolTime,
			End,
		};

	public:
		LayoutScript();
		virtual ~LayoutScript();

	private:
		PlayerScript*	m_PlayerScript;
		GameObject*		m_arrParts[(int)eParts::End];
		tSkill			m_arrSkill[MaxSkill];
		int				m_SkillCount;
		int				m_CurSlot;

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
		void SelectSlot(int _Num); // 1 ~ 4 스킬 슬롯 선택
		void LearnSkill(tSkill& _Skill);
	};
}
