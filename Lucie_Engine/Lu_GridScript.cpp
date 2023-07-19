#include "Lu_GridScript.h"
#include "Lu_ConstantBuffer.h"
#include "Lu_Camera.h"
#include "Lu_Transform.h"
#include "Lu_Application.h"
#include "Lu_GameObject.h"
#include "Lu_Renderer.h"
#include "Lu_Time.h"
#include "Lu_Object.h"

extern Lu::Application application;

namespace Lu
{
	GridScript::GridScript()
		: m_Camera(nullptr)
	{

	}

	GridScript::~GridScript()
	{

	}

	void GridScript::Initialize()
	{

	}

	void GridScript::Update()
	{
		//static float chTime = 0.0f;
		//chTime += Time::DeltaTime();
		//
		//if (chTime > 3.0f)
		//{
		//	object::Destroy(GetOwner());
		//}

		//if (m_Camera == nullptr)
		//	return;


		GameObject* gameObj = m_Camera->GetOwner();


		Transform* tr = gameObj->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();

		float scale = m_Camera->GetScale();

		HWND hWnd = application.GetHwnd();
		RECT winRect = {};
		GetClientRect(hWnd, &winRect);

		float width = (float)winRect.right - (float)winRect.left;
		float height = (float)winRect.bottom - (float)winRect.top;
		Vector2 resolution(width, height);

		///cs buffer
		graphics::ConstantBuffer* cb
			= renderer::constantBuffer[(int)eCBType::Grid];

		renderer::GridCB data;
		data.CameraPosition = Vector4(pos.x, pos.y, pos.z, 1.0f);
		data.Resolution = resolution;
		data.CameraScale = Vector2(scale, scale);

		cb->SetData(&data);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::PS);
	}

	void GridScript::LateUpdate()
	{

	}

	void GridScript::Render()
	{

	}
}