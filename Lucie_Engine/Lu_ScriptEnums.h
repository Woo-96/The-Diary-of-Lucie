#pragma once

namespace Lu
{
#define MaxSkill 4

	enum class eDir
	{
		None,
		Left,
		Right,
		Up,
		Down,
		LeftUp,
		RightUp,
		LeftDown,
		RightDown
	};

	enum class eWeaponType
	{
		Wand,
		Bow,
		Sword,
		None,
	};

	enum class eElementType
	{
		None,
		Fire,
		Ice,
		Wind,
		Thunder,
		White,
		Dark,
		End,
	};

	enum class eSkillType
	{
		IceBall,
		FireStrike,
		None,
	};

	struct tSkill
	{
		eSkillType		SkillType;
		eElementType	ElmentType;
		float			SkillCoolTime;
		float			CurCoolTime;
		int				NeedMana;
		std::wstring	IconMaterialName;
		std::wstring	SkillName;
		std::wstring	SkillDescription;

		tSkill()
			: SkillType(eSkillType::None)
			, ElmentType(eElementType::End)
			, SkillCoolTime(0.f)
			, CurCoolTime(0.f)
			, NeedMana(0)
			, IconMaterialName{}
			, SkillName{}
			, SkillDescription{}
		{

		}
	};
}