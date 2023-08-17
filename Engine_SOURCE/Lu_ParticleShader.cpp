#include "Lu_ParticleShader.h"
#include "Lu_Renderer.h"
#include "Lu_ConstantBuffer.h"
#include "Lu_Time.h"

namespace Lu::graphics
{
	ParticleShader::ParticleShader()
		: ComputeShader(128, 1, 1)
		, m_ParticleBuffer(nullptr)
		, m_ModuleData(nullptr)
		, m_RWBuffer(nullptr)
	{
	}

	ParticleShader::~ParticleShader()
	{

	}

	void ParticleShader::SetParticleBuffer(StructedBuffer* _Buffer)
	{
		m_ParticleBuffer = _Buffer;
		m_Const.arrInt[0] = m_ParticleBuffer->GetStride();
	}

	void ParticleShader::Binds()
	{
		m_ParticleBuffer->BindUAV(0);
		m_RWBuffer->BindUAV(1);
		m_ModuleData->BindSRV(eShaderStage::CS, 20);
		m_NoiseTex->BindShaderResource(eShaderStage::CS, 21);

		m_GroupX = m_ParticleBuffer->GetStride() / m_ThreadGroupCountX + 1;
		m_GroupY = 1;
		m_GroupZ = 1;
	}

	void ParticleShader::Clear()
	{
		m_ParticleBuffer->Clear();
		m_RWBuffer->Clear();
		m_ModuleData->Clear();
		m_NoiseTex->Clear();
	}
}
