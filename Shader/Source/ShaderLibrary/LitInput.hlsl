#ifndef LITINPUT_HLSL
#define LITINPUT_HLSL
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

struct PushConstant
{
    float4x4 modelMatrix;
    float3 baseColor;
    float metallic;
    float3 emissionColor;
    float roughness;
    
};
#endif