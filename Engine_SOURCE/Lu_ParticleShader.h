#pragma once
#include "Lu_ComputeShader.h"
#include "Lu_StructedBuffer.h"

namespace Lu::graphics
{
	class ParticleShader : public ComputeShader
	{
	public:
		ParticleShader();
		virtual ~ParticleShader();

	private:
		StructedBuffer* mParticleBuffer;

	public:
		void SetParticleBuffer(StructedBuffer* particleBuffer);

		virtual void Binds() override;
		virtual void Clear() override;
	};
}

