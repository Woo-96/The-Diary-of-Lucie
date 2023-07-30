#include "globals.hlsli"

//structedBuffer
struct VSIn
{
    float3 Pos : POSITION;
};

struct VSOut
{
    float4 Pos : SV_Position;
};


VSOut main(VSIn In)
{
    VSOut Out = (VSOut) 0.0f;
    
    float4 world = mul(float4(In.Pos, 1.0f), WorldMatrix);
    float4 view = mul(world, ViewMatrix);
    float4 proj = mul(view, ProjectionMatrix);

    Out.Pos = proj;
    
    return Out;
}