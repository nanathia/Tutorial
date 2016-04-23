cbuffer global{
	matrix mWVP;
	matrix mW;
};

struct VS_OUTPUT{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD;
	float3 Normal : NORMAL;
};

VS_OUTPUT VS(float4 Pos : POSITION, float3 Normal : NORMAL, float2 UV : TEXCOORD){
	VS_OUTPUT output;
	output.Pos = mul(Pos, mWVP);
	return output;
}

SamplerState g_textureSampler : register(s0);
Texture2D g_textureDecal : register(t0);

float4 PS(VS_OUTPUT input){
	float4 ret = g_textureDecal.Sample(g_textureSampler, input.UV);
	return ret;
}