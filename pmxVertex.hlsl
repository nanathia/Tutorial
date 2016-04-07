
cbuffer global
{
	matrix g_mW;
	matrix g_mV;
	matrix g_mP;
	matrix g_mWVP;

	matrix Bones[512];
};

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD;
	float3 Normal : BBBB;
};

//VS_OUTPUT VS(float4 Pos : POSITION,
//	float4 vBoneIndices : POSITION0, float4 vBoneWeights : POSITION1, float vWeitFormula : BLENDWEIGHT,
//	float4 Normal : NORMAL, float2 UV : TEXCOORD)
//{
//	VS_OUTPUT output = (VS_OUTPUT)0;
//
//
//
//	output.Pos = mul(Pos, mul(mul(g_mW, g_mV), g_mP));
//	output.Normal = mul(Normal, (float3x3)g_mW);
//	//output.Light = g_vLightDir;
//
//	float3 PosWorld = mul(Pos, g_mW);
//	//output.EyeVector = g_vEye - PosWorld;
//
//	output.UV = UV;
//
//	return output;
//}

VS_OUTPUT VS(float4 vPosition : POSITION0,
	float4 vBoneIndices : POSITION1, float4 vBoneWeights : POSITION2, float vWeightFormula : BLENDWEIGHT,
	float3 vNormal : NORMAL, float2 UV : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4 newVertex = vPosition;
		float4 newNormal;

	uint index1, index2, index3, index4, weightFormula = 4;

	index1 = vBoneIndices.x;
	index2 = vBoneIndices.y;
	index3 = vBoneIndices.z;
	index4 = vBoneIndices.w;

	weightFormula = vWeightFormula;

	newNormal = float4(vNormal, 0.0);

	//if (1){
	//	newVertex = mul(vPosition, mul(mul(g_mW, g_mV), g_mP));
	//	newNormal = normalize(mul(vNormal, g_mW));
	//}
	//else

		if (weightFormula == 0) //BDEF1
		{
			newVertex = mul(Bones[index1], vPosition);
			newNormal = mul(Bones[index1], float4(vNormal, 0.0));

		}
		else if (weightFormula == 1) //BDEF2 
		{
			newVertex = mul(Bones[index1], vPosition)          * vBoneWeights.x;
			newVertex += mul(Bones[index2], vPosition)          * (1.0 - vBoneWeights.x);

			newNormal = mul(Bones[index1], float4(vNormal, 0.0)) * vBoneWeights.x;
			newNormal += mul(Bones[index2], float4(vNormal, 0.0)) * (1.0 - vBoneWeights.x);

		}
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



