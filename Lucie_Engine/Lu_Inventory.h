#pragma once
#include "Lu_UIScript.h"

namespace Lu
{
	class Inventory : public UIScript
	{
	public:
		Inventory();
		virtual ~Inventory();

	public:
		virtual void Update() override;
	};
}