#ifndef LIGHTING_HLSL_INCLUDED
#define LIGHTING_HLSL_INCLUDED

#include "SurfaceData.hlsl"
#include "BRDF.hlsl"

float4 FragmentPBR(SurfaceInput inputData, SurfaceData surfaceData)
{
    return float4(1, 0, 1, 1);
}

#endif