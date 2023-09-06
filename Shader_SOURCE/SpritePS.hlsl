#include "globals.hlsli"

struct VSIn
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float3 WorldPos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

#define bFADING         g_int_3
#define ALPHA           g_float_3

float4 main(VSOut In) : SV_TARGET
{
    float4 color = (float4) 0.0f;
    
    if (bFADING)
    {
        color = albedoTexture.Sample(anisotropicSampler, In.UV);
        color.a *= ALPHA;
    }
    else
    {
        color = albedoTexture.Sample(anisotropicSampler, In.UV);
    }
    
    if (color.a <= 0.0f)
        discard;
    
    //float4 lightColor = float4(1.f, 1.f, 1.f, 1.f);
    
    //for (int i = 0; i < 2; i++)
    //{
    //    CalculateLight2D(lightColor, In.WorldPos, i);
    //}
    
    //color *= lightColor;
    
    return color;
}