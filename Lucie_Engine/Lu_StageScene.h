#pragma once
#include "..\Engine_SOURCE\Lu_Scene.h"

namespace Lu
{
	class StageScene : public Scene
	{
	public:
		StageScene();
		virtual ~StageScene();

	private:
		static GameObject* m_GoldTex;

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void Destroy() override;

	public:
		virtual void OnEnter() override;
		virtual void OnExit() override;

	protected:
		void CreateUI();
		bool IsInBattle();
		void HideHUD();
		void ShowHUD();
	};
}