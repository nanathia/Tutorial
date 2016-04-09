

cbuffer global
{
	matrix g_mV;
	matrix g_mP;
	matrix test;
	matrix Bones[512];
};

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD;
	float3 Normal : BBBB;
};

VS_OUTPUT VS(float4 vPosition : POSITION0,
	uint4 vBoneIndices : POSITION1, float4 vBoneWeights : POSITION2, uint vWeightFormula : BLENDWEIGHT,
	float3 vNormal : NORMAL, float2 UV : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4 newVertex = vPosition;
	float4 newNormal;

	uint index1, index2, index3, index4, weightFormula = 4;

	index1 = vBoneIndices[0];
	index2 = vBoneIndices[1];
	index3 = vBoneIndices[2];
	index4 = vBoneIndices[3];

	weightFormula = vWeightFormula;

	newNormal = float4(vNormal, 0.0);

	//if (index1 > 300){
	//	newVertex = mul(vPosition, mul(mul(test, g_mV), g_mP));
	//	newNormal = normalize(mul(vNormal, test));
	//}else

	if (weightFormula == 0) //BDEF1
	{
		// 対象頂点はスキンメッシュを適用しない。通常ボーンアニメーションとして変換。
		newNormal = normalize(mul(vNormal, Bones[index1]));
		newVertex = mul(mul(mul(vPosition, Bones[index1]), g_mV), g_mP);
	}
	else if (weightFormula == 1) //BDEF2 
	{
		// 影響ボーン２つ。最も単純なスキンメッシュだ。
		float4x4 world = (float4x4)0;
		world += Bones[index1] * vBoneWeights[0];
		world += Bones[index2] * vBoneWeights[1];
		newNormal = normalize(mul(vNormal, world));
		newVertex = mul(mul(mul(vPosition, world), g_mV), g_mP);
	}

		//newVertex = mul(Bones[index1], vPosition)          * vBoneWeights.x;
		//newVertex += mul(Bones[index2], vPosition)          * (1.0 - vBoneWeights.x);

	//	newNormal = mul(Bones[index1], float4(vNormal, 0.0)) * vBoneWeights.x;
	//	newNormal += mul(Bones[index2], float4(vNormal, 0.0)) * (1.0 - vBoneWeights.x);

	//}
	//else if (weightFormula == 2) //BDEF4
	//{
	//	newVertex = (Bones[index1] * vPosition)          * vBoneWeights.x;
	//	newNormal = (Bones[index1] * float4(vNormal, 0.0)) * vBoneWeights.x;

	//	newVertex += (Bones[index2] * vPosition)          * vBoneWeights.y;
	//	newNormal += (Bones[index2] * float4(vNormal, 0.0)) * vBoneWeights.y;

	//	newVertex += (Bones[index3] * vPosition)          * vBoneWeights.z;
	//	newNormal += (Bones[index3] * float4(vNormal, 0.0)) * vBoneWeights.z;

	//	newVertex += (Bones[index4] * vPosition)          * vBoneWeights.w;
	//	newNormal += (Bones[index4] * float4(vNormal, 0.0)) * vBoneWeights.w;

	//	//newVertex = vPosition;
	//}
	//else if (weightFormula == 3) //Fake SDEF by using BDEF2 formula
	//{
	//	newVertex = (Bones[index1] * vPosition)          * vBoneWeights.x;
	//	newVertex += (Bones[index2] * vPosition)          * (1.0 - vBoneWeights.x);

	//	newNormal = (Bones[index1] * float4(vNormal, 0.0)) * vBoneWeights.x;
	//	newNormal += (Bones[index2] * float4(vNormal, 0.0)) * (1.0 - vBoneWeights.x);

	//	//newVertex = vPosition;
	//}
	//else // TODO: SDEF (weightFormula==3), QDEF (weightFormula==4)
	//{
	//	newVertex = vPosition;
	//	//newVertex = float4(0.0,0.0,0.0,0.0);
	//	newNormal = float4(vNormal, 0.0);
	//}

	output.Pos = newVertex;
	output.Normal = newNormal;
	output.UV = UV;

	return output;
}