#ifndef SURFACE_DATA_HLSL_INCLUDED
#define SURFACE_DATA_HLSL_INCLUDED

struct SurfaceData
{
    float3 albedo;
    float3 specular;
    float metallic;
    float smoothness;
    float3 normalTS;
    float3 emission;
    float occlusion;
    float alpha;
    float clearCoatMask;
    float clearCoatSmoothness;
};

#endif