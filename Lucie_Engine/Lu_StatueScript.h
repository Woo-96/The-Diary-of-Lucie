#pragma once
#include "..\\Engine_SOURCE\\Lu_Script.h"

namespace Lu
{
	class Animator;
	class StatueScript : public Script
	{
	public:
		enum class eStatueColor
		{
			None,
			Red,
			Blue,
		};

		enum class eStatueType
		{
			None,
			Angel,
			Devil,
		};

		enum class eStatueState
		{
			None,
			Active,
			DeActive,
		};

	public:
		StatueScript();
		virtual ~StatueScript();

	private:
		eStatueColor	m_StatueColor;
		eStatueType		m_StatueType;
		eStatueState	m_StatueState;
		StatueScript*	m_AnotherStatue;
		Animator*		m_Animator;
		std::wstring	m_DeActiveAnimName;

	public:
		void SetStatueColor(eStatueColor _Color)
		{
			m_StatueColor = _Color;
		}

		void SetStatueType(eStatueType _Type)
		{
			m_StatueType = _Type;
		}

		void SetAnotherStatue(StatueScript* _Other)
		{
			m_AnotherStatue = _Other;
		}

	public:
		eStatueState GetStatueState()	const
		{
			return m_StatueState;
		}

	public:
		virtual void Initialize() override;
		virtual void Update() override;

	public:
		virtual void OnCollisionEnter(Collider2D* _Other);

	public:
		void CreateStatueAnimation(Animator* _Animator);
		void DeActiveStatue();

	private:
		void StatueEfficacy();
	};
}
