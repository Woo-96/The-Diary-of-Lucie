#include "Lu_ParticleSystem.h"
#include "Lu_Resources.h"
#include "Lu_GameObject.h"
#include "Lu_Time.h"

namespace Lu
{
	ParticleSystem::ParticleSystem()
		: m_ModuleData{}
		, m_Time(0.f)
	{
		SetMesh(Resources::Find<Mesh>(L"PointMesh"));
		SetMaterial(Resources::Find<Material>(L"ParticleMaterial"));
		m_UpdateCS = Resources::Find<ParticleShader>(L"ParticleSystemShader");

		// 모듈 데이터 초기화
		m_ModuleData.iMaxParticleCount = 1000;

		m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN] = true;
		m_ModuleData.SpawnRate = 20;
		m_ModuleData.vSpawnColor = Vector4(0.4f, 1.f, 0.4f, 1.f);
		m_ModuleData.vSpawnScaleMin = Vector4(15.f, 15.f, 1.f, 1.f);
		m_ModuleData.vSpawnScaleMax = Vector4(20.f, 20.f, 1.f, 1.f);

		m_ModuleData.SpawnShapeType = 0;
		m_ModuleData.vBoxShapeScale = Vector3(500.f, 500.f, 500.f);
		m_ModuleData.Space = 0; // 시뮬레이션 좌표계

		m_ModuleData.MinLifeTime = 3.f;
		m_ModuleData.MaxLifeTime = 5.f;

		m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE] = true;
		m_ModuleData.StartScale = 1.5f;
		m_ModuleData.EndScale = 0.2f;

		m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE] = true;
		m_ModuleData.vStartColor = Vector4(0.2f, 0.3f, 1.0f, 1.f);
		m_ModuleData.vEndColor = Vector4(0.4f, 1.f, 0.4f, 1.f);

		m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
		m_ModuleData.AddVelocityType = 0; // From Center
		m_ModuleData.Speed = 700.f;
		m_ModuleData.vVelocityDir;
		m_ModuleData.OffsetAngle;

		m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = true;
		m_ModuleData.StartDrag = 500.f;
		m_ModuleData.EndDrag = -500.f;

		m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = true;
		m_ModuleData.fNoiseTerm = 0.3f;
		m_ModuleData.fNoiseForce = 50.f;

		m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = true;
		m_ModuleData.VelocityAlignment = true;
		m_ModuleData.VelocityScale = true;
		m_ModuleData.vMaxVelocityScale = Vector4(15.f, 1.f, 1.f, 1.f);
		m_ModuleData.vMaxSpeed = 500.f;

		m_ParticleBuffer = new graphics::StructedBuffer();
		m_ParticleBuffer->Create(sizeof(tParticle), m_ModuleData.iMaxParticleCount, eViewType::UAV, nullptr, true);

		m_RWBuffer = new graphics::StructedBuffer();
		m_RWBuffer->Create(sizeof(tRWParticleBuffer), 1, eViewType::UAV, nullptr, true);

		m_ModuleDataBuffer = new graphics::StructedBuffer();
		m_ModuleDataBuffer->Create(sizeof(tParticleModule), 1, eViewType::SRV, nullptr, true);
	}

	ParticleSystem::~ParticleSystem()
	{
		if (nullptr != m_ParticleBuffer)
			delete m_ParticleBuffer;

		if (nullptr != m_RWBuffer)
			delete m_RWBuffer;

		if (nullptr != m_ModuleDataBuffer)
			delete m_ModuleDataBuffer;
	}
	
	void ParticleSystem::LateUpdate()
	{
		// SpawnRate를 이용하여 현재 프레임에 생성될 파티클 개수 계산
		m_Time += (float)Time::DeltaTime();

		// 개당 생성 시간 (1초 / 초당 파티클 생성 개수)
		float fTimePerCount = 1.f / (float)m_ModuleData.SpawnRate;


		if (fTimePerCount < m_Time)
		{
			// 누적 시간을 개당 생성 시간으로 나눈 몫 = 이번 프레임에 생성할 파티클 개수
			float fData = m_Time / fTimePerCount;

			// 누적 시간을 개당 생성 시간으로 나눈 나머지 = 누적 시간에 더할 초과 시간
			m_Time = fTimePerCount * float(fData - floor(fData));

			// RW버퍼에 스폰 카운트 전달
			tRWParticleBuffer rwbuffer = { (int)fData, };
			m_RWBuffer->SetData(&rwbuffer, 1);
		}


		// 모듈 데이터 Set
		m_ModuleDataBuffer->SetData(&m_ModuleData, 1);	// CPU Access가 true이므로 SetData 가능

		// 파티클 업데이트 셰이더에 버퍼 전달
		m_UpdateCS->SetParticleBuffer(m_ParticleBuffer);
		m_UpdateCS->SetRWParticleBuffer(m_RWBuffer);
		m_UpdateCS->SetModuleData(m_ModuleDataBuffer);

		//Resources::Load<Texture>(L"Noise01", L"..\\Resources\\Noise\\noise_01.png");
		//Resources::Load<Texture>(L"Noise02", L"..\\Resources\\Noise\\noise_02.png");
		//Resources::Load<Texture>(L"Noise03", L"..\\Resources\\Noise\\noise_03.png");
		m_UpdateCS->SetNoiseTexture(Resources::Load<Texture>(L"Noise01", L"..\\Resources\\Noise\\noise_01.png"));
		m_UpdateCS->SetParticleObjectPos(GetOwner()->GetComponent<Transform>()->GetPosition());


		// 파티클 업데이트 셰이더 tick 수행
		m_UpdateCS->OnExcute();
	}
	
	void ParticleSystem::Render()
	{
		// View 행렬과 투영 행렬을 업데이트 하기 위해 트랜스폼 바인딩
		GetOwner()->GetComponent<Transform>()->BindConstantBuffer();

		// 구조화 버퍼로 입자 데이터를 t20에 바인딩
		m_ParticleBuffer->BindSRV(eShaderStage::GS, 20);
		m_ParticleBuffer->BindSRV(eShaderStage::PS, 20);
		m_ParticleBuffer->BindSRV(eShaderStage::CS, 20);

		// 모듈 데이터 t21 에 바인딩
		m_ModuleDataBuffer->BindSRV(eShaderStage::GS, 21);
		m_ModuleDataBuffer->BindSRV(eShaderStage::PS, 21);
		m_ModuleDataBuffer->BindSRV(eShaderStage::CS, 21);

		// 파티클 렌더 셰이더 업데이트
		GetMaterial()->Binds();

		// 인스턴싱으로 하나의 파이프라인에서 파티클 최대 개수만큼 렌더링 수행
		GetMesh()->RenderInstanced(m_ModuleData.iMaxParticleCount);

		// 버퍼 바인딩 해제
		m_ParticleBuffer->Clear();
		m_ModuleDataBuffer->Clear();
	}
}