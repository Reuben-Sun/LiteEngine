#include "ShaderLibrary/Input.hlsl"
#include "ShaderLibrary/Lighting.hlsl"

struct Attributes
{
    float3 positionOS : POSITION0;
    float3 inColor : COLOR0;
    float2 texcoord : TEXCOORD0;
    float3 normalOS : NORMAL0;
    float4 tangentOS : TANGENT0;
};

struct Varyings
{
	float4 positionCS : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normalWS : NORMAL0;
    float3 positionWS : TEXCOORD1;
    float4 tangentOS : TEXCOORD2;
};

struct PushConstant
{
    float4x4 modelMatrix;
    float3 baseColor;
    float metallic;
    float3 emissionColor;
    float roughness;
    uint textureEnable;
    uint debugMode;
    float2 padding;
};

cbuffer ubo : register(b0) { UBO ubo; }

[[vk::push_constant]]PushConstant pushConstant;

Varyings MainVS(Attributes input)
{
    Varyings output = (Varyings) 0;
    output.positionCS = mul(ubo.projectionMatrix, mul(ubo.viewMatrix, mul(pushConstant.modelMatrix, float4(input.positionOS, 1.0f))));
    output.uv = input.texcoord;
    output.normalWS = normalize(mul(pushConstant.modelMatrix, float4(input.normalOS, 0.0f)).xyz);
    output.positionWS = mul(pushConstant.modelMatrix, float4(input.positionOS, 1.0f)).xyz;
    output.tangentOS = float4(input.tangentOS.xyz, 1.0f);
	return output;
}

float4 MainPS(Varyings input) : SV_TARGET
{
    SurfaceData surfaceData = (SurfaceData) 0;
    float3 albedo = pushConstant.baseColor;
    if (pushConstant.textureEnable & ENABLE_BASECOLOR)
    {
        albedo *= _BaseMap.Sample(_BaseMap_ST, input.uv).xyz;
    }
    surfaceData.albedo = albedo;
    float3 emission = pushConstant.emissionColor;
    if (pushConstant.textureEnable & ENABLE_EMISSION)
    {
        emission *= _EmissionMap.Sample(_BaseMap_ST, input.uv).xyz;
    }
    surfaceData.emission = emission;
    if (pushConstant.textureEnable & ENABLE_NORMAL)
    {
        float3 normalTS = _NormalMap.Sample(_BaseMap_ST, input.uv).xyz;
        surfaceData.normalTS = normalTS;   
    }
    float metallic = pushConstant.metallic;
    float roughness = pushConstant.roughness;
    float occlusion = 1.0f;
    if (pushConstant.textureEnable & ENABLE_OMR)
    {
        float3 omr = _OMRMap.Sample(_BaseMap_ST, input.uv).xyz;
        occlusion *= omr.r;
        metallic *= omr.g;
        roughness *= omr.b;
    }
    surfaceData.metallic = metallic;
    surfaceData.occlusion = occlusion;
    surfaceData.roughness = roughness;
    
    SurfaceInput inputData = (SurfaceInput) 0;
    float3 viewDir = normalize(ubo.cameraPosition.xyz - input.positionWS);
    inputData.viewDirectionWS = viewDir;
    float3 normalWS = input.normalWS;
    if (pushConstant.textureEnable & ENABLE_NORMAL)
    {
        float3 bitangent = cross(input.normalWS, input.tangentOS.xyz) * input.tangentOS.w;
        float3x3 TBN = float3x3(input.tangentOS.xyz, bitangent, input.normalWS);
        normalWS = normalize(mul(surfaceData.normalTS, TBN));
    }
    inputData.normalWS = normalWS;
    
    LightList lightData = (LightList) 0;
    lightData.mainLight.direction = ubo.dirLight.direction.xyz;
    lightData.mainLight.color = ubo.dirLight.color.xyz * ubo.dirLight.intensity;

    
    float4 result = FragmentPBR(inputData, surfaceData, lightData);

    if(pushConstant.debugMode == 1)
    {
        _DebugColor = input.normalWS;
    }
    else if(pushConstant.debugMode == 2)
    {
        _DebugColor = normalWS;
    }
    else if (pushConstant.debugMode == 3)
    {
        _DebugColor = metallic.xxx;
    }
    else if(pushConstant.debugMode == 4)
    {
        _DebugColor = roughness.xxx;
    }
    else if(pushConstant.debugMode == 5)
    {
        _DebugColor = emission;
    }
    
    if (pushConstant.debugMode == 0)
    {
        return float4(result.xyz, 1.0f);
    }
    else
    {
        return float4(_DebugColor, 1.0f);
    }
    
}
