cbuffer global
{
	matrix g_mW;
	matrix g_mWVP;
	float4 g_vLightDir;
	float4 g_Diffuse = float4(1, 0, 0, 0);
	float4 g_vAmbient;
	float4 g_vSpecular;
	float4 g_Specularlity;
	float4 g_vEye;
};

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD;
	float3 Light : AAAA;
	float3 Normal : BBBB;
	float3 EyeVector : CCCC;
};

Texture2D g_texDecal : register(t0);
SamplerState g_samLinear : register(s0);

float4 PS(VS_OUTPUT input) : SV_Target
{
	float3 Normal = normalize(input.Normal);
	float3 LightDir = normalize(input.Light);
	float3 ViewDir = normalize(input.EyeVector);
	float4 NL = saturate(dot(Normal, LightDir));

	float3 Reflect = normalize(2 * NL * Normal - LightDir);
	float4 specular = 2 * pow(saturate(dot(Reflect, ViewDir)), g_Specularlity[0]) * g_vSpecular;

	//input.UV[1] = 1.0 - input.UV[1];

	return g_vAmbient * g_texDecal.Sample(g_samLinear, input.UV) * NL*g_Diffuse + specular;
}