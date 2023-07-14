#pragma once
#include "..\\Engine_SOURCE\\Lu_GameObject.h"


namespace gui
{
	class EditorObject : public Lu::GameObject
	{
	public:
		EditorObject();
		virtual ~EditorObject();

	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
	};
}