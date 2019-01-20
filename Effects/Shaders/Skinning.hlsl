struct InstanceType
{
	matrix wVP;

	matrix world;

	float3 diffuseColor;
	float diffuseBlendFactor;

	float3 specularColor;
	float specularBlendFactor;

	float3 emissiveColor;
	float emissiveBlendFactor; 

	float2 diffuseTextureOffset;
	float2 specularTextureOffset;

	float2 emissiveTextureOffset;
	float2 normalTextureOffset;

	uint flags;
	float alphaFactor;
	float refractiveIndex;
	float specularHardness;

	float diffusePower;
	float specularPower;
	float emissivePower;
	uint bindMatrixOffset;

	//uint bindMatrixOffset;
	//uint instanceMaterialID;
};
StructuredBuffer<float4x4> boneMatrix : register(t5);
StructuredBuffer<InstanceType> instanceData : register(t6);

float4 BoneTransformPos(uniform float4 vertexPos, uniform float4 weight, uniform uint4 boneID)
{
	float4 result = float4(0, 0, 0, 0);
	for (int i = 0; i < 4; i++)
	{
		if (weight[i] > 0.001)
		{
			result += weight[i] * mul(vertexPos, boneMatrix[boneID[i]]);
		}
	}
	return result;
}

float3 BoneTransformNorm(uniform float3 vertexNormal, uniform float4 weight, uniform uint4 boneID)
{
	float3 result = float3(0, 0, 0);
	for (int i = 0; i < 4; i++)
	{
		if (weight[i] != 0)
		{
			result += weight[i] * normalize(mul(vertexNormal, (float3x3)boneMatrix[boneID[i]]));
		}
	}
	return result;
}
