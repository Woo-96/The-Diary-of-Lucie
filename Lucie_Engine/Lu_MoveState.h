#pragma once
#include "Lu_StateScript.h"

namespace Lu
{
	class AudioClip;
	class MoveState : public StateScript
	{
	public:
		MoveState();
		virtual ~MoveState();

	private:
		std::shared_ptr<AudioClip>	m_SFX;

	public:
		virtual void Initialize() override;
		virtual void Update() override;

	public:
		virtual void Enter() override;
		virtual void Exit() override;
	};
}