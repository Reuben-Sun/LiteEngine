#ifndef REALTIME_LIGHTS_HLSL_INCLUDED
#define REALTIME_LIGHTS_HLSL_INCLUDED

struct DirectionalLight
{
    float3 direction;
    float3 color;
};

struct LightList
{
    DirectionalLight mainLight;
};

#endif