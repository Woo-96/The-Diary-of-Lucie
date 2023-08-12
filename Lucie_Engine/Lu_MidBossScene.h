#pragma once
#include "Lu_StageScene.h"

namespace Lu
{
	class GameObject;
	class MidBossScene : public StageScene
	{
	public:
		MidBossScene();
		virtual ~MidBossScene();

	private:
		GameObject* m_BossName;
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
	};
}