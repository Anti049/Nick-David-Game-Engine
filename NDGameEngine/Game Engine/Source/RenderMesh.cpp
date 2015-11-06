#include "Precompiled.h"
#include "RenderMesh.h"
#include "Renderer.h"
#include "IndexBuffer.h"

RenderMesh::RenderMesh(void)
{
	m_unNumVertices = 0;
	m_unNumIndices = 0;
	m_unNumPrimitives = 0;
	m_unStartVertex = 0;
	m_unStartIndex = 0;
	m_ePrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

RenderMesh::~RenderMesh(void)
{
	m_unNumVertices = 0;
	m_unNumIndices = 0;
	m_unNumPrimitives = 0;
	m_unStartVertex = 0;
	m_unStartIndex = 0;
	m_ePrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

void RenderMesh::AddIndices(std::vector<unsigned int> vIndices)
{
	m_vIndices = vIndices;
	m_unNumIndices = (unsigned int)vIndices.size();

	m_unStartIndex = IndexBuffer::GetInstance()->AddIndices(&vIndices[0], m_unNumIndices);
}