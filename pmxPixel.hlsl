
// 2016-04-01 PMXEditerソースコード内、私がピクセルシェーダをhlslへ書き直したファイルになります。

SamplerState g_textureSampler : register(s0);
SamplerState g_sphereSampler : register(s15);
SamplerState g_toonSampler : register(s31);

Texture2D g_textureDecal : register(t2);
Texture2D g_sphereDecal : register(t1);
Texture2D g_toonDecal : register(t0);

cbuffer global{
	float3 ambient;
	float4 diffuse;
	float3 specular;
	float shininess;

	float3 lightDirection; //direction towards the light
	float3 halfVector; //surface orientation for shiniest spots- the vector stretching from from the camera position to the target

	bool isEdge;
	float4 edgeColor;
	float edgeSize;

	float fSphereMode; //sphereMode stored as a float
}

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
	float3 normal : BBBB;
};

float4 getSphereColor(VS_OUTPUT vs)
{
	int sphereMode = int(fSphereMode);

	float4 sphereColor = float4(0, 0, 0, 0);

	if (sphereMode>0) //if sphere mapping is used
	{
		float2 sphereCoord = 0.5 * ( 1.0 + float2(1.0, -1.0) * (float2)(normalize(vs.normal)) );
			sphereColor = g_sphereDecal.Sample(g_sphereSampler, sphereCoord);
	}

	return sphereColor;
}


float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 color;

	if (isEdge)
	{
		color = edgeColor;
		return color;
	}

	int sphereMode = int(fSphereMode);

	float3 textureColor = float3(1.0, 1.0, 1.0);

	textureColor *= g_textureDecal.Sample(g_textureSampler, input.uv).rgb;

	float4 sphereColor = getSphereColor(input);

	//finalColor*=sphereColor;

	if ((input.uv[0] != 0 && input.uv[1] != 0))
	{
		float intensity = dot(normalize(input.normal), lightDirection);
		float3 lightColor = normalize(float3(1.0, 1.0, 0.95));

		float3 scatteredLight;

		if (intensity > 0.99) scatteredLight = ambient + (diffuse.rgb*lightColor*.8);
		else if (intensity > 0.3) scatteredLight = ambient + (diffuse.rgb*lightColor * 0.75);
		else scatteredLight = ambient + (diffuse.rgb *lightColor * 0.5);

		float specularWeight;
		if (intensity == 0.0) //if surface is facing away from light
		{
			specularWeight = 0.0;
		}
		else
		{
			specularWeight = pow(max(0.0, dot(halfVector, input.normal)), shininess); //sharpen the highlight
		}

		intensity = dot(normalize(input.normal), halfVector);
		float3 reflectedLight = float3(0.0, 0.0, 0.0);
		if (intensity>.999)reflectedLight = (specular*specularWeight);


		float dotNL = max(0.0, dot(normalize(lightDirection), normalize(input.normal)));
		float2 toonCoord = float2(0.0, 0.5 * (1.0 - dotNL));
		float3 toon = g_toonDecal.Sample(g_toonSampler, toonCoord).rgb;

		float3 colorRGB = min((textureColor*scatteredLight) + reflectedLight, float3(1.0, 1.0, 1.0));
		color = float4(colorRGB, g_textureDecal.Sample(g_textureSampler, input.uv).a);

		/*if(sphereMode==1)
		{
		//color=float4(color.x*sphereColor.x,color.y*sphereColor.y,color.z*sphereColor.z,color.w);
		float3 colorTest=cross(float3(sphereColor),float3(color));
		//float3 colorTest=float3(color)*float3(sphereColor);
		color+=float4(colorTest,color.w);
		}
		else if(sphereMode==2)
		{
		color+=sphereColor;
		}*/

		/*if(sphereMode>0)
		{
		color=float4(1.0,0.0,0,1);
		}*/

	}
	else
	{
		if (input.normal.x == 1)
		{
			color = float4(0.0, 1.0, 0.0, 1.0);
		}
		else if (input.normal.y == 1)
		{
			color = float4(0.0, 0.4, 1.0, 1.0);
		}
		else
		{
			color = float4(1.0, 0.5, 0.5, 0.5);
		}
		//vec2 pos={0.1,0.1};

		//color = texture(textureSampler, input.uv).rgb;
		//color = texture(textureSampler, input.uv).rgb;
		//color = texture(textureSampler2, input.uv).rgb;
	}

	return color;
}