#pragma once
#include "Lu_UIScript.h"

namespace Lu
{
	class QuickItem : public UIScript
	{
	public:
		QuickItem();
		virtual ~QuickItem();

	public:
		virtual void Update() override;
	};
}