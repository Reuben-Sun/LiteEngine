#ifndef SURFACE_DATA_HLSL_INCLUDED
#define SURFACE_DATA_HLSL_INCLUDED

struct SurfaceData
{
    float3 albedo;
    float3 specular; // TODO
    float metallic;
    float smoothness;
    float3 normalTS;
    float3 emission;
    float occlusion;
    float alpha; // TODO
    float clearCoatMask;    // TODO
    float clearCoatSmoothness; // TODO
};

struct SurfaceInput
{
    float3 positionWS;
    float4 positionCS;
    float3 normalWS;
    float3 viewDirectionWS;
    float4 shadowCoord; // TODO
    float fogCoord; // TODO
    float3 vertexLighting; // TODO
    float3 bakedGI; // TODO
    float2 normalizedScreenSpaceUV; // TODO
    float4 shadowMask; // TODO
    float3x3 tangentToWorld;
};

#endif