

cbuffer global
{
	matrix g_mW;
	matrix g_mV;
	matrix g_mP;
	matrix g_mWVP;
};

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD;
	float3 Normal : BBBB;
};

VS_OUTPUT VS(float4 Pos : POSITION,
	float4 vBoneIndices : POSITION0, float4 vBoneWeights : POSITION1, float vWeitFormula : BLENDWEIGHT,
	float4 Normal : NORMAL, float2 UV : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;



	output.Pos = mul(Pos, mul(mul(g_mW, g_mV), g_mP));
	output.Normal = mul(Normal, (float3x3)g_mW);
	//output.Light = g_vLightDir;

	float3 PosWorld = mul(Pos, g_mW);
	//output.EyeVector = g_vEye - PosWorld;

	output.UV = UV;

	return output;
}