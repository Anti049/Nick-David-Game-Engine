#include "../../Game Engine/Source/ShaderIncludes.h"
#include "../../Game Engine/Source/ConstantBuffers.h"
#include "../../Game Engine/Source/VertexFormats.h"

[maxvertexcount(4)]
void main(point VERTEX_POSCOLOR particles[1], inout TriangleStream<GBufferVertexOut> output)
{
	VERTEX_POSCOLOR particle = particles[0];

	float3 zAxis = normalize(vCameraPosition - particle.m_vPosition.xyz);
	float3 xAxis = normalize(cross(float3(0, 1, 0), zAxis));
    float3 yAxis = cross(zAxis, xAxis);

	float4x4 localToWorld;
    localToWorld._11 = xAxis.x;
    localToWorld._21 = xAxis.y;
    localToWorld._31 = xAxis.z;
    localToWorld._12 = yAxis.x;
    localToWorld._22 = yAxis.y;
    localToWorld._32 = yAxis.z;
    localToWorld._13 = zAxis.x;
    localToWorld._23 = zAxis.y;
    localToWorld._33 = zAxis.z;
    localToWorld._41 = particle.m_vPosition.x;
    localToWorld._42 = particle.m_vPosition.y;
    localToWorld._43 = particle.m_vPosition.z;
    localToWorld._14 = 0;
    localToWorld._24 = 0;
    localToWorld._34 = 0;
    localToWorld._44 = 1;
	localToWorld = mWorld;
	float4x4 mViewProjection = mView * mProj;

	GBufferVertexOut v1 = (GBufferVertexOut)0, v2 = (GBufferVertexOut)0, v3 = (GBufferVertexOut)0, v4 = (GBufferVertexOut)0;
    float size = 0.5f;
    v1.m_vPosition = mul(float4(-size, size, 0, 1), mMVP);
    //v1.m_vPosition = mul(v1.m_vPosition, mViewProjection);
    v1.m_vTexCoord = float2(0, 0);
	v1.m_vNormal = normalize(zAxis);
	v1.m_vDepthDiv = v1.m_vPosition.zw;
    v2.m_vPosition = mul(float4(size, size, 0, 1), mMVP);
    //v2.m_vPosition = mul(v2.m_vPosition, mViewProjection);
    v2.m_vTexCoord = float2(1, 0);
	v2.m_vNormal = normalize(zAxis);
	v2.m_vDepthDiv = v2.m_vPosition.zw;
    v3.m_vPosition = mul(float4(-size, -size, 0, 1), mMVP);
    //v3.m_vPosition = mul(v3.m_vPosition, mViewProjection);
    v3.m_vTexCoord = float2(0, 1);
	v3.m_vNormal = normalize(zAxis);
	v3.m_vDepthDiv = v3.m_vPosition.zw;
    v4.m_vPosition = mul(float4(size, -size, 0, 1), mMVP);
    //v4.m_vPosition = mul(v4.m_vPosition, mViewProjection);
    v4.m_vTexCoord = float2(1, 1);
	v4.m_vNormal = normalize(zAxis);
	v4.m_vDepthDiv = v4.m_vPosition.zw;

    output.Append(v1);
    output.Append(v2);
    output.Append(v3);
    output.Append(v4);
}