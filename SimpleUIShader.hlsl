//�O���[�o��
cbuffer global
{
	float4x4 g_mWVP; //���[���h����ˉe�܂ł̕ϊ��s��
	float4 g_Diffuse = float4(1, 1, 1, 1); //�g�U����(�F�j	
};

//�\����
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD;
};

//
//�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT VS(float4 Pos : POSITION, float2 UV : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Pos = mul(Pos, g_mWVP);
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
	float4 ret = g_Diffuse * g_textureDecal.Sample(g_textureSampler, input.UV);
	return ret/* * ret.a*/;
}