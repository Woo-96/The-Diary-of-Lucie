#include "globals.hlsli"

struct VSIn
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VSOut
{
    float4 vPos : SV_Position;
    float2 vUV : TEXCOORD;
};

VSOut main(VSIn _In)
{
    VSOut Out = (VSOut) 0.0f;

    float4 world = mul(float4(_In.vPos, 1.0f), WorldMatrix);
    float4 view = mul(world, ViewMatrix);
    float4 proj = mul(view, ProjectionMatrix);

    Out.vPos = proj;
    Out.vUV = _In.vUV;
 
    return Out;
}
