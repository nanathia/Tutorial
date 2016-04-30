

//�O���[�o��
cbuffer global
{
	float4x4 g_mW;//���[���h�s��
	float4x4 g_mWVP; //���[���h����ˉe�܂ł̕ϊ��s��
	float4 g_vLightDir;//���C�g�̕����x�N�g��
	float4 g_Diffuse = float4(0, 0, 0, 1); //�g�U����(�F�j	
	float4 g_vEye;	//�J�����i���_�j
	float4 test;
};

//�\����
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 Light : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 EyeVector : TEXCOORD2;
	float2 UV : TEXCOORD3;
};

//
//�o�[�e�b�N�X�V�F�[�_�[
//
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

//
//�s�N�Z���V�F�[�_�[
//

SamplerState g_textureSampler : register(s0);
Texture2D g_textureDecal : register(t0);

float4 PS(VS_OUTPUT input) : SV_Target
{
	float3 Normal = normalize(input.Normal);
	float3 LightDir = normalize(input.Light);
	float3 ViewDir = normalize(input.EyeVector);
	float4 NL = dot(Normal, LightDir);

	float3 Reflect = normalize(NL * 2 * input.Normal  - LightDir); 
	float4 specular = 2 * pow(saturate(dot(Reflect, ViewDir)), 4);

	return /*0.5 * g_Diffuse*/ g_textureDecal.Sample(g_textureSampler, input.UV) + specular;
	//return g_Diffuse;
}