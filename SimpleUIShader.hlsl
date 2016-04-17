cbuffer global{
	float2 Pos;
	float2 UV;
	float4 Color;
};

struct VS_OUTPUT{
	float4 Pos : SV_POSITION;
};

VS_OUTPUT VS(){
	VS_OUTPUT output;
	output.Pos[0] = Pos[0];
	output.Pos[1] = Pos[1];
	output.Pos[2] = 0;
	output.Pos[3] = 1;
	return output;
};

SamplerState g_Sampler : register(s0);
Texture2D g_textureDecal : register(t0);

float4 PS(VS_OUTPUT input) : SV_Target{
	float4 ret = g_textureDecal.Sample(g_Sampler, UV) * Color;
	return ret;
}