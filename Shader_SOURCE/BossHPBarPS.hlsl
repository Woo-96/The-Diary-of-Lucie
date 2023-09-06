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

#define UV_X    g_float_0

float4 main(VSOut In) : SV_TARGET
{
    float4 color = (float4) 0.0f;
    
    float2 UV = float2(UV_X, In.UV.y);
    color = albedoTexture.Sample(anisotropicSampler, UV);
 
    if (color.a <= 0.0f)
        discard;
    
    return color;
}