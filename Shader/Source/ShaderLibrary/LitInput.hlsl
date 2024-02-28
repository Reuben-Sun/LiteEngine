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

// texture enable
#define ENABLE_BASECOLOR 0x2
#define ENABLE_METALLIC 0x4
#define ENABLE_NORMAL 0x8
#define ENABLE_ROUGHNESS 0x10

// debug enable

#endif