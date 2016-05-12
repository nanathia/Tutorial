

SamplerState g_textureSampler : register(s0);
Texture2D g_textureDecal : register(t0);

float4 PS(float4 pos : SV_POSITION, float2 tex : TEXCOORD) : SV_Target{
	return 0xffffffff;
//	return g_textureDecal.Sample(g_textureSampler, tex);
}