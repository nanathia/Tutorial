cbuffer global
{
	matrix g_mW;
	matrix g_mWVP;
	float4 g_vLightDir;
	float4 g_Diffuse = float4(1, 0, 0, 0);
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


VS_OUTPUT VS(float4 Pos : POSITION, float4 Normal : NORMAL, float2 UV : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Pos = mul(Pos, g_mWVP);
	output.Normal = mul(Normal, (float3x3)g_mW);
	output.Light = g_vLightDir;

	float3 PosWorld = mul(Pos, g_mW);
	output.EyeVector = g_vEye - PosWorld;

	output.UV = UV;

	return output;
}

Texture2D g_texDecal : register(t0);
SamplerState g_samLinear : register(s0);

float4 PS(VS_OUTPUT input) : SV_Target
{
	float3 Normal = normalize(input.Normal);
	float3 LightDir = normalize(input.Light);
	float3 ViewDir = normalize(input.EyeVector);
	float4 NL = saturate(dot(Normal, LightDir));

	float3 Reflect = normalize(2 * NL * Normal - LightDir);
	float4 specular = 2 * pow(saturate(dot(Reflect, ViewDir)), 8);

	//input.UV[1] = 1.0 - input.UV[1];

	return g_texDecal.Sample(g_samLinear, input.UV)/*float4(1.0, 0.4, 1.0, 1.0)*//* * NL + specular*/;
}