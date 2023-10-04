#pragma once
#include "..\\Engine_SOURCE\\Lu_Script.h"

namespace Lu
{
	class Animator;
	class ChestScript : public Script
	{
	public:
		ChestScript();
		virtual ~ChestScript();

	private:
		Animator*	m_Animator;
		GameObject* m_ChestFX;
		float		m_Time;
		bool		m_bMove;
		bool		m_bOpen;

	public:
		virtual void Initialize() override;
		virtual void Update() override;

	public:
		virtual void OnCollisionStay(Collider2D* _Other);

	private:
		void CreateReward();
		void ChestOpenFXComplete();
	};
}
