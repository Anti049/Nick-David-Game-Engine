float CalculateAttenuation(float3 attenuationFactors, float distanceToLight, float lightRange, 
						   float numerator = 1.0)
{
	float attenuation = numerator / (attenuationFactors.x + attenuationFactors.y * distanceToLight + attenuationFactors.z * distanceToLight * distanceToLight);
	float alpha = distanceToLight / lightRange;
	float damping = saturate(1.0f - alpha * alpha);
	attenuation *= damping;
	return attenuation;
}

float4 CalculateWorldSpacePosition(float2 pixelPosition, float pixelDepth, 
								   float4x4 inverseViewProjection)
{
	float4 viewSpacePos = float4(pixelPosition, pixelDepth, 1.0f);
	float4 worldSpacePos = mul(viewSpacePos, inverseViewProjection);
	worldSpacePos /= worldSpacePos.w;
	return worldSpacePos;
}

// Calculates how bright of a specualr highlight/reflection a given location should have
// normalizedToLight - a direction vector pointing to the light
// normal - a direction vector representing the orientation of the surface
// worldSpaceFragPos - The location of the fragment in world space
// specularLightIntensisty - The lights specular intensisty
// specularLightPower - The lights specular power
float CalculateSpecularAmount(float3 normalizedToLight, float3 normal, float3 worldSpaceFragPos,
							  float specularLightIntensisty, float specularLightPower)
{
	return 0;
}

float ComputeBlinnSpecularModulation(float3 unitHalfVec, float3 directionToCamera, float intensity, float power)
{
    float coeff = saturate(dot(unitHalfVec, directionToCamera));
    return saturate(intensity * pow(coeff, power));
}

float4 ApplyDirectionalLight(DirLightStruct DirLight, float3 directionToCamera, 
							 float3 halfVec, float4 normal, float4 matColor)
{
	float4 diffuse, ambient, specular, color;
	float nDotL, specMod;

	color = float4(0, 0, 0, 0);
    if(DirLight.nEnabled)
	{
		nDotL = saturate(dot(-DirLight.vDirection, normal.xyz));
		diffuse = matColor * float4(DirLight.vColor, 1);
		specular = float4(DirLight.vColor, 1);
		ambient = matColor * float4(DirLight.vColor, 1) * DirLight.fAmbient;

		specMod = ComputeBlinnSpecularModulation(normalize(halfVec), directionToCamera, 
			DirLight.fSpecularIntensity, DirLight.fSpecularPower) * (nDotL > 0);
		color.xyz = (/*nDotL * diffuse.xyz +*/ ambient.xyz + specMod * specular.xyz);
		color.a = specMod + diffuse.a;
	}
	return color;
}