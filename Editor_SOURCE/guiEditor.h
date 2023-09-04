#pragma once
#include "..\\Engine_SOURCE\\CommonInclude.h"
#include "..\\Engine_SOURCE\\Lucie_Engine.h"
#include "..\\Engine_SOURCE\\Lu_Graphics.h"
#include "guiWidget.h"
#include "guiEditorObject.h"
#include "guiDebugOjbect.h"

namespace gui
{
	class Editor
	{
	private:
		static std::vector<Widget*>			m_Widgets;
		static std::vector<EditorObject*>	m_EditorObjects;
		static std::vector<DebugOjbect*>	m_DebugOjbects;

		static bool							m_bDebugRender;

	public:
		static void Initialize();
		static void Run();

		static void Update();
		static void LateUpdate();
		static void Render();
		static void Release();

	public:
		static void DebugRender(const Lu::graphics::DebugMesh& _Mesh);
	};
}
