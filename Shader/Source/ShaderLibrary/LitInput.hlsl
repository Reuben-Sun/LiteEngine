struct Light
{
    float3 position;
    float intensity;
    float4 color;
    float4 direction;
};

struct UBO
{
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
    float4 cameraPosition;
    Light dirLight;
};