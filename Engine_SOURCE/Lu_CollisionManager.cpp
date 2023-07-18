#include "Lu_CollisionManager.h"
#include "Lu_GameObject.h"
#include "Lu_Scene.h"
#include "Lu_SceneManager.h"
#include "Lu_Layer.h"
#include "Lu_Collider2D.h"


namespace Lu
{
	std::bitset<LAYER_MAX> CollisionManager::m_Matrix[LAYER_MAX] = {};
	std::map<UINT64, bool> CollisionManager::m_CollisionMap = {};

	void CollisionManager::Initialize()
	{
	}

	void CollisionManager::Update()
	{
		for (UINT column = 0; column < (UINT)eLayerType::End; column++)
		{
			for (UINT row = 0; row < (UINT)eLayerType::End; row++)
			{
				if (m_Matrix[column][row] == true)
				{
					LayerCollision((eLayerType)column, (eLayerType)row);
				}
			}
		}
	}

	void CollisionManager::LayerCollision(eLayerType _Left, eLayerType _Right)
	{
		Scene* activeScene = SceneManager::GetActiveScene();

		const std::vector<GameObject*>& lefts = activeScene->GetLayer(_Left).GetGameObjects();
		const std::vector<GameObject*>& rights = activeScene->GetLayer(_Right).GetGameObjects();

		for (GameObject* leftObj : lefts)
		{
			Collider2D* leftCol = leftObj->GetComponent<Collider2D>();

			if (leftCol == nullptr
				|| leftObj->GetState() != GameObject::eState::Active)
				continue;

			for (GameObject* rightObj : rights)
			{
				Collider2D* rightCol = rightObj->GetComponent<Collider2D>();

				if (rightCol == nullptr
					|| leftObj == rightObj
					|| rightObj->GetState() != GameObject::eState::Active)
					continue;

				ColliderCollision(leftCol, rightCol);
			}
		}
	}

	void CollisionManager::ColliderCollision(Collider2D* _Left, Collider2D* _Right)
	{
		// 두 충돌체의 ID bool값을 확인
		ColliderID id = {};
		id.Left = _Left->GetColliderID();
		id.Right = _Right->GetColliderID();

		std::map<UINT64, bool>::iterator iter = m_CollisionMap.find(id.id);
	
		if (iter == m_CollisionMap.end())
		{
			m_CollisionMap.insert(std::make_pair(id.id, false));
			iter = m_CollisionMap.find(id.id);
		}

		if (Intersect(_Left, _Right))
		{
			// 충돌
			if (iter->second == false)
			{
				//최초 충돌
				_Left->OnCollisionEnter(_Right);
				_Right->OnCollisionEnter(_Left);
				iter->second = true;
			}
			else
			{
				// 충돌 중
				_Left->OnCollisionStay(_Right);
				_Right->OnCollisionStay(_Left);
			}
		}
		else
		{
			// 충돌 X
			if (iter->second == true)
			{
				// 충돌하고 있다가 나갈떄
				_Left->OnCollisionExit(_Right);
				_Right->OnCollisionExit(_Left);
				iter->second = false;
			}
		}
	}

	bool CollisionManager::Intersect(Collider2D* _Left, Collider2D* _Right)
	{
		// Rect to Rect
		if (eColliderType::Rect == _Left->GetType()
			&& eColliderType::Rect == _Right->GetType())
		{
			// Rect vs Rect 
			// 0 --- 1
			// |     |
			// 3 --- 2
			Vector3 arrLocalPos[4] =
			{
			   Vector3{-0.5f, 0.5f, 0.0f}
			   ,Vector3{0.5f, 0.5f, 0.0f}
			   ,Vector3{0.5f, -0.5f, 0.0f}
			   ,Vector3{-0.5f, -0.5f, 0.0f}
			};

			Transform* leftTr = _Left->GetOwner()->GetComponent<Transform>();
			Transform* rightTr = _Right->GetOwner()->GetComponent<Transform>();

			Matrix leftMatrix = leftTr->GetWorldMatrix();
			Matrix rightMatrix = rightTr->GetWorldMatrix();

			Vector3 Axis[4] = {};

			Vector3 leftScale = Vector3(_Left->GetSize().x, _Left->GetSize().y, 1.0f);
			Matrix finalLeft = Matrix::CreateScale(leftScale);
			finalLeft *= leftMatrix;

			Vector3 rightScale = Vector3(_Right->GetSize().x, _Right->GetSize().y, 1.0f);
			Matrix finalRight = Matrix::CreateScale(rightScale);
			finalRight *= rightMatrix;

			Axis[0] = Vector3::Transform(arrLocalPos[1], finalLeft);
			Axis[1] = Vector3::Transform(arrLocalPos[3], finalLeft);
			Axis[2] = Vector3::Transform(arrLocalPos[1], finalRight);
			Axis[3] = Vector3::Transform(arrLocalPos[3], finalRight);

			Axis[0] -= Vector3::Transform(arrLocalPos[0], finalLeft);
			Axis[1] -= Vector3::Transform(arrLocalPos[0], finalLeft);
			Axis[2] -= Vector3::Transform(arrLocalPos[0], finalRight);
			Axis[3] -= Vector3::Transform(arrLocalPos[0], finalRight);

			for (size_t i = 0; i < 4; i++)
				Axis[i].z = 0.0f;

			Vector3 vc = leftTr->GetPosition() - rightTr->GetPosition();
			vc.z = 0.0f;

			Vector3 centerDir = vc;
			for (size_t i = 0; i < 4; i++)
			{
				Vector3 vA = Axis[i];

				float projDistance = 0.0f;
				for (size_t j = 0; j < 4; j++)
				{
					projDistance += fabsf(Axis[j].Dot(vA) / 2.0f);
				}

				if (projDistance < fabsf(centerDir.Dot(vA)))
					return false;
			}

			return true;
		}

		// Circle to Circle
		if (eColliderType::Circle == _Left->GetType() && eColliderType::Circle == _Right->GetType())
		{
			// 중심점 사이의 거리
			Vector3 centerLeft = _Left->GetPosition();
			Vector3 centerRight = _Right->GetPosition();

			float distance = Vector3::Distance(centerLeft, centerRight);

			Transform* LeftTransform = _Left->GetOwner()->GetComponent<Transform>();

			//Vector3 scale = LeftTransform->GetScale();
			//scale.x *= _Left->GetSize().x;
			//scale.y *= _Left->GetSize().y;

			//Vector3 centerLeft = LeftTransform->GetPosition();
			//centerLeft.x += _Left->GetCenter().x;
			//centerLeft.y += _Left->GetCenter().y;

			Transform* RightTransform = _Right->GetOwner()->GetComponent<Transform>();

			//scale = RightTransform->GetScale();
			//scale.x *= _Right->GetSize().x;
			//scale.y *= _Right->GetSize().y;

			//Vector3 centerRight = RightTransform->GetPosition();
			//centerRight.x += _Right->GetCenter().x;
			//centerRight.y += _Right->GetCenter().y;

			//float distance = Vector3::Distance(centerLeft, centerRight);

			// 반지름
			float radiusLeft = (LeftTransform->GetScale().x * _Left->GetSize().x) / 2.f;
			float radiusRight = (RightTransform->GetScale().x * _Right->GetSize().x) / 2.f;

			// 중심 사이의 거리보다 반지름의 합이 더 크면 충돌
			if (distance <= radiusLeft + radiusRight)
				return true;
		}

		return false;
	}

	void CollisionManager::SetLayer(eLayerType _Left, eLayerType _Right, bool _bEnable)
	{
		UINT row = -1;
		UINT col = -1;

		UINT iLeft = (UINT)_Left;
		UINT iRight = (UINT)_Right;

		if (iLeft <= iRight)
		{
			row = iLeft;
			col = iRight;
		}
		else
		{
			row = iRight;
			col = iLeft;
		}

		m_Matrix[col][row] = _bEnable;
	}

	void CollisionManager::Clear()
	{
		m_Matrix->reset();
		m_CollisionMap.clear();
	}
}
