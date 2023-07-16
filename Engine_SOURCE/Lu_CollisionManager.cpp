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
		// �� �浹ü�� ID bool���� Ȯ��
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
			// �浹
			if (iter->second == false)
			{
				//���� �浹
				_Left->OnCollisionEnter(_Right);
				_Right->OnCollisionEnter(_Left);
				iter->second = true;
			}
			else
			{
				// �浹 ��
				_Left->OnCollisionStay(_Right);
				_Right->OnCollisionStay(_Left);
			}
		}
		else
		{
			// �浹 X
			if (iter->second == true)
			{
				// �浹�ϰ� �ִٰ� ������
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
			/* �и��� �̷�
				1. ���� ��(ǥ�� ���� ����) ���ϱ�
				2. �� �浹ü�� ������ �ش��ϴ� �߽��� ���ϱ�
				3. ǥ�� ���� ���� 4���� �߽� ���� 1���� �и��� �׽�Ʈ ����
					- ǥ�� ���� ���� 4���� ���ư��� ���� ���� ��� (������ �Ǵ� ����)
					- ���� �� ���⿡ ǥ�� ���� ���͸� �����Ͽ� ����(����) ���ϱ�
					- �߽� ���͵� ���� ��� �����Ͽ� ���� ���ϱ�
					- ���� ���� ���̿� �߽� ���� ���� �� (�߽� ���̰� �� ��� false)
				4. 4���� �׽�Ʈ ��, �ѹ��̶� ���� �и��Ǿ� ������ �浹���� ����
			*/

			// ������ 4��
			Vector3 proj[4] = {};

			// ���� ����
			Vector3 center;

			// �и��� �׽�Ʈ 4ȸ
			for (int i = 0; i < 4; ++i)
			{
				float projDist = 0.f;

				float centerDist = 0.f;

				if (projDist < centerDist)
					return false;
			}

			// �׽�Ʈ 4ȸ�� ��� ����� ��� �浹
			return true;
		}

		// Circle to Circle
		if (eColliderType::Circle == _Left->GetType() && eColliderType::Circle == _Right->GetType())
		{
			// �߽��� ������ �Ÿ�
			// CollisionMgr�� Collision2D���� ���� ȣ��Ǳ� ������ �״�� �������� ������ ���� ó�� 1�������� ������ �浹��
			//Vector3 centerLeft = _Left->GetPosition();
			//Vector3 centerRight = _Right->GetPosition();

			//float distance = Vector3::Distance(centerLeft, centerRight);

			Transform* LeftTransform = _Left->GetOwner()->GetComponent<Transform>();

			Vector3 scale = LeftTransform->GetScale();
			scale.x *= _Left->GetSize().x;
			scale.y *= _Left->GetSize().y;

			Vector3 centerLeft = LeftTransform->GetPosition();
			centerLeft.x += _Left->GetCenter().x;
			centerLeft.y += _Left->GetCenter().y;

			Transform* RightTransform = _Right->GetOwner()->GetComponent<Transform>();

			scale = RightTransform->GetScale();
			scale.x *= _Right->GetSize().x;
			scale.y *= _Right->GetSize().y;

			Vector3 centerRight = RightTransform->GetPosition();
			centerRight.x += _Right->GetCenter().x;
			centerRight.y += _Right->GetCenter().y;

			float distance = Vector3::Distance(centerLeft, centerRight);

			// ������
			float radiusLeft = (LeftTransform->GetScale().x * _Left->GetSize().x) / 2.f;
			float radiusRight = (RightTransform->GetScale().x * _Right->GetSize().x) / 2.f;

			// �߽� ������ �Ÿ����� �������� ���� �� ũ�� �浹
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
