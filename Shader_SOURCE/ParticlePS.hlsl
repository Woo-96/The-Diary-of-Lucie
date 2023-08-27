#include "globals.hlsli"
StructuredBuffer<tParticle> ParticleBuffer : register(t20);
StructuredBuffer<tParticleModule> ParticleModuleData : register(t21);

struct GS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    uint iInstID : SV_InstanceID;
};

float4 main(GS_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.0f, 0.f, 1.0f, 1.0f);
    
    vOutColor = albedoTexture.Sample(anisotropicSampler, _in.vUV);
    vOutColor.rgb *= ParticleBuffer[_in.iInstID].vColor.rgb;

    if (0.f == vOutColor.a)
    {
        discard;
    }
    
    return vOutColor;
}