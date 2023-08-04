#include "globals.hlsli"
RWStructuredBuffer<Particle> ParticleBuffer : register(u0);

[numthreads(128, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
   //if (elementCount <= DTid.x)
   //     return;
    
    
    //ParticleBuffer[DTid.x].Position += /*float4(1.0f, 0.0f, 0.0f, 1.0f)*/
    //ParticleBuffer[DTid.x].Direction * ParticleBuffer[DTid.x].Speed * elapsedTime;
}