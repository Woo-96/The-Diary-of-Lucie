#pragma once
#include "..\\Engine_SOURCE\\Lu_Script.h"

namespace Lu
{
	class Animator;
	class DiaryScript : public Script
	{
	public:
		DiaryScript();
		virtual ~DiaryScript();

	private:
		Animator* m_Animator;

	public:
		virtual void Initialize() override;

	public:
		virtual void OnCollisionStay(Collider2D* _Other);

	private:
		void CreateDiaryAnimation();
		void GameStart();
		void DiarySFX();
	};
}
