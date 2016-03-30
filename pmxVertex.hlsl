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

VS_OUTPUT VS(float4 Pos : POSITION,
	float4 vBoneIndices : POSITION0, float4 vBoneWeights : POSITION1, float vWeitFormula : BLENDWEIGHT,
	float4 Normal : NORMAL, float2 UV : TEXCOORD)
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