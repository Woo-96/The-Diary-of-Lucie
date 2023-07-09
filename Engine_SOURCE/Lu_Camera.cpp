#include "Lu_Camera.h"
#include "Lu_Transform.h"
#include "Lu_GameObject.h"
#include "Lu_Application.h"

extern Lu::Application application;

namespace Lu
{
	Matrix Camera::m_View = Matrix::Identity;
	Matrix Camera::m_Projection = Matrix::Identity;

	Camera::Camera()
		: Component(eComponentType::Camera)
		, m_Type(eProjectionType::OrthoGraphic)
		, m_AspectRatio(1.0f)
		, m_Near(1.0f)
		, m_Far(1000.0f)
		, m_Size(5.0f)
	{
	}

	Camera::~Camera()
	{
	}

	void Camera::Initialize()
	{
	}

	void Camera::Update()
	{
	}

	void Camera::LateUpdate()
	{
		CreateViewMatrix();
		CreateProjectionMatrix(m_Type);
	}

	void Camera::Render()
	{
	}

	bool Camera::CreateViewMatrix()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();

		// View Translate Matrix
		m_View = Matrix::Identity;
		m_View *= Matrix::CreateTranslation(-pos);

		// View Rotation Matrix
		Vector3 up = tr->Up();
		Vector3 right = tr->Right();
		Vector3 foward = tr->Foward();

		Matrix viewRotate;
		viewRotate._11 = right.x;	viewRotate._12 = up.x;	viewRotate._13 = foward.x;
		viewRotate._21 = right.y;	viewRotate._22 = up.y;	viewRotate._23 = foward.y;
		viewRotate._31 = right.z;	viewRotate._32 = up.z;	viewRotate._33 = foward.z;
		m_View *= viewRotate;

		return true;
	}

	bool Camera::CreateProjectionMatrix(eProjectionType type)
	{
		RECT rect = {};
		GetClientRect(application.GetHwnd(), &rect);
		float width = rect.right - rect.left;
		float height = rect.bottom - rect.top;
		m_AspectRatio = width / height;;

		if (type == eProjectionType::OrthoGraphic)
		{
			float OrthorGraphicRatio = m_Size / 1000.f;
			width *= OrthorGraphicRatio;
			height *= OrthorGraphicRatio;

			m_Projection = Matrix::CreateOrthographicLH(width, height, m_Near, m_Far);
		}
		else
		{
			m_Projection = Matrix::CreatePerspectiveFieldOfViewLH(XM_2PI / 6.0f, m_AspectRatio, m_Near, m_Far);
		}


		return true;
	}

}
