#pragma once
#include "Lu_UIScript.h"

namespace Lu
{
	class WeaponSlot : public UIScript
	{
	public:
		WeaponSlot();
		virtual ~WeaponSlot();

	public:
		virtual void Update() override;
	};
}