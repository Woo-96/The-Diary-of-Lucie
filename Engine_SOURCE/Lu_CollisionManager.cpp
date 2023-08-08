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

		// 둘 중 하나라도 삭제 예정 상태라면(Dead 상태)
		bool bDead = false;
		if (_Left->GetOwner()->IsDead() || _Right->GetOwner()->IsDead())
		{
			bDead = true;
		}

		// 두 충돌체가 지금 충돌 중인지 확인
		if (Intersect(_Left, _Right))
		{
			// 이전에 충돌한 적이 있고, 둘중 하나 이상이 삭제 예정이라면
			if (bDead && iter->second)
			{
				_Left->OnCollisionExit(_Right);
				_Right->OnCollisionExit(_Left);
			}
			else if (iter->second)
			{
				// 계속 충돌 중
				_Left->OnCollisionStay(_Right);
				_Right->OnCollisionStay(_Left);
			}
			else
			{
				// 이번 프레임에 충돌
				if (!bDead) // 둘중 하나라도 Dead 상태면 충돌을 무시한다.
				{
					_Left->OnCollisionEnter(_Right);
					_Right->OnCollisionEnter(_Left);
					iter->second = true;
				}
			}
		}

		else
		{
			// 충돌 해제
			if (iter->second)
			{
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
			// 0 --- 1
			// |  \  | 
			// 3 --- 2   
			static const Vector3 arrLocalPos[4]
				=
			{
			   Vector3(-0.5f, 0.5f, 0.f),
			   Vector3(0.5f, 0.5f, 0.f),
			   Vector3(0.5f, -0.5f, 0.f),
			   Vector3(-0.5f, -0.5f, 0.f)
			};


			// 분리축 구하기
			Vector3 vAxis[4] = {};

			const Matrix& matLeft = _Left->GetFinalTransform();
			const Matrix& matRight = _Right->GetFinalTransform();

			// 분리축 벡터 == 투영벡터
			vAxis[0] = XMVectorSubtract(XMVector3TransformCoord(arrLocalPos[1], matLeft), XMVector3TransformCoord(arrLocalPos[0], matLeft));
			vAxis[1] = XMVectorSubtract(XMVector3TransformCoord(arrLocalPos[3], matLeft), XMVector3TransformCoord(arrLocalPos[0], matLeft));
			vAxis[2] = XMVectorSubtract(XMVector3TransformCoord(arrLocalPos[1], matRight), XMVector3TransformCoord(arrLocalPos[0], matRight));
			vAxis[3] = XMVectorSubtract(XMVector3TransformCoord(arrLocalPos[3], matRight), XMVector3TransformCoord(arrLocalPos[0], matRight));

			for (int i = 0; i < 4; ++i)
				vAxis[i].z = 0.f;


			Vector3 vC = _Left->GetPosition() - _Right->GetPosition();
			Vector3 vCenterDir = Vector3(vC.x, vC.y, 0.f);


			for (int i = 0; i < 4; ++i)
			{
				Vector3 vA = vAxis[i];
				vA.Normalize();

				float fProjDist = 0.f;
				for (int j = 0; j < 4; ++j)
				{
					fProjDist += fabsf(vAxis[j].Dot(vA)) / 2.f;
				}

				if (fProjDist < fabsf(vCenterDir.Dot(vA)))
				{
					return false;
				}
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
			Transform* RightTransform = _Right->GetOwner()->GetComponent<Transform>();

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
