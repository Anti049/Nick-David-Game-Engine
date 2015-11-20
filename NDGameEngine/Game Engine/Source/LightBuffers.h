#ifndef _LIGHT_BUFFERS_H_
#define _LIGHT_BUFFERS_H_

#include "ShaderIncludes.h"

struct regAlign DirLightStruct
{
	// Projection Matrix, used for Shadow Mapping
	float4x4			mViewProjection;
	// Light Direction
	float3				vDirection;
	// Enabled State
	int					nEnabled;
	// Color
	float3				vColor;
	// Ambient
	float				fAmbient;
	// Controls the Size of the Specular Reflection. Smaller Value, Larger Reflection
	float				fSpecularPower;
	// Controls the Brightness of the Specular Reflection
	float				fSpecularIntensity;
	// Casts shadow
	int					nCastsShadow;
};
struct regAlign PointLightStruct
{
	// Light Position
	float4				vPosition;
	// Diffuse Color
	float3				vColor;
	// Enabled State
	int					nEnabled;
	// X = Constant, Y = Linear, Z = Quadratic
	float3				vAttenuation;
	// Controls the Size of the Specular Reflection. Smaller Value, Larger Reflection
	float				fSpecularPower;
	// Controls the Brightness of the Specular Reflection
	float				fSpecularIntensity;
	// Maximum Distance of the Light
	float				fRange;
};
struct regAlign PLightShadowStruct
{
	// Projection Matrix, used for Shadow Mapping
	float4x4			mViewProjection[6];
};
struct regAlign SpotLightStruct
{
	// Light Position
	float3				vPosition;
	// Controls how the Light Attenuates with Distance. Larger Value = Faster Falloff
	float				fExponent;
	// Light Direction
	float3				vDirection;
	// Enabled State
	int					nEnabled;
	// Diffuse Color
	float3				vDiffuseColor;
	// Width of Angle. Cosine of Angle between Direction and Furthest Lit Point
	float				fCutoff;
	// Ambient Color
	float3				vAmbientColor;
	// Controls the Size of the Specular Reflection. Smaller Value, Larger Reflection
	float				fSpecularPower;
	// Specular Color
	float3				vSpecularColor;
	// Controls the Brightness of the Specular Reflection
	float				fSpecularIntensity;
	// X = Constant, Y = Linear, Z = Quadratic
	float3				vAttenuation;
	// Maximum Distance of the Light
	float				fRange;
};
struct regAlign SLightShadowStruct
{
	// Projection Matrix, used for Shadow Mapping
	float4x4			mViewProjection;
};

CONSTANT_BUFFER_BEGIN(cbDirectionalLight, b4)
{
	DirLightStruct		DirLight;
#ifdef __cplusplus
	const static int REGISTER_SLOT = 4;
#endif
}
CONSTANT_BUFFER_END

	CONSTANT_BUFFER_BEGIN(cbPointLight, b5)
{
	PointLightStruct	PointLight;
#ifdef __cplusplus
	const static int REGISTER_SLOT = 5;
#endif
}
CONSTANT_BUFFER_END

	CONSTANT_BUFFER_BEGIN(cbPointLightShadow, b6)
{
	PLightShadowStruct	PointLightShadow;
#ifdef __cplusplus
	const static int REGISTER_SLOT = 6;
#endif
}
CONSTANT_BUFFER_END

	CONSTANT_BUFFER_BEGIN(cbSpotLight, b7)
{
	SpotLightStruct		SpotLight;
#ifdef __cplusplus
	const static int REGISTER_SLOT = 7;
#endif
}
CONSTANT_BUFFER_END

	CONSTANT_BUFFER_BEGIN(cbSpotLightShadow, b8)
{
	SLightShadowStruct	SpotLightShadow;
#ifdef __cplusplus
	const static int REGISTER_SLOT = 8;
#endif
}
CONSTANT_BUFFER_END

#endif