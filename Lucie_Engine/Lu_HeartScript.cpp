#include "Lu_HeartScript.h"
#include "Lu_GameObject.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_Material.h"

namespace Lu
{
	HeartScript::HeartScript()
		: m_arrHeart{}
		, m_arrHeartPos{}
		, m_MaxIndex(0)
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
			// 0 ~ 4번 : Empty 하트
			if (i < MaxHeart)
			{
				m_arrHeartPos[i] = Vector2(-80.f - (i * 36) - (i * 5.f), -280.f);
				m_arrHeart[i] = object::Instantiate<GameObject>(Vector3(m_arrHeartPos[i].x, m_arrHeartPos[i].y, 100.f), Vector3(36.f, 33.f, 100.f), eLayerType::UI);
				MeshRenderer* pMeshRender = m_arrHeart[i]->AddComponent<MeshRenderer>();
				pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			}

			// 5 ~ 9번 : Full Heart
			else
			{
				m_arrHeart[i] = object::Instantiate<GameObject>(Vector3(m_arrHeartPos[i - 5].x, m_arrHeartPos[i - 5].y, 100.f), Vector3(36.f, 33.f, 100.f), eLayerType::UI);
				MeshRenderer* pMeshRender = m_arrHeart[i]->AddComponent<MeshRenderer>();
				pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			}
		}
	}

	void HeartScript::Update()
	{
		// 재질 업데이트
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
		// 0 ~ 4번 : Empty 하트
		// 시스템 상 최대 체력 : MaxHeart(5칸) * 2 = 10을 넘어선 경우
		if(_MaxHP > MaxHeart * 2)
			return;

		int iBigHeartCount = _MaxHP / 2;
		int iSmallHeartCount = _MaxHP % 2;

		m_MaxIndex = iBigHeartCount + iSmallHeartCount;

		int i = 0;
		// Small Heart
		if (0 != iSmallHeartCount)
		{
			ChangeHeartSize(i++, eHeartSize::Small);
		}

		for (; i < MaxHeart; ++i)
		{
			if (i > m_MaxIndex)
			{
				m_arrHeart[i]->GetComponent<MeshRenderer>()->SetMaterial(nullptr);
			}
			else
			{
				ChangeHeartSize(i, eHeartSize::Big);
			}
		}
	}

	void HeartScript::SetHeart(int _CurHP)
	{
		// 5 ~ 9번 : Full Heart
		int iMaxHP = m_MaxIndex * 2;
		int iFullHeartCount = _CurHP / 2;
		int iHalfHeartCount = _CurHP % 2;

		int i = MaxHeart;

		if (0 == iHalfHeartCount)
		{
			if (_CurHP > iMaxHP)
				return;

			// Full Heart
			if (_CurHP == iMaxHP)
			{
				for (; i < MaxHeart + iFullHeartCount; ++i)
				{
					ChangeHeartColor(i, eHeartColor::Full);
					ChangeHeartSize(i, eHeartSize::Big);
				}

				for (; i < MaxHeart * 2; ++i)
				{
					m_arrHeart[i]->GetComponent<MeshRenderer>()->SetMaterial(nullptr);
				}
			}
			// Empty Heart
			else
			{
				int iEmptyHeartCount = m_MaxIndex - iFullHeartCount;
				if (iMaxHP % 2 == 0)
				{
					for (; i < MaxHeart + iEmptyHeartCount; ++i)
					{
						ChangeHeartColor(i, eHeartColor::Empty);
						ChangeHeartSize(i, eHeartSize::Big);
					}
				}
				else
				{
					ChangeHeartColor(++i, eHeartColor::Empty);
					ChangeHeartSize(i, eHeartSize::Small);

					for (; i < MaxHeart + 1 + iEmptyHeartCount; ++i)
					{
						ChangeHeartColor(i, eHeartColor::Empty);
						ChangeHeartSize(i, eHeartSize::Big);
					}
				}

				for (; i < MaxHeart + iFullHeartCount; ++i)
				{
					ChangeHeartColor(i, eHeartColor::Full);
					ChangeHeartSize(i, eHeartSize::Big);
				}

				for (; i < MaxHeart * 2; ++i)
				{
					m_arrHeart[i]->GetComponent<MeshRenderer>()->SetMaterial(nullptr);
				}
			}
		}
		// HalfHeart
		else
		{
			if (_CurHP > --iMaxHP)
				return;

			// 첫 번째 하트
			ChangeHeartColor(i, eHeartColor::Full);
			ChangeHeartSize(i++, eHeartSize::Small);

			// 두 번째 하트 ~ 꽉찬 하트 개수
			for (; i < MaxHeart + 1 + iFullHeartCount; ++i)
			{
				ChangeHeartColor(i, eHeartColor::Full);
				ChangeHeartSize(i, eHeartSize::Big);
			}

			for (; i < MaxHeart * 2; ++i)
			{
				m_arrHeart[i]->GetComponent<MeshRenderer>()->SetMaterial(nullptr);
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
			m_arrHeart[_Index]->GetComponent<Transform>()->SetScale(Vector3(36.f / 1.5f, 33.f / 1.5f, 100.f));
		}
		else
		{
			m_arrHeart[_Index]->GetComponent<Transform>()->SetScale(Vector3(36.f, 33.f, 100.f));
		}
	}
}