#pragma once
//#include <Lu_Scene.h>
#include "..\Engine_SOURCE\Lu_Scene.h"

namespace Lu
{
	class PlayScene : public Scene
	{
	public:
		PlayScene();
		virtual ~PlayScene();

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
	};
}