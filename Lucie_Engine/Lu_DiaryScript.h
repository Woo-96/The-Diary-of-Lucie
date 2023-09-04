#pragma once
#include "Lu_PortalScript.h"

namespace Lu
{
	class Animator;
	class DiaryScript : public PortalScript
	{
	public:
		DiaryScript();
		virtual ~DiaryScript();

	private:
		Animator* m_Animator;

	public:
		virtual void Initialize() override;
		virtual void Update() override {};

	public:
		virtual void OnCollisionStay(Collider2D* _Other);

	private:
		void CreateDiaryAnimation();
		void GameStart();
	};
}
