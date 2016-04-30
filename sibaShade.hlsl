
cbuffer global{
	matrix mWVP;
	matrix mW;
	float3 vLight;
	float3 vEye;
};

struct VS_OUTPUT{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD;
	float3 Normal : NORMAL;
};

VS_OUTPUT VS(float4 Pos : POSITION, float2 UV : TEXCOORD, float3 Normal : NORMAL){
	VS_OUTPUT output;
	output.Pos = mul(Pos, mWVP);
	output.Normal = normalize(mul(Normal, (float3x3)mW));
	output.UV = UV;
	return output;
}

SamplerState g_textureSampler : register(s0);
Texture2D g_textureDecal : register(t0);

struct PS_OUTPUT{
	float4 Target0 : SV_TARGET0;
	float4 Target1 : SV_TARGET1;
};

PS_OUTPUT PS(VS_OUTPUT input){
	
	float3 vView = normalize(vEye - input.Pos);
	float3 NL = normalize(dot(input.Normal, vLight));
	float3 R = normalize(NL*2*input.Normal - vLight);
	float spe = 2 * pow(saturate(dot(R, vView)), 6);
	PS_OUTPUT output;
	output.Target0 = g_textureDecal.Sample(g_textureSampler, input.UV) * float4(NL, 1) + spe;
	output.Target1 = spe;
	return output;
}

