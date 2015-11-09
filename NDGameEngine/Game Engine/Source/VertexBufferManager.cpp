#include "Precompiled.h"
#include "VertexBufferManager.h"

VertexBufferManager* VertexBufferManager::s_pInstance = nullptr;

VertexBufferManager::VertexBufferManager(void)
{

}

VertexBufferManager::~VertexBufferManager(void)
{

}

template <>
VertexBuffer<VERTEX_POS2D>& VertexBufferManager::GetVertexBuffer(void)
{
	return GetPos2DBuffer();
}
template <>
VertexBuffer<VERTEX_POSCOLOR2D>& VertexBufferManager::GetVertexBuffer(void)
{
	return GetPosColor2DBuffer();
}
template <>
VertexBuffer<VERTEX_POSTEX2D>& VertexBufferManager::GetVertexBuffer(void)
{
	return GetPosTex2DBuffer();
}
template <>
VertexBuffer<VERTEX_POSITION>& VertexBufferManager::GetVertexBuffer(void)
{
	return GetPositionBuffer();
}
template <>
VertexBuffer<VERTEX_POSCOLOR>& VertexBufferManager::GetVertexBuffer(void)
{
	return GetPosColorBuffer();
}
template <>
VertexBuffer<VERTEX_POSTEX>& VertexBufferManager::GetVertexBuffer(void)
{
	return GetPosTexBuffer();
}
template <>
VertexBuffer<VERTEX_POSNORMTEX>& VertexBufferManager::GetVertexBuffer(void)
{
	return GetPosNormTexBuffer();
}