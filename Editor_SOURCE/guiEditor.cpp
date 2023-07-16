#include "guiEditor.h"
#include "..\\Engine_SOURCE\\Lu_Mesh.h"
#include "..\\Engine_SOURCE\\Lu_Resources.h"
#include "..\\Engine_SOURCE\\Lu_Transform.h"
#include "..\\Engine_SOURCE\\Lu_MeshRenderer.h"
#include "..\\Engine_SOURCE\\Lu_Material.h"
#include "..\\Engine_SOURCE\\Lu_Renderer.h"
#include "..\\Lucie_Engine\\Lu_GridScript.h"
#include "Lu_GridScript.h"

namespace gui
{
	using namespace Lu::enums;
	std::vector<Widget*> Editor::m_Widgets = {};
	std::vector<EditorObject*> Editor::m_EditorObjects = {};
	std::vector<DebugOjbect*> Editor::m_DebugOjbects = {};

	void Editor::Initialize()
	{
		// ================
		// == Debug Mesh ==
		// ================
		m_DebugOjbects.resize((UINT)eColliderType::End);

		// RectMesh
		std::shared_ptr<Lu::Mesh> mesh = Lu::Resources::Find<Lu::Mesh>(L"DebugRect");
		std::shared_ptr<Lu::Material> material = Lu::Resources::Find<Lu::Material>(L"DebugMaterial");

		m_DebugOjbects[(UINT)eColliderType::Rect] = new DebugOjbect();
		m_DebugOjbects[(UINT)eColliderType::Rect]->AddComponent<Lu::Transform>();
		Lu::MeshRenderer* mr = m_DebugOjbects[(UINT)eColliderType::Rect]->AddComponent<Lu::MeshRenderer>();
		mr->SetMaterial(material);
		mr->SetMesh(mesh);

		// CircleMesh
		mesh = Lu::Resources::Find<Lu::Mesh>(L"DebugCircle");

		m_DebugOjbects[(UINT)eColliderType::Circle] = new DebugOjbect();
		m_DebugOjbects[(UINT)eColliderType::Circle]->AddComponent<Lu::Transform>();
		mr = m_DebugOjbects[(UINT)eColliderType::Circle]->AddComponent<Lu::MeshRenderer>();
		mr->SetMaterial(material);
		mr->SetMesh(mesh);


		// Grid
		//EditorObject* grid = new EditorObject();
		//grid->SetName(L"Grid");

		//mr = grid->AddComponent<Lu::MeshRenderer>();
		//mr->SetMesh(Lu::Resources::Find<Lu::Mesh>(L"RectMesh"));
		//mr->SetMaterial(Lu::Resources::Find<Lu::Material>(L"GridMaterial"));
		//Lu::GridScript* gridSc = grid->AddComponent<Lu::GridScript>();
		//gridSc->SetCamera(renderer::cameras[0]);

		//m_EditorObjects.push_back(grid);

	}
	void Editor::Run()
	{
		Update();
		LateUpdate();
		Render();
	}
	void Editor::Update()
	{
		for (EditorObject* obj : m_EditorObjects)
		{
			obj->Update();
		}
	}
	void Editor::LateUpdate()
	{
		for (EditorObject* obj : m_EditorObjects)
		{
			obj->LateUpdate();
		}
	}
	void Editor::Render()
	{
		for (EditorObject* obj : m_EditorObjects)
		{
			obj->Render();
		}

		for (const Lu::graphics::DebugMesh& mesh
			: renderer::debugMeshs)
		{
			DebugRender(mesh);
		}

		renderer::debugMeshs.clear();
	}
	void Editor::Release()
	{
		for (auto widget : m_Widgets)
		{
			delete widget;
			widget = nullptr;
		}

		for (auto editorObj : m_EditorObjects)
		{
			delete editorObj;
			editorObj = nullptr;
		}

		for (auto debugObj : m_DebugOjbects)
		{
			delete debugObj;
			debugObj = nullptr;
		}
	}

	void Editor::DebugRender(const Lu::graphics::DebugMesh& _Mesh)
	{
		DebugOjbect* debugObj = m_DebugOjbects[(UINT)_Mesh.Type];

		// 위치 크기 회전 정보를 받아와서
		// 해당 게임오브젝트위에 그려주면된다.
		Lu::Transform* tr = debugObj->GetComponent<Lu::Transform>();

		Vector3 pos = _Mesh.Position;
		pos.z -= 0.01f;

		tr->SetPosition(pos);
		tr->SetScale(_Mesh.Scale);
		tr->SetRotation(_Mesh.Rotation);

		tr->LateUpdate();

		/*ya::MeshRenderer * mr
			= debugObj->GetComponent<ya::MeshRenderer>();*/
			// main camera
		Lu::Camera* mainCamara = renderer::mainCamera;

		Lu::Camera::SetGpuViewMatrix(mainCamara->GetViewMatrix());
		Lu::Camera::SetGpuProjectionMatrix(mainCamara->GetProjectionMatrix());

		debugObj->Render();
	}
}
