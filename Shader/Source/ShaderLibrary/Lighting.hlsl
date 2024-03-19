#ifndef LIGHTING_HLSL_INCLUDED
#define LIGHTING_HLSL_INCLUDED

#include "SurfaceData.hlsl"
#include "BRDF.hlsl"
#include "RealtimeLights.hlsl"

#define HALF_MIN_SQRT 0.0078125
#define HALF_MIN 6.103515625e-5

Texture2D _BaseMap : register(t1);
SamplerState _BaseMap_ST : register(s1);

Texture2D _EmissionMap : register(t2);

Texture2D _NormalMap : register(t3);

Texture2D _OMRMap : register(t4);

TextureCube _SkyboxMap : register(t5);

static float3 _DebugColor = float3(1.0f, 0.0f, 1.0f);

void InitializeBRDFData(inout SurfaceData surfaceData, out BRDFData brdfData)
{
    float oneMinusReflectivity = OneMinusReflectivityMetallic(surfaceData.metallic);
    float reflectivity = 1.0 - oneMinusReflectivity;
    float3 brdfDiffuse = surfaceData.albedo * oneMinusReflectivity;
    float3 brdfSpecular = lerp(float3(0.04, 0.04, 0.04), surfaceData.albedo, surfaceData.metallic);
    
    brdfData = (BRDFData) 0;
    brdfData.albedo = surfaceData.albedo;
    brdfData.diffuse = brdfDiffuse;
    brdfData.specular = brdfSpecular;
    brdfData.reflectivity = reflectivity;
    brdfData.roughness = max(surfaceData.roughness * surfaceData.roughness, HALF_MIN_SQRT);
    brdfData.roughness2 = max(brdfData.roughness * brdfData.roughness, HALF_MIN);
    brdfData.grazingTerm = saturate(1 - surfaceData.roughness + reflectivity);
}

float3 GlobalIllumination(BRDFData brdfData, float occlusion, float3 normalWS)
{
    float3 color = 0;
    // TODO: sample cubemap
    return color * occlusion;
}

float3 LightingPhysicallyBased(BRDFData brdfData, DirectionalLight mainLight, float3 normalWS, float3 viewDir)
{
    float NoL = saturate(dot(normalWS, mainLight.direction));
    float3 radiance = mainLight.color * NoL;
    float3 brdf = brdfData.diffuse;
    
    return brdf * radiance;
}


float4 FragmentPBR(SurfaceInput inputData, SurfaceData surfaceData, LightList lightData)
{
    BRDFData brdfData;
    InitializeBRDFData(surfaceData, brdfData);
    float3 giColor = GlobalIllumination(brdfData, surfaceData.occlusion, inputData.normalWS);
    float3 mainLightColor = LightingPhysicallyBased(brdfData, lightData.mainLight, inputData.normalWS, inputData.viewDirectionWS);
    float3 result = giColor + mainLightColor;
    return float4(result, 1);
}

#endif