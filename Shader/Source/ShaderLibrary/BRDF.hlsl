#ifndef BRDF_HLSL_INCLUDED
#define BRDF_HLSL_INCLUDED
struct BRDFData
{
    float3 albedo;
    float metallic;
    float reflectivity;
    float3 diffuse;
    float3 specular;
    float roughness;
    float roughness2;
    float grazingTerm;
};

static const float PI = 3.14159265359f;

inline float OneMinusReflectivityMetallic(float metallic)
{
    float oneMinusDielectricSpec = 1.0 - 0.04;
    return oneMinusDielectricSpec - metallic * oneMinusDielectricSpec;
}

float SpecularBRDF(BRDFData data, float3 normalWS, float3 viewDir, float3 lightDir)
{
    float3 H = normalize(viewDir + lightDir);
    float NoV = saturate(dot(normalWS, viewDir));
    float NoL = saturate(dot(normalWS, lightDir));
    float NoH = saturate(dot(normalWS, H));
    float VoH = saturate(dot(viewDir, H));
    float LoH = saturate(dot(normalWS, lightDir));
    float LoH2 = LoH * LoH;
    
    float normalizationTerm = data.roughness * 4.0 + 2.0;
    float roughness2MinusOne = data.roughness2 - 1.0;
    
    float D = NoH * NoH * roughness2MinusOne + 1.00001f;
    half specularTerm = data.roughness2 / ((D * D) * max(0.1h, LoH2) * normalizationTerm);
    return specularTerm;
}
#endif