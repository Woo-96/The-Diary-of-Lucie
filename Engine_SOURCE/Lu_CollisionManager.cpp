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
				if (m_Matrix[row] == true)
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
			}
		}
	}

	bool CollisionManager::Intersect(Collider2D* _Left, Collider2D* _Right)
	{
		// Rect to Rect
		if (eColliderType::Rect == _Left->GetType()
			&& eColliderType::Rect == _Right->GetType())
		{
			/* 분리축 이론
				1. 투영 축(표면 방향 벡터) 구하기
				2. 각 충돌체의 원점에 해당하는 중심점 구하기
				3. 표면 방향 벡터 4개와 중심 벡터 1개로 분리축 테스트 수행
					- 표면 방향 벡터 4개가 돌아가며 투영 축을 담당 (기준이 되는 방향)
					- 투영 축 방향에 표면 방향 벡터를 투영하여 길이(면적) 구하기
					- 중심 벡터도 투영 축과 내적하여 길이 구하기
					- 최종 투영 길이와 중심 벡터 길이 비교 (중심 길이가 더 길면 false)
				4. 4번의 테스트 중, 한번이라도 축이 분리되어 있으면 충돌하지 않음
			*/

			// 투영축 4개
			Vector3 proj[4] = {};

			// 센터 벡터
			Vector3 center;

			// 분리축 테스트 4회
			for (int i = 0; i < 4; ++i)
			{
				float projDist = 0.f;

				float centerDist = 0.f;

				if (projDist < centerDist)
					return false;
			}

			// 테스트 4회를 모두 통과할 경우 충돌
			return true;
		}

		// Circle to Circle
		if (eColliderType::Circle == _Left->GetType() && eColliderType::Circle == _Right->GetType())
		{
			// 중심점 사이의 거리
			Vector3 centerLeft = _Left->GetPosition();
			Vector3 centerRight = _Right->GetPosition();

			float distance = Vector3::Distance(centerLeft, centerRight);

			// 반지름
			float radiusLeft = (_Left->GetTransform()->GetScale().x * _Left->GetSize().x) / 2.f;
			float radiusRight = (_Right->GetTransform()->GetScale().x * _Right->GetSize().x) / 2.f;

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
