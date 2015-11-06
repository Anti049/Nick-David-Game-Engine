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
VertexBuffer<VERTEX_POSITION>& VertexBufferManager::GetVertexBuffer(void)
{
	return GetPositionBuffer();
}
template <>
VertexBuffer<VERTEX_POSCOLOR>& VertexBufferManager::GetVertexBuffer(void)
{
	return GetPosColorBuffer();
}