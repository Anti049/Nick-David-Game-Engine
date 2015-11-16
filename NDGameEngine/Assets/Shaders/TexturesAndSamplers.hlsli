#ifndef _TEXTURES_AND_SAMPLERS_HLSLI_
#define _TEXTURES_AND_SAMPLERS_HLSLI_

///////////////////////////////////////////////////////////////////////////
// Object Textures
Texture2D				tAmbientMap						: register(t0);
Texture2D				tDiffuseMap						: register(t1);
Texture2D				tSpecularMap					: register(t2);
Texture2D				tEmissiveMap					: register(t3);
Texture2D				tNormalMap						: register(t4);
TextureCube				tReflectMap						: register(t5);
TextureCube				tCubeMap						: register(t6);
// Post-Process Textures
Texture2D				tPrePostMap						: register(t7);
Texture2D				tPostSourceMap					: register(t8);
// GBuffers
Texture2D				tDiffuseGBuffer					: register(t9);
Texture2D				tSpecularGBuffer				: register(t10);
Texture2D				tNormalGBuffer					: register(t11);
Texture2D				tDepthGBuffer					: register(t12);
// Destiny Shader System
Texture2D				tShaderMap						: register(t13);
Texture2D				tShaderColors					: register(t14);

///////////////////////////////////////////////////////////////////////////
// Samplers
SamplerState			sSpriteBatchSampler				: register(s0);
SamplerState			sAnisoWrapSampler				: register(s1);
SamplerState			sAnisoClampSampler				: register(s2);
SamplerState			sPointWrapSampler				: register(s3);
SamplerState			sPointClampSampler				: register(s4);
SamplerState			sLinearWrapSampler				: register(s5);
SamplerState			sLinearClampSampler				: register(s6);
SamplerComparisonState	sLinearBorderCompareSampler		: register(s7);

#endif //_TEXTURES_AND_SAMPLERS_HLSLI_