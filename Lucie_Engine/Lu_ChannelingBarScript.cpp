#include "Lu_ChannelingBarScript.h"
#include "Lu_GameObject.h"
#include "Lu_Object.h"
#include "Lu_ProgressBarScript.h"
#include "Lu_MeshRenderer.h"
#include "Lu_Resources.h"
#include "Lu_SceneManager.h"
#include "Lu_Camera.h"
#include "Lu_Renderer.h"
#include "Lu_Time.h"
#include "Lu_SoundManager.h"
#include "Lu_AudioSource.h"

namespace Lu
{
	ChannelingBarScript::ChannelingBarScript()
		: m_PlayerTransform(nullptr)
		, m_arrParts{}
		, m_bFirst(false)
		, m_bFXFirst(false)
		, m_bActive(false)
		, m_ChannelingType(eChannelingType::End)
		, m_Time(0.f)
		, m_MaxTime(2.f)
	{
	}

	ChannelingBarScript::~ChannelingBarScript()
	{
	}

	void ChannelingBarScript::SetChannelingType(eChannelingType _Type)
	{
		if (!m_bFirst)
		{
			m_ChannelingType = _Type;

			if (m_ChannelingType == eChannelingType::Charging)
			{
				m_arrParts[(int)eParts::Bar]->GetComponent<MeshRenderer>()->SetMaterial(Resources::Find<Material>(L"Shud_B_Mtrl"));
				m_MaxTime = 2.f;
			}
			else if (m_ChannelingType == eChannelingType::Consuming)
			{
				m_arrParts[(int)eParts::Bar]->GetComponent<MeshRenderer>()->SetMaterial(Resources::Find<Material>(L"Shud_C_Mtrl"));
			}
		}
	}

	bool ChannelingBarScript::IsComplete()
	{
		if (m_ChannelingType == eChannelingType::Consuming)
		{
			if(m_Time >= m_MaxTime)
				return true;
		}
		
		return false;
	}

	float ChannelingBarScript::GetChargeGauge()
	{
		if (m_ChannelingType == eChannelingType::Charging)
		{
			return m_Time / m_MaxTime;
		}

		return 0.0f;
	}

	void ChannelingBarScript::Initialize()
	{
		m_arrParts[(int)eParts::BarFram] = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 100.f), Vector3(115.5f, 25.5f, 500.f), Lu::enums::eLayerType::Default);
		m_arrParts[(int)eParts::BarFram]->SetName(L"ChannelingBarFrame");
		m_arrParts[(int)eParts::BarFram]->SetActive(m_bActive);
		SceneManager::DontDestroyOnLoad(m_arrParts[(int)eParts::BarFram]);
		MeshRenderer* pMeshRender = m_arrParts[(int)eParts::BarFram]->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		pMeshRender->SetMaterial(Resources::Find<Material>(L"Shud_A_Mtrl"));


		m_arrParts[(int)eParts::Bar] = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 100.f), Vector3(97.5f, 7.5f, 500.f), Lu::enums::eLayerType::Default);
		m_arrParts[(int)eParts::Bar]->SetName(L"ChannelingBar");
		m_arrParts[(int)eParts::Bar]->SetActive(m_bActive);
		SceneManager::DontDestroyOnLoad(m_arrParts[(int)eParts::Bar]);
		pMeshRender = m_arrParts[(int)eParts::Bar]->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		pMeshRender->SetMaterial(Resources::Find<Material>(L"Shud_B_Mtrl"));
		m_arrParts[(int)eParts::Bar]->AddComponent<ProgressBarScript>()->SetMeshRender(pMeshRender);


		m_arrParts[(int)eParts::ChargeFX] = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 300.f), Vector3(192.f, 192.f, 100.f), eLayerType::FX);
		m_arrParts[(int)eParts::ChargeFX]->SetName(L"ChargeFX");
		m_arrParts[(int)eParts::ChargeFX]->SetActive(m_bActive);
		SceneManager::DontDestroyOnLoad(m_arrParts[(int)eParts::ChargeFX]);
		pMeshRender = m_arrParts[(int)eParts::ChargeFX]->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		pMeshRender->SetMaterial(Resources::Find<Material>(L"WandCharge_Mtrl"));
		Animator* pAnimator = m_arrParts[(int)eParts::ChargeFX]->AddComponent<Animator>();
		pAnimator->Create(L"ChargeFX", Resources::Load<Texture>(L"WandCharge_Tex", L"..\\Resources\\Texture\\Player\\WandCharge.png")
			, Vector2(0.f, 0.f), Vector2(192.f, 192.f), 10, Vector2(192.f, 192.f), Vector2::Zero, 0.1f);
		pAnimator->PlayAnimation(L"ChargeFX", true);


		m_arrParts[(int)eParts::FullChargeFX] = object::Instantiate<GameObject>(Vector3(0.f, 0.f, 300.f), Vector3(192.f, 192.f, 100.f), eLayerType::FX);
		m_arrParts[(int)eParts::FullChargeFX]->SetName(L"FullChargeFX");
		m_arrParts[(int)eParts::FullChargeFX]->SetActive(m_bActive);
		SceneManager::DontDestroyOnLoad(m_arrParts[(int)eParts::FullChargeFX]);
		pMeshRender = m_arrParts[(int)eParts::FullChargeFX]->AddComponent<MeshRenderer>();
		pMeshRender->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		pMeshRender->SetMaterial(Resources::Find<Material>(L"WandFullCharge_Mtrl"));
		pAnimator = m_arrParts[(int)eParts::FullChargeFX]->AddComponent<Animator>();
		pAnimator->Create(L"FullChargeFX", Resources::Load<Texture>(L"WandFullCharge_Tex", L"..\\Resources\\Texture\\Player\\WandFullCharge.png")
			, Vector2(0.f, 0.f), Vector2(160.f, 192.f), 6, Vector2(160.f, 192.f), Vector2::Zero, 0.1f);
		pAnimator->CompleteEvent(L"FullChargeFX") = std::bind(&ChannelingBarScript::CompleteFX, this);
		pAnimator->PlayAnimation(L"FullChargeFX", true);
	}

	void ChannelingBarScript::Update()
	{
		if (m_ChannelingType == eChannelingType::Charging)
		{
			if (m_bActive)
			{
				m_Time += (float)Time::DeltaTime();

				if (m_Time > 0.5f)
				{
					for (int i = 0; i < (int)eParts::End; ++i)
					{
						if (m_arrParts[i])
						{
							if (!m_bFirst)
							{
								AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
								pSFX->SetClip(Resources::Load<AudioClip>(L"WandChargeSFX", L"..\\Resources\\Sound\\SFX\\Player\\WandChargeSFX.ogg"));
								pSFX->Play();
								pSFX->SetVolume(0.1f);

								if (i == (int)eParts::FullChargeFX)
								{
									m_bFirst = true;
									m_bFXFirst = false;
								}
								else
								{
									m_arrParts[i]->SetActive(m_bActive);
								}
							}

							Vector3 vPos = m_PlayerTransform->GetPosition();
							vPos.z = 200.f;

							if (i == (int)eParts::BarFram)
							{
								vPos.x += 5.f;
								vPos.y += 60.f;
							}
							else if (i == (int)eParts::Bar)
							{
								vPos.y += 65.f;
							}
							else if (i == (int)eParts::ChargeFX)
							{
								vPos.y -= 35.f;
							}

							m_arrParts[i]->GetComponent<Transform>()->SetPosition(vPos);
						}
					}

					ProgressBarScript* pProgressBar = m_arrParts[(int)eParts::Bar]->GetComponent<ProgressBarScript>();
					pProgressBar->SetValuePercent(m_Time / m_MaxTime);

					if (m_Time / m_MaxTime >= 1.f && !m_bFXFirst)
					{
						m_arrParts[(int)eParts::FullChargeFX]->SetActive(m_bActive);

						AudioSource* pSFX = SceneManager::FindSoundMgr()->GetComponent<SoundManager>()->GetSFX();
						pSFX->SetClip(Resources::Load<AudioClip>(L"WandFullChargeSFX", L"..\\Resources\\Sound\\SFX\\Player\\WandFullChargeSFX.ogg"));
						pSFX->Play();

						m_bFXFirst = true;
					}
				}
			}
		}
		else if (m_ChannelingType == eChannelingType::Consuming)
		{
			if (m_bActive)
			{
				m_Time += (float)Time::DeltaTime();

				for (int i = 0; i <= (int)eParts::Bar; ++i)
				{
					if (m_arrParts[i])
					{
						if (!m_bFirst)
						{
							m_arrParts[i]->SetActive(m_bActive);

							if (i == (int)eParts::Bar)
							{
								m_bFirst = true;
							}
						}

						Vector3 vPos = m_PlayerTransform->GetPosition();
						vPos.z = 200.f;

						if (i == (int)eParts::BarFram)
						{
							vPos.x += 5.f;
							vPos.y += 60.f;
						}
						else if (i == (int)eParts::Bar)
						{
							vPos.y += 65.f;
						}

						m_arrParts[i]->GetComponent<Transform>()->SetPosition(vPos);
					}

					ProgressBarScript* pProgressBar = m_arrParts[(int)eParts::Bar]->GetComponent<ProgressBarScript>();
					pProgressBar->SetValuePercent((m_MaxTime - m_Time) / m_MaxTime);
				}
			}
		}
	}

	void ChannelingBarScript::ChannelingOnOff(bool _b)
	{
		m_bActive = _b;

		if (!m_bActive)
		{
			m_Time = 0.f;
			m_bFirst = false;
			for (int i = 0; i < (int)eParts::End; ++i)
			{
				if (m_arrParts[i])
					m_arrParts[i]->SetActive(m_bActive);
			}
		}
	}

	void ChannelingBarScript::CompleteFX()
	{
		m_arrParts[(int)eParts::FullChargeFX]->GetComponent<Animator>()->FindAnimation(L"FullChargeFX")->Reset();
		m_arrParts[(int)eParts::FullChargeFX]->SetActive(false);
	}

	void ChannelingBarScript::SetActive(bool _b)
	{
		GetOwner()->SetActive(_b);
	}
}