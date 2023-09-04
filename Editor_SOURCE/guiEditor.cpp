#include "guiEditor.h"
#include "..\\Engine_SOURCE\\Lu_Mesh.h"
#include "..\\Engine_SOURCE\\Lu_Resources.h"
#include "..\\Engine_SOURCE\\Lu_Transform.h"
#include "..\\Engine_SOURCE\\Lu_MeshRenderer.h"
#include "..\\Engine_SOURCE\\Lu_Material.h"
#include "..\\Engine_SOURCE\\Lu_Renderer.h"
#include "..\\Engine_SOURCE\\Lu_Enums.h"
#include "..\\Engine_SOURCE\\Lu_Input.h"

namespace gui
{
	using namespace Lu::enums;
	std::vector<Widget*> Editor::m_Widgets = {};
	std::vector<EditorObject*> Editor::m_EditorObjects = {};
	std::vector<DebugOjbect*> Editor::m_DebugOjbects = {};
	bool Editor::m_bDebugRender = true;

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

		if (Lu::Input::GetKeyDown(Lu::eKeyCode::P))
		{
			if (m_bDebugRender)
				m_bDebugRender = false;
			else
				m_bDebugRender = true;
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
		if (!m_bDebugRender)
			return;

		DebugOjbect* debugObj = m_DebugOjbects[(UINT)_Mesh.Type];
		Lu::Transform* tr = debugObj->GetComponent<Lu::Transform>();

		Vector3 pos = _Mesh.Position;
		pos.z -= 0.01f;
		tr->SetPosition(pos);
		tr->SetRotation(_Mesh.Rotation);

		if (_Mesh.Type == Lu::enums::eColliderType::Rect)
		{
			tr->SetScale(_Mesh.Scale);
		}
		else
		{
			float radius = _Mesh.Radius;
			Vector3 scale(radius * 2, radius * 2, 1.0f); // 원의 지름을 스케일로 설정
			tr->SetScale(scale);
		}

		// 트랜스폼 업데이트
		tr->LateUpdate();

		// 색상 결정
		Lu::MeshRenderer* mr = debugObj->GetComponent<Lu::MeshRenderer>();
		mr->GetMaterial()->SetScalarParam(VEC4_0, &_Mesh.Color);

		Lu::Camera* mainCamara = renderer::mainCamera;

		Lu::Camera::SetGpuViewMatrix(mainCamara->GetViewMatrix());
		Lu::Camera::SetGpuProjectionMatrix(mainCamara->GetProjectionMatrix());

		debugObj->Render();
	}
}
