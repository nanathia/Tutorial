cbuffer global{
	float texelOfs[15];
};

struct VS_OUTPUT{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
};

VS_OUTPUT VS(float4 Pos : POSITION, float2 Tex : TEXCOORD){
	VS_OUTPUT output;
	output.Pos = Pos;
	output.Tex = Tex;
	return output;
};

SamplerState g_textureSampler : register(s0);
Texture2D g_textureDecal : register(t0);

float4 PS(VS_OUTPUT input) : SV_Target
{
	/*
	float tp1 = input.Tex[0] + texelOfs[0];
	float tp2 = input.Tex[0] + texelOfs[1];
	float tp3 = input.Tex[0] + texelOfs[2];
	float tp4 = input.Tex[0] + texelOfs[3];
	float tp5 = input.Tex[0] + texelOfs[4];

	float tp6 = input.Tex[0] - texelOfs[0];
	float tp7 = input.Tex[0] - texelOfs[1];
	float tp8 = input.Tex[0] - texelOfs[2];
	float tp9 = input.Tex[0] - texelOfs[3];
	float tp10 = input.Tex[0] - texelOfs[4];

	float2 t1 = float2(tp1, input.Tex[1]);
		float2 t2 = float2(tp2, input.Tex[1]);
		float2 t3 = float2(tp3, input.Tex[1]);
		float2 t4 = float2(tp4, input.Tex[1]);
		float2 t5 = float2(tp5, input.Tex[1]);
		float2 t6 = float2(tp6, input.Tex[1]);
		float2 t7 = float2(tp7, input.Tex[1]);
		float2 t8 = float2(tp8, input.Tex[1]);
		float2 t9 = float2(tp9, input.Tex[1]);
		float2 t10 = float2(tp10, input.Tex[1]);

		float4 tex0 = g_textureDecal.Sample(g_textureSampler, input.Tex) * 0.20;
		float4 tex1 = g_textureDecal.Sample(g_textureSampler, t1) * 0.12;
		float4 tex2 = g_textureDecal.Sample(g_textureSampler, t2) * 0.10;
		float4 tex3 = g_textureDecal.Sample(g_textureSampler, t3) * 0.08;
		float4 tex4 = g_textureDecal.Sample(g_textureSampler, t4) * 0.06;
		float4 tex5 = g_textureDecal.Sample(g_textureSampler, t5) * 0.04;
		float4 tex6 = g_textureDecal.Sample(g_textureSampler, t6) * 0.12;
		float4 tex7 = g_textureDecal.Sample(g_textureSampler, t7) * 0.10;
		float4 tex8 = g_textureDecal.Sample(g_textureSampler, t8) * 0.08;
		float4 tex9 = g_textureDecal.Sample(g_textureSampler, t9) * 0.06;
		float4 tex10 = g_textureDecal.Sample(g_textureSampler, t10) * 0.04;

		float4 ret = tex0 + tex1 + tex2 + tex3 + tex4 + tex5 + tex6 + tex7 + tex8 + tex9 + tex10;
		*/
		float4 ret = g_textureDecal.Sample(g_textureSampler, input.Tex);
		return /*float4(0.f, 0.f, 0.f, 1.0f) +*/ ret;
};