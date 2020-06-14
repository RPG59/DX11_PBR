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

cbuffer PS_MATERIAL_DATA_BUFFER : register(b2)
{
	float3 albedo;
	float mettalic;
	float roughness;
	float ao;
	float t1;
	float t2;
} 

cbuffer PS_LIGHT_BUFFER : register(b3)
{
	float3 lightPosition;
	float3 lightColor;
	float2 t123;
}

static const float PI = 3.14159265359;

float3 fresnelShlick(float cosTheta, float3 F0) {
	return F0 + (1. - F0) * pow(1. - cosTheta, 5.);
}

float distrubutionGGX(float3 N, float3 H, float roughness) {
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.);
	float denom = NdotH * (a2 - 1.) + 1.;
	denom = PI * denom * denom;

	return a / denom;
}

float GeometryShlickGGX(float NdotV, float roughness) {
	float r = roughness + 1.;
	float k = (r * r) / 8.;
	float denom = NdotV * (1. - k) + k;

	return NdotV / denom;
}

float GeometrySmith(float3 N, float3 V, float3 L, float roughness) {
	float NdotV = max(dot(N, V), 0.);
	float NdotL = max(dot(N, L), 0.);
	float ggx2 = GeometryShlickGGX(NdotV, roughness);
	float ggx1 = GeometryShlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
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

float4 ps_main(VOut foo) : SV_TARGET {
	float3 F0 = (float3).04;
	float3 Lo = (float3)0;
	// radiance
	float3 N = normalize(foo.Normal);
	float3 V = normalize(cameraPos - foo.WorldPos);
	float3 L = normalize(lightPosition - foo.WorldPos);
	float3 H = normalize(V + L);
	float distance = length(lightPosition - foo.WorldPos);
	float attenuation = 1. / (distance * distance);
	float3 radiance = lightColor * attenuation;

	// cook-torrance brdf
	float NDF = distrubutionGGX(N, H, roughness);
	float G = GeometrySmith(N, V, L, roughness);
	float3 F = fresnelShlick(max(dot(H, V), 0.), F0);
	
	float3 kS = F;
	float3 kD = (float3)(1.) - kS;
	kD  *= 1. - mettalic;

	float3 numerator = NDF * G * F;
	float denominator = 4. * max(dot(N, V), 0.) * max(dot(N, L), 0.);
	float3 specular = numerator / max(denominator, 0.001);

	float NdotL = max(dot(N, L), 0.);
	// += for multiple light sources
	Lo += (kD * albedo / PI + specular) * radiance * NdotL;

	float3 ambient = (float3)(.03) * albedo * ao;
	float3 color = ambient * Lo;

	color = abs(color / (color + (float3)(1.)));
	color = pow(color, (float3)(1. / 2.2));
	
	return float4(color, 1.);

	// return float4(1. / (distance * distance), 0, 0, 1);
	// return float4(1, 0, 0, 1);
}
