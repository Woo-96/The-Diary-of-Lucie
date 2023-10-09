#pragma once
#include "Lu_StageScene.h"

namespace Lu
{
	class BossScene : public StageScene
	{
	public:
		BossScene();
		virtual ~BossScene();

	private:
		GameObject* m_Boss;
		GameObject* m_DramaFX;
		bool		m_bBossWakeUp;
		float		m_Time;

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void Destroy() override;

	public:
		virtual void OnEnter() override;
		virtual void OnExit() override;

	public:
		void BossWakeUp();
	};
}