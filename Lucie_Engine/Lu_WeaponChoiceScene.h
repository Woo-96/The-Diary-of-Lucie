#pragma once
#include "Lu_StageScene.h"
#include "Lu_WeaponScript.h"
#include "Lu_ScriptEnums.h"

namespace Lu
{
	class WeaponChoiceScene : public StageScene
	{
	public:
		WeaponChoiceScene();
		virtual ~WeaponChoiceScene();

	private:
		WeaponScript* m_arrWeapon[(int)eWeaponType::None];

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void Destroy() override;

	public:
		virtual void OnEnter() override;
		virtual void OnExit() override;

	public:
		void PlayerWeaponGet(eWeaponType _Type);
	};
}