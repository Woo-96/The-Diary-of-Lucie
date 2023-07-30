#include "globals.hlsli"

struct VSIn
{
    float3 Pos : POSITION;
};

struct VSOut
{
    float4 Pos : SV_Position;
};


float4 main(VSOut In) : SV_TARGET
{
    float4 vOutColor = (float4) 0.f;
    
    vOutColor = g_vec4_0;
    
    return vOutColor;
}