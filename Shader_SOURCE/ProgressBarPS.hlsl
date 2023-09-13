#include "globals.hlsli"

struct VSOut
{
    float4 vPos : SV_Position;
    float2 vUV : TEXCOORD;
};

#define UV_X    g_float_0

float4 main(VSOut _In) : SV_TARGET
{
    float4 vOutColor = float4(1.0f, 0.f, 1.0f, 1.0f);
   
    if(_In.vUV.x < UV_X)
        vOutColor = albedoTexture.Sample(anisotropicSampler, _In.vUV);
    else
        vOutColor = float4(0.1f, 0.1f, 0.1f, 1.0f);
    
    return vOutColor;
}
