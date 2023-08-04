#include "Lu_ParticleShader.h"
#include "Lu_Renderer.h"
#include "Lu_ConstantBuffer.h"
#include "Lu_Time.h"

namespace Lu::graphics
{
	ParticleShader::ParticleShader()
		: ComputeShader(128, 1, 1)
		, mParticleBuffer(nullptr)
	{
	}

	ParticleShader::~ParticleShader()
	{
	}

	void ParticleShader::Binds()
	{
		mParticleBuffer->BindUAV(0);

		m_GroupX = mParticleBuffer->GetStride() / m_ThreadGroupCountX + 1;
		m_GroupY = 1;
		m_GroupZ = 1;
	}

	void ParticleShader::Clear()
	{
		mParticleBuffer->Clear();
	}

	void ParticleShader::SetParticleBuffer(StructedBuffer* particleBuffer)
	{
		//mParticleBuffer = particleBuffer;

		//ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Particle];

		//static float elapsedTime = 0.0f;
		//elapsedTime += Time::DeltaTime();

		//renderer::ParticleCB data = {};
		//data.elementCount = mParticleBuffer->GetStride();
		//data.elpasedTime = Time::DeltaTime();

		//cb->SetData(&data);
		//cb->Bind(eShaderStage::CS);
	}

}
