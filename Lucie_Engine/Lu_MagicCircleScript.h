#pragma once
#include "..\\Engine_SOURCE\\Lu_Script.h"

namespace Lu
{
	class Animator;
	class MagicCircleScript : public Script
	{
	public:
		MagicCircleScript();
		virtual ~MagicCircleScript();

	private:
		Animator*	m_Animator;

	public:
		virtual void Initialize() override;

	public:
		virtual void OnCollisionStay(Collider2D* _Other);

	private:
		void CreateMagicCircleAnimation();

		// ���ҽ� �����ϱ� �Ⱦ ����
		void MagicCircle1Finished();
		void MagicCircle2Finished();
		void MagicCircle3Finished();
		void MagicCircle4Finished();
		void MagicCircle5Finished();
	};
}
