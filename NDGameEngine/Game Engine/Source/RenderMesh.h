#pragma once
#include "VertexFormats.h"
#include "VertexBufferManager.h"

class RenderMesh
{
public:
	RenderMesh(void);
	~RenderMesh(void);

	template <typename VertexFormat>
	void										AddVertices(vector<VertexFormat> vVertices, D3D11_PRIMITIVE_TOPOLOGY ePrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void										AddIndices(vector<unsigned int> vIndices);

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	const unsigned int							GetNumVertices(void)	const										{ return m_unNumVertices; }
	const unsigned int							GetNumIndices(void)		const										{ return m_unNumIndices; }
	const unsigned int							GetNumPrimitives(void)	const										{ return m_unNumPrimitives; }
	const unsigned int							GetStartVertex(void)	const										{ return m_unStartVertex; }
	const unsigned int							GetStartIndex(void)		const										{ return m_unStartIndex; }
	const D3D11_PRIMITIVE_TOPOLOGY				GetPrimitiveType(void)	const										{ return m_ePrimitiveType; }
	const vector<VERTEX_POSCOLOR>				GetVertices(void)		const										{ return m_vVertices; }
	const vector<unsigned int>					GetIndices(void)		const										{ return m_vIndices; }

	//////////////////////////////////////////////////////////////////////////
	// Mutators
	void										SetNumVertices(const unsigned int unNumVertices)					{ m_unNumVertices = unNumVertices; }
	void										SetNumIndices(const unsigned int unNumIndices)						{ m_unNumIndices = unNumIndices; }
	void										SetNumPrimitives(const unsigned int unNumPrimitives)				{ m_unNumPrimitives = unNumPrimitives; }
	void										SetStartVertex(const unsigned int unStartVertex)					{ m_unStartVertex = unStartVertex; }
	void										SetStartIndex(const unsigned int unStartIndex)						{ m_unStartIndex = unStartIndex; }
	void										SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY ePrimitiveType)			{ m_ePrimitiveType = ePrimitiveType; }
	void										SetNumVertices(const vector<VERTEX_POSCOLOR> vVertices)				{ m_vVertices = vVertices; }
	void										SetNumVertices(const vector<unsigned int> vIndices)					{ m_vIndices = vIndices; }

private:
	// Numbers
	unsigned int								m_unNumVertices;
	unsigned int								m_unNumIndices;
	unsigned int								m_unNumPrimitives;
	unsigned int								m_unStartVertex;
	unsigned int								m_unStartIndex;

	// Primitive Topology
	D3D11_PRIMITIVE_TOPOLOGY					m_ePrimitiveType;

	// Arrays
	vector<VERTEX_POSCOLOR>						m_vVertices;
	vector<unsigned int>						m_vIndices;
};

template <typename VertexFormat>
void RenderMesh::AddVertices(vector<VertexFormat> vVertices, D3D11_PRIMITIVE_TOPOLOGY ePrimitiveType)
{
	m_unNumVertices = (unsigned int)vVertices.size();
	m_ePrimitiveType = ePrimitiveType;
	m_unNumPrimitives = m_unNumVertices / 3;
	if (m_unNumVertices % 3 != 0)
		m_unNumPrimitives++;

	m_unStartVertex = VertexBufferManager::GetInstance()->GetVertexBuffer<VertexFormat>().AddVerts(&vVertices[0], m_unNumVertices);
}