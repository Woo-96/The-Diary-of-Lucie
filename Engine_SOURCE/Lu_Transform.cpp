#include "Lu_Transform.h"
#include "Lu_Renderer.h"
#include "Lu_ConstantBuffer.h"
#include "Lu_Camera.h"

namespace Lu
{
	using namespace Lu::graphics;

	Transform::Transform()
		: Component(eComponentType::Transform)
		, m_Position(Vector3::Zero)
		, m_Rotation(Vector3::Zero)
		, m_Scale(Vector3::One)
		, m_Parent(nullptr)
	{
		SetName(L"Transform");
	}

	Transform::~Transform()
	{
	}

	void Transform::Initialize()
	{
	}

	void Transform::Update()
	{
	}

	void Transform::LateUpdate()
	{
		m_World = Matrix::Identity;

		Matrix scale = Matrix::CreateScale(m_Scale);

		Matrix rotation;
		rotation = Matrix::CreateRotationX(m_Rotation.x);
		rotation *= Matrix::CreateRotationY(m_Rotation.y);
		rotation *= Matrix::CreateRotationZ(m_Rotation.z);

		Matrix position;
		position.Translation(m_Position);

		m_World = scale * rotation * position;

		m_Up = Vector3::TransformNormal(Vector3::Up, rotation);
		m_Foward = Vector3::TransformNormal(Vector3::Forward, rotation);
		m_Right = Vector3::TransformNormal(Vector3::Right, rotation);

		if (m_Parent)
		{
			m_World *= m_Parent->m_World;
		}
	}

	void Transform::Render()
	{

	}

	void Transform::BindConstantBuffer()
	{
		renderer::TransformCB trCB = {};
		trCB.mWorld = m_World;
		trCB.mView = Camera::GetGpuViewMatrix();
		trCB.mProjection = Camera::GetGpuProjectionMatrix();

		ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Transform];
		cb->SetData(&trCB);
		cb->Bind(eShaderStage::VS);
	}
}