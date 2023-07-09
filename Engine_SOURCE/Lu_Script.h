#pragma once
#include "Lu_Component.h"

namespace Lu
{
	class Script : public Component
	{
	public:
		Script();
		virtual ~Script();

	private:

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
	};
}