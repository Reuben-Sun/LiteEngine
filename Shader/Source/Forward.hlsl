#include "ShaderLibrary/LitInput.hlsl"
#include "ShaderLibrary/BRDF.hlsl"

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

Texture2D _BaseMap : register(t1);
SamplerState _BaseMap_ST : register(s1);

Texture2D _MetallicMap : register(t2);
SamplerState _MetallicMap_ST : register(s2);

Texture2D _NormalMap : register(t3);
SamplerState _NormalMap_ST : register(s3);

Texture2D _RoughnessMap : register(t4);
SamplerState _RoughnessMap_ST : register(s4);

float4 MainPS(Varyings input) : SV_TARGET
{
    float3 lightDir = ubo.dirLight.direction.xyz;
    float3 lightColor = ubo.dirLight.color.xyz * ubo.dirLight.intensity;
    float3 viewDir = normalize(ubo.cameraPosition.xyz - input.positionWS);
    
    float3 albedo = pushConstant.baseColor;
    if (pushConstant.textureEnable & ENABLE_BASECOLOR)
    {
        albedo *= _BaseMap.Sample(_BaseMap_ST, input.uv).xyz;
    }
    float metallic = pushConstant.metallic;
    if (pushConstant.textureEnable & ENABLE_METALLIC)
    {
        //metallic *= _MetallicMap.Sample(_MetallicMap_ST, input.uv).x;
    }
    float3 normalWS = input.normalWS;
    if (pushConstant.textureEnable & ENABLE_NORMAL)
    {
        float3 bitangent = cross(input.normalWS, input.tangentOS.xyz) * input.tangentOS.w;
        float3x3 TBN = float3x3(input.tangentOS.xyz, bitangent, input.normalWS);
        float3 normalTS = _NormalMap.Sample(_NormalMap_ST, input.uv).xyz;
        normalWS = normalize(mul(normalTS, TBN));
    }
    float roughness = pushConstant.roughness;
    if (pushConstant.textureEnable & ENABLE_ROUGHNESS)
    {
        //roughness *= _RoughnessMap.Sample(_RoughnessMap_ST, input.uv).x;
    }
    
    BRDFData data = (BRDFData) 0;
    data.albedo = albedo;
    data.metallic = metallic;
    data.emissionColor = pushConstant.emissionColor;
    data.roughness = roughness;
    
    Input litInput = (Input) 0;
    litInput.L = lightDir;
    litInput.radiance = lightColor;
    litInput.H = normalize(lightDir + viewDir);
    litInput.NoV = max(0.0f, dot(normalWS, viewDir));
    litInput.NoL = max(0.0f, dot(normalWS, lightDir));
    litInput.NoH = max(0.0f, dot(normalWS, litInput.H));
    litInput.VoH = max(0.0f, dot(viewDir, litInput.H));
    
    float4 result = BRDF(data, litInput);
    return result;
    //return float4(normalWS, 1.0f);
}
