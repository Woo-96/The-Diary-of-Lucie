#pragma once
#include "Lu_MeshRenderer.h"
#include "Lu_StructedBuffer.h"
#include "Lu_ParticleShader.h"

namespace Lu
{
	class ParticleSystem : public MeshRenderer
	{
	public:
		ParticleSystem();
		virtual ~ParticleSystem();

	private:
		graphics::StructedBuffer* m_ParticleBuffer;
		graphics::StructedBuffer* m_RWBuffer;
		graphics::StructedBuffer* m_ModuleDataBuffer;

		std::shared_ptr<ParticleShader> m_UpdateCS;

		tParticleModule             m_ModuleData;
		float                       m_Time;

	public:
		tParticleModule GetModuleData() const
		{
			return m_ModuleData;
		}

		void SetModuleData(tParticleModule _ModuleData)
		{
			m_ModuleData = _ModuleData;
		}

	public:
		virtual void LateUpdate() override;
		virtual void Render() override;
	};
}
