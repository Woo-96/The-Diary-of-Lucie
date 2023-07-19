#include "globals.hlsli"

struct VSOut
{
    float4 vPos : SV_Position;
    float2 vUV : TEXCOORD;
};

float4 main(VSOut _In) : SV_TARGET
{
    float4 vOutColor = float4(1.0f, 0.f, 1.0f, 1.0f);
    
    float2 vUV = vLtUV + (vSliceUV * _In.vUV);
    
    vOutColor = albedoTexture.Sample(anisotropicSampler, vUV);
    
    if (0.f == vOutColor.a)
    {
        discard;
    }
    
    return vOutColor;
}