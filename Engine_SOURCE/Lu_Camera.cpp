#include "Lu_Camera.h"
#include "Lu_Transform.h"
#include "Lu_GameObject.h"
#include "Lu_Application.h"
#include "Lu_Renderer.h"
#include "Lu_SceneManager.h"
#include "Lu_Scene.h"
#include "Lu_MeshRenderer.h"

extern Lu::Application application;

namespace Lu
{
	bool CompareZSort(GameObject* a, GameObject* b)
	{
		if (a->GetComponent<Transform>()->GetPosition().z
			<= b->GetComponent<Transform>()->GetPosition().z)
			return false;

		return true;
	}

	Matrix Camera::View = Matrix::Identity;
	Matrix Camera::Projection = Matrix::Identity;

	Camera::Camera()
		: Component(eComponentType::Camera)
		, m_View(Matrix::Identity)
		, m_Projection(Matrix::Identity)
		, m_Type(eProjectionType::OrthoGraphic)
		, m_AspectRatio(1.0f)
		, m_Near(1.0f)
		, m_Far(1000.0f)
		, m_Scale(1.f)
		, m_LayerMask{}
	{
		SetName(L"Camera");
		EnableLayerMasks();
	}

	Camera::~Camera()
	{
	}

	void Camera::LateUpdate()
	{
		CreateViewMatrix();
		CreateProjectionMatrix(m_Type);
		RegisterCameraInRenderer();
	}

	void Camera::Render()
	{
		View = m_View;
		Projection = m_Projection;

		AlphaSortGameObjects();
		ZSortTransparencyGameObjects();
		RenderOpaque();

		DisableDepthStencilState();
		RenderCutOut();
		RenderTransparent();
		EnableDepthStencilState();
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
		float width = (float)rect.right - (float)rect.left;
		float height = (float)rect.bottom - (float)rect.top;
		m_AspectRatio = width / height;

		if (type == eProjectionType::OrthoGraphic)
		{
			//float OrthorGraphicRatio = m_Scale / 1000.f;
			//width *= OrthorGraphicRatio;
			//height *= OrthorGraphicRatio;
			//m_Projection = Matrix::CreateOrthographicLH(width, height, m_Near, m_Far);


			//m_Projection = XMMatrixOrthographicLH(width * (1.f / m_Scale), height * (1.f / m_Scale), m_Near, m_Far);

			m_Projection = Matrix::CreateOrthographicLH(width * (1.f / m_Scale), height * (1.f / m_Scale), m_Near, m_Far);
		}
		else
		{
			m_Projection = Matrix::CreatePerspectiveFieldOfViewLH(XM_2PI / 6.0f, m_AspectRatio, m_Near, m_Far);
		}


		return true;
	}

	void Camera::RegisterCameraInRenderer()
	{
		renderer::cameras.push_back(this);
	}

	void Camera::TurnLayerMask(eLayerType _Type, bool _Enable)
	{
		m_LayerMask.set((UINT)_Type, _Enable);
	}

	void Camera::AlphaSortGameObjects()
	{
		m_OpaqueGameObjects.clear();
		m_CutOutGameObjects.clear();
		m_TransparentGameObjects.clear();

		Scene* scene = SceneManager::GetActiveScene();
		for (size_t i = 0; i < (UINT)eLayerType::End; i++)
		{
			if (m_LayerMask[i] == true)
			{
				Layer& layer = scene->GetLayer((eLayerType)i);
				const std::vector<GameObject*> gameObjs
					= layer.GetGameObjects();

				DivideAlphaBlendGameObjects(gameObjs);
			}
		}
	}

	void Camera::ZSortTransparencyGameObjects()
	{
		std::sort(m_CutOutGameObjects.begin()
			, m_CutOutGameObjects.end()
			, CompareZSort);
		std::sort(m_TransparentGameObjects.begin()
			, m_TransparentGameObjects.end()
			, CompareZSort);
	}

	void Camera::DivideAlphaBlendGameObjects(const std::vector<GameObject*> _GameObjs)
	{
		for (GameObject* obj : _GameObjs)
		{
			MeshRenderer* mr
				= obj->GetComponent<MeshRenderer>();
			if (mr == nullptr)
				continue;

			std::shared_ptr<Material> mt = mr->GetMaterial();
			eRenderingMode mode = mt->GetRenderingMode();
			switch (mode)
			{
			case Lu::graphics::eRenderingMode::Opaque:
				m_OpaqueGameObjects.push_back(obj);
				break;
			case Lu::graphics::eRenderingMode::CutOut:
				m_CutOutGameObjects.push_back(obj);
				break;
			case Lu::graphics::eRenderingMode::Transparent:
				m_TransparentGameObjects.push_back(obj);
				break;
			default:
				break;
			}
		}
	}

	void Camera::RenderOpaque()
	{
		for (GameObject* gameObj : m_OpaqueGameObjects)
		{
			if (gameObj == nullptr)
				continue;
			if (gameObj->GetState()
				!= GameObject::eState::Active)
				continue;


			gameObj->Render();
		}
	}

	void Camera::RenderCutOut()
	{
		for (GameObject* gameObj : m_CutOutGameObjects)
		{
			if (gameObj == nullptr)
				continue;
			if (gameObj->GetState()
				!= GameObject::eState::Active)
				continue;

			gameObj->Render();
		}
	}

	void Camera::RenderTransparent()
	{
		for (GameObject* gameObj : m_TransparentGameObjects)
		{
			if (gameObj == nullptr)
				continue;
			if (gameObj->GetState()
				!= GameObject::eState::Active)
				continue;

			gameObj->Render();
		}
	}

	void Camera::EnableDepthStencilState()
	{
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dsState
			= renderer::depthStencilStates[(UINT)eDSType::Less];
		GetDevice()->BindDepthStencilState(dsState.Get());
	}

	void Camera::DisableDepthStencilState()
	{
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dsState
			= renderer::depthStencilStates[(UINT)eDSType::None];
		GetDevice()->BindDepthStencilState(dsState.Get());
	}

	Vector3 Camera::ScreenToWorld(Vector2 _MousePos)
	{
		Viewport viewport;
		viewport.width = (float)application.GetWidth();
		viewport.height = (float)application.GetHeight();
		viewport.x = 0;
		viewport.y = 0;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		
		return viewport.Unproject(Vector3(_MousePos.x, _MousePos.y, 0.f), View, Projection, Matrix::Identity);
	}
}
