#include "Lu_HeartScript.h"
#include "Lu_GameObject.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_Transform.h"
#include "Lu_PlayerScript.h"

namespace Lu
{
	HeartScript::HeartScript()
		: m_arrHeart{}
		, m_arrHeartPos{}
		, m_MaxCount(0)
	{
		SetName(L"HeartScript");

		m_arrMaterial[(int)eHeartColor::Empty] = Resources::Find<Material>(L"EmptyHeart_Icon_Mtrl");
		m_arrMaterial[(int)eHeartColor::Full] = Resources::Find<Material>(L"FullHeart_Icon_Mtrl");
	}

	HeartScript::~HeartScript()
	{

	}

	void HeartScript::Initialize()
	{
		for (int i = 0; i < MaxHeart * 2; ++i)
		{
			// 0 ~ 4�� : Empty ��Ʈ
			if (i < MaxHeart)
			{
				m_arrHeartPos[i] = Vector2(-80.f - (i * 36) - (i * 5.f), -280.f);
				m_arrHeart[i] = object::Instantiate<GameObject>(Vector3(m_arrHeartPos[i].x, m_arrHeartPos[i].y, 200.f), Vector3(36.f, 33.f, 100.f), eLayerType::UI);
				m_arrHeart[i]->SetName(L"Empty_Heart");
			}

			// 5 ~ 9�� : Full Heart
			else
			{
				m_arrHeart[i] = object::Instantiate<GameObject>(Vector3(m_arrHeartPos[i - 5].x, m_arrHeartPos[i - 5].y, 200.f), Vector3(36.f, 33.f, 100.f), eLayerType::UI);
				m_arrHeart[i]->SetName(L"Full_Heart");
			}

			SceneManager::DontDestroyOnLoad(m_arrHeart[i]);
			MeshRenderer* pMeshRender = m_arrHeart[i]->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		}
	}

	void HeartScript::Update()
	{
		// ���� ������Ʈ
		for (int i = 0; i < (int)eHeartColor::End; ++i)
		{
			Vector2 vLtUV = Vector2(0.f + (24.f * i), 0.f) / Vector2(48.f, 22.f);
			Vector2 vSliceUV = Vector2(24.f, 22.f) / Vector2(48.f, 22.f);
			m_arrMaterial[i]->SetScalarParam(Lu::graphics::SCALAR_PARAM::VEC2_0, &vLtUV);
			m_arrMaterial[i]->SetScalarParam(Lu::graphics::SCALAR_PARAM::VEC2_1, &vSliceUV);
		}
	}

	void HeartScript::SetMaxHP(int _MaxHP)
	{
		// 0 ~ 4�� : Empty ��Ʈ
		// �ý��� �� �ִ� ü�� : MaxHeart(5ĭ) * 2 = 10�� �Ѿ ���
		if(_MaxHP > MaxHeart * 2)
			return;

		if (_MaxHP < 0)
			return;

		int iBigHeartCount = _MaxHP / 2;
		int iSmallHeartCount = _MaxHP % 2;

		m_MaxCount = iBigHeartCount + iSmallHeartCount;

		int i = 0;
		// Small Heart
		if (0 != iSmallHeartCount)
		{
			ChangeHeartSize(i, eHeartSize::Small);
			ChangeHeartColor(i++, eHeartColor::Empty);
		}

		for (; i < MaxHeart; ++i)
		{
			// �ε����ϱ� -1�� �������
			if (i > m_MaxCount - 1)
			{
				m_arrHeart[i]->GetComponent<MeshRenderer>()->SetMaterial(nullptr);
			}
			else
			{
				ChangeHeartSize(i, eHeartSize::Big);
				ChangeHeartColor(i, eHeartColor::Empty);
			}
		}
	}

	void HeartScript::SetHeart(int _CurHP)
	{
		// 5 ~ 9�� : Full Heart
		if (_CurHP < 0)
			return;

		int iMaxIndex = m_MaxCount + 4;
		int iFullHeartCount = _CurHP / 2;
		int iHalfHeartCount = _CurHP % 2;
		int iEmptyHeartCount = m_MaxCount - (iFullHeartCount + iHalfHeartCount);

		int iMaxHP = m_MaxCount * 2;
		if (iHalfHeartCount != 0)
			--iMaxHP;
		
		if (iMaxHP < _CurHP)
			return;

		for (int i = MaxHeart; i < MaxHeart * 2; ++i)
		{
			// Empty Heart�� ������ ����
			if (i < MaxHeart + iEmptyHeartCount)
			{
				m_arrHeart[i]->GetComponent<MeshRenderer>()->SetMaterial(nullptr);
			}

			// MaxCount�� �ش���� �ʴ� ��Ʈ�� ������ ����
			else if (i >= m_MaxCount + MaxHeart)
			{
				m_arrHeart[i]->GetComponent<MeshRenderer>()->SetMaterial(nullptr);
			}

			else
			{
				// ù ��° �ε����� Half Heart
				if (iHalfHeartCount != 0)
				{
					if (i == MaxHeart + iEmptyHeartCount)
					{
						ChangeHeartColor(i, eHeartColor::Full);
						ChangeHeartSize(i, eHeartSize::Small);
					}
					else
					{
						ChangeHeartColor(i, eHeartColor::Full);
						ChangeHeartSize(i, eHeartSize::Big);
					}
				}
				else
				{
					ChangeHeartColor(i, eHeartColor::Full);
					ChangeHeartSize(i, eHeartSize::Big);
				}
			}
		}
	}

	void HeartScript::ChangeHeartColor(int _Index, eHeartColor _Color)
	{
		m_arrHeart[_Index]->GetComponent<MeshRenderer>()->SetMaterial(m_arrMaterial[(int)_Color]);
	}

	void HeartScript::ChangeHeartSize(int _Index, eHeartSize _Size)
	{
		if (_Size == eHeartSize::Small)
		{
			m_arrHeart[_Index]->GetComponent<Transform>()->SetScale(Vector3(36.f / 1.7f, 33.f / 1.7f, 100.f));
		}
		else
		{
			m_arrHeart[_Index]->GetComponent<Transform>()->SetScale(Vector3(36.f, 33.f, 100.f));
		}
	}

	void HeartScript::SetActive(bool _b)
	{
		for (int i = 0; i < (int)MaxHeart * 2; ++i)
		{
			if (m_arrHeart[i])
				m_arrHeart[i]->SetActive(_b);
		}

		GetOwner()->SetActive(_b);
	}
}