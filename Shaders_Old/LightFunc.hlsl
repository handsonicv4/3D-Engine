
float4 Diffuse(float4 lightColor, float3 lightVec, float3 normal)
{
	float NdotL = max(dot(normal, lightVec), 0);
	return lightColor * NdotL;
}

float3 PointLightDiffuse(float3 lightColor, float3 lightPos,float3 pixelPos, float3 normal)
{
	float3 lightVec = lightPos - pixelPos;
	lightColor = lightColor /(1+pow(length(lightVec),2))*20;
	lightVec= normalize(lightVec);
	float NdotL = max(dot(normal, lightVec), 0);
	return lightColor * NdotL;
}

float4 Specular(float4 lightColor, float3 lightVec, float specularPower, float3 viewVec, float3 normal)
{
	float3 reflectionVec = normalize(reflect(-lightVec, normal));
	float RdotV = max(dot(reflectionVec, viewVec), 0);

	return lightColor * pow(RdotV, specularPower);
}

float4 SpecularBlinnPhong(float4 lightColor, float3 lightVec, float3 viewVec, float3 normal, float hardness)
{
	float3 H = normalize(lightVec + viewVec);
	float NdotH = dot(normal, H);
	float intensity = pow(saturate(NdotH), hardness);

	return lightColor * intensity;
}