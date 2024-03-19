#ifndef LIGHTING_HLSL_INCLUDED
#define LIGHTING_HLSL_INCLUDED

#include "SurfaceData.hlsl"
#include "BRDF.hlsl"

Texture2D _BaseMap : register(t1);
SamplerState _BaseMap_ST : register(s1);

Texture2D _EmissionMap : register(t2);

Texture2D _NormalMap : register(t3);

Texture2D _OMRMap : register(t4);

TextureCube _SkyboxMap : register(t5);

static float3 _DebugColor = float3(1.0f, 0.0f, 1.0f);

float4 FragmentPBR(SurfaceInput inputData, SurfaceData surfaceData)
{
    return float4(1, 0, 1, 1);
}

#endif