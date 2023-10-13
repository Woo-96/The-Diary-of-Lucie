#include "Lu_ManaScript.h"
#include "Lu_Resources.h"
#include "Lu_Object.h"
#include "Lu_MeshRenderer.h"

namespace Lu
{
	ManaScript::ManaScript()
		: m_arrMana{}
		, m_arrManaPos{}
		, m_MaxCount(0)
	{
		SetName(L"ManaScript");

		m_arrMaterial[(int)eManaColor::Empty] = Resources::Find<Material>(L"EmptyMana_Icon_Mtrl");
		m_arrMaterial[(int)eManaColor::Full] = Resources::Find<Material>(L"FullMana_Icon_Mtrl");
	}

	ManaScript::~ManaScript()
	{
	}

	void ManaScript::Initialize()
	{
		for (int i = 0; i < MaxMana * 2; ++i)
		{
			// 0 ~ 4번 : Empty
			if (i < MaxMana)
			{
				m_arrManaPos[i] = Vector2(63.f + (i * 36) + (i * 5.f), -280.f);
				m_arrMana[i] = object::Instantiate<GameObject>(Vector3(m_arrManaPos[i].x, m_arrManaPos[i].y, 200.f), Vector3(36.f, 36.f, 100.f), eLayerType::UI);
			}

			// 5 ~ 9번 : Full
			else
			{
				m_arrMana[i] = object::Instantiate<GameObject>(Vector3(m_arrManaPos[i - 5].x, m_arrManaPos[i - 5].y, 200.f), Vector3(36.f, 36.f, 100.f), eLayerType::UI);
			}

			m_arrMana[i]->SetName(L"Mana");
			SceneManager::DontDestroyOnLoad(m_arrMana[i]);
			MeshRenderer* pMeshRender = m_arrMana[i]->AddComponent<MeshRenderer>();
			pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		}
	}

	void ManaScript::Update()
	{
		// 재질 업데이트
		for (int i = 0; i < (int)eManaColor::End; ++i)
		{
			Vector2 vLtUV = Vector2(0.f + (24.f * i), 0.f) / Vector2(48.f, 24.f);
			Vector2 vSliceUV = Vector2(24.f, 24.f) / Vector2(48.f, 24.f);
			m_arrMaterial[i]->SetScalarParam(Lu::graphics::SCALAR_PARAM::VEC2_0, &vLtUV);
			m_arrMaterial[i]->SetScalarParam(Lu::graphics::SCALAR_PARAM::VEC2_1, &vSliceUV);
		}
	}

	void ManaScript::SetMaxMP(int _MaxMP)
	{
		// 0 ~ 4번 : Empty 
		if (_MaxMP > MaxMana * 2)
			return;

		if (_MaxMP < 0)
			return;

		m_MaxCount = _MaxMP;

		for (int i = 0; i < MaxMana; ++i)
		{
			if (i < m_MaxCount)
			{
				ChangeManaColor(i, eManaColor::Empty);
			}
			else
			{
				m_arrMana[i]->GetComponent<MeshRenderer>()->SetMaterial(nullptr);
			}
		}
	}

	void ManaScript::SetMana(int _CurMP)
	{
		// 최대 마나를 초과할 경우
		if (m_MaxCount < _CurMP)
			return;

		if (_CurMP < 0)
			return;

		for (int i = 0; i < MaxMana; ++i)
		{
			if (i < _CurMP)
			{
				ChangeManaColor(i + MaxMana, eManaColor::Full);
			}
			else
			{
				m_arrMana[i + MaxMana]->GetComponent<MeshRenderer>()->SetMaterial(nullptr);
			}
		}
	}

	void ManaScript::ChangeManaColor(int _Index, eManaColor _Color)
	{
		m_arrMana[_Index]->GetComponent<MeshRenderer>()->SetMaterial(m_arrMaterial[(int)_Color]);
	}

	void ManaScript::SetActive(bool _b)
	{
		for (int i = 0; i < (int)MaxMana *2; ++i)
		{
			if (m_arrMana[i])
				m_arrMana[i]->SetActive(_b);
		}

		GetOwner()->SetActive(_b);
	}
}