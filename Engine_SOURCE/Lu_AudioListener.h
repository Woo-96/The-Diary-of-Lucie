#pragma once
#include "Lu_Component.h"

namespace Lu
{
	class AudioListener : public Component
	{
	public:
		AudioListener();
		virtual ~AudioListener();

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
	};
}