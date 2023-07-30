#include "globals.hlsli"

#define LeftTop g_vec2_0
#define Slice   g_vec2_1

struct VSOut
{
    float4 vPos : SV_Position;
    float2 vUV : TEXCOORD;
};

float4 main(VSOut _In) : SV_TARGET
{
    float4 vOutColor = float4(1.0f, 0.f, 1.0f, 1.0f);
    
    float2 vUV = LeftTop + (Slice * _In.vUV);
   
    vOutColor = albedoTexture.Sample(anisotropicSampler, vUV);
    
    if (0.f == vOutColor.a)
    {
        discard;
    }
    
    return vOutColor;
}