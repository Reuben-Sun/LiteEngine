struct Particle
{
    float2 positionWS;
    float2 velocity;
};

RWStructuredBuffer<Particle> particles : register(u0);

[numthreads(256, 1, 1)]
void MainCS(uint3 GlobalInvocationID : SV_DispatchThreadID)
{
    // Current SSBO index
    uint index = GlobalInvocationID.x;
    // Write back
    particles[index].velocity.xy = float2(1.0f, 0.5f);
    particles[index].positionWS = float2(0.5f, 1.0f);
}