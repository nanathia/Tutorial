
cbuffer global{
	matrix mWVP;
};

struct VS_OUTPUT{
	float4 Pos : SV_POSITION;
};

VS_OUTPUT VS(float4 Pos : POSITION){
	VS_OUTPUT output;
	output.Pos = mul(Pos, mWVP);
	return output;
}

float4 PS() : SV_Target{
	return float4(0.0f, 0.f, 0.f, 0.4f);
}