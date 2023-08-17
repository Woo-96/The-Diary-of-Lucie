#pragma once
#include "Lu_ComputeShader.h"
#include "Lu_StructedBuffer.h"
#include "Lu_Texture.h"

namespace Lu::graphics
{
	class ParticleShader : public ComputeShader
	{
	public:
		ParticleShader();
		virtual ~ParticleShader();

	private:
		StructedBuffer* m_ParticleBuffer;
		StructedBuffer* m_RWBuffer;
		StructedBuffer* m_ModuleData;

        std::shared_ptr<Texture>   m_NoiseTex;

    public:
        void SetParticleBuffer(StructedBuffer* _Buffer);

        void SetRWParticleBuffer(StructedBuffer* _Buffer)
        {
            m_RWBuffer = _Buffer;
        }

        void SetModuleData(StructedBuffer* _Buffer)
        {
            m_ModuleData = _Buffer;
        }

        void SetNoiseTexture(std::shared_ptr<Texture> _tex)
        {
            m_NoiseTex = _tex;
            m_Const.arrV2[0] = Vector2((float)m_NoiseTex->GetWidth(), (float)m_NoiseTex->GetHeight());
        }

        void SetParticleObjectPos(Vector3 _vPos)
        {
            m_Const.arrV4[0] = Vector4(_vPos.x, _vPos.y, _vPos.z, 0.f);
        }

		virtual void Binds() override;
		virtual void Clear() override;
	};
}

