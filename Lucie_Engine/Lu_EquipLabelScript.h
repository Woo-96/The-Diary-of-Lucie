#pragma once
#include "Lu_LabelScript.h"

namespace Lu
{
	class EquipLabelScript : public LabelScript
	{
	public:
		EquipLabelScript();
		virtual ~EquipLabelScript();

	public:
		virtual void Update() override {}
		virtual void Render() override;
	};
}


