struct VOut {
	float4 position : SV_POSITION;
	float3 WorldPos : POSITION;
	float3 Normal : NORMAL;
};

struct VS_INPUT {
	float4 position : POSITION;
	float4 normal : NORMAL;
	float2 uv: UV;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
	matrix viewMatrix;
	matrix projMatrix;
	float3 cameraPos;
};

cbuffer VS_MODEL_BUFFER : register(b1)
{
	matrix modelMatrix;
}

//cbuffer PS_MATERIAL_DATA_BUFFER : register(b2)
//{
//	float3 m_ambient;
//	float3 m_diffuse;
//	float3 m_specular;
//	float2 m_shininess; // TODO: google it
//} 

cbuffer PS_MATERIAL_DATA_BUFFER : register(b2) {
    float3 albedo;
    float metallic;
    float roughness;
    float ao;
}

cbuffer PS_LIGHT_BUFFER : register(b3)
{
	float3 lightPosition;
	float3 lightAmbient;
	float3 lightDiffuse;
	float3 lightSpecular;
}

static const float PI = 3.14159265359;

float DistributionGGX(float3 normalVector, float3 halfVector, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(normalVector, halfVector), .0);
    float NdotH2 = NdotH * NdotH;
    float nom = a2;
    float denom = NdotH2 * (a2 - 1.) + 1.;

    denom = PI * denom * denom;
    return nom / max(denom, .0001);
}

float GeometrySchlickGGX(float NdotV, float roungness)
{
    float r = roughness + 1.;
    float k = (r * r) / 8.;

    float nom = NdotV;
    float denom = NdotV * (1. - k) + k;
    
    return nom / denom;
}

float GeometrySmith(float3 normalVector, float3 eyeDir, float3 lightDir, float roughness)
{
    float NdotV = max(dot(normalVector, eyeDir), 0.);
    float NdotL = max(dot(normalVector, lightDir), 0.);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

float3 FresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1. - F0) * pow(1. - cosTheta, 5.);
}


VOut vs_main(VS_INPUT input) {
	VOut output;
	float4 normal = mul(modelMatrix, input.normal);
	float4 worldPos = mul(modelMatrix, input.position);

	output.position = mul(projMatrix, mul(viewMatrix, worldPos));
	output.WorldPos = worldPos.xyz / worldPos.w;
	output.Normal = normal.xyz / normal.w;
	return output;
}


float4 ps_main(VOut data) : SV_TARGET
{
    float3 lightColor = float3(300, 300, 300);
    float3 ambient = float3(.03, .03, .03) * albedo * ao;

	// reflectance 
    float3 F0 = float3(.04, .04, .04);
    F0 = lerp(F0, albedo, metallic); // from .04 for dia-electric to ALBEDO for metall
	// ***


    float3 normalVector = normalize(data.Normal);
    float3 eyeDir = normalize(cameraPos - data.WorldPos);
    float3 lightDir = normalize(lightPosition - data.WorldPos);
    float3 halfVector = normalize(eyeDir + lightDir);
    float distanceToLight = length(lightPosition - data.WorldPos);
    float attenuation = 1. / (distanceToLight * distanceToLight);
    float3 radiance = lightColor * attenuation;

	// Cook-Torrance BRDF
    float NDF = DistributionGGX(normalVector, halfVector, roughness);
    float G = GeometrySmith(normalVector, eyeDir, lightDir, roughness);
    float3 F = FresnelSchlick(clamp(dot(halfVector, eyeDir), 0., 1.), F0);

    float3 nom = NDF * G * F;
    float denom = 4 * max(dot(normalVector, eyeDir), 0.) * max(dot(normalVector, lightDir), 0.);
    float3 specular = nom / max(denom, .0001);

    float3 kD = float3(1, 1, 1) - F;
    kD *= 1. - metallic;

    float NdotL = max(dot(normalVector, lightDir), 0.);

    float3 Lo = (kD * albedo / PI + specular) * radiance * NdotL;

	

	

    //return float4(Lo, 1);
    return float4(1, 0, 0, 1);
}


//float4 ps_main(VOut data) : SV_TARGET {
//	float3 ambient = lightAmbient * m_ambient;

//	float3 norm = normalize(data.Normal);
//	float3 lightDir = normalize(lightPosition - data.WorldPos);
//	float diff = max(dot(norm, lightDir), 0.);
//	float3 diffuse = lightDiffuse * (diff * m_diffuse);

//	float3 viewDir = normalize(cameraPos - data.WorldPos);
//	float3 reflectDir = reflect(lightDir, norm);
//	float spec = pow(max(dot(viewDir, reflectDir), .0), m_shininess.x);
//	float3 specular = lightSpecular * (spec * m_specular);

//	float3 res = ambient + diffuse + specular;

//	// return float4(spec, 0, 0, 0);
//	return float4(res, 1.0);
//	// return float4(1., 0, 0, 0);


//}
