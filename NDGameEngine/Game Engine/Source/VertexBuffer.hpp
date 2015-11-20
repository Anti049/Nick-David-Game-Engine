#pragma once

//#include "Precompiled.h"
#include "Renderer.h"
#include "VertexFormats.h"

template <typename VertexFormat>
class VertexBuffer
{
public:
	VertexBuffer(void);
	~VertexBuffer(void);

	unsigned int				AddVerts(const VertexFormat* pVertices, unsigned int unNumVerts, bool bDynamic = false);
	void						UpdateVerts(unsigned int unStartIndex, const VertexFormat* pVertices, unsigned int unNumVerts);
	void						Clear(void)
	{
		SafeRelease(&m_pVertexBuffer);
	}

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	ID3D11Buffer*				GetVertexBuffer(void) { return m_pVertexBuffer; }

private:
	ID3D11Buffer*				m_pVertexBuffer;
	bool						m_bDynamic;
};

template <typename VertexFormat>
VertexBuffer<VertexFormat>::VertexBuffer(void) 
{
	m_pVertexBuffer = 0;
	m_bDynamic = false;
}

template <typename VertexFormat>
VertexBuffer<VertexFormat>::~VertexBuffer(void) 
{
	Clear();
}

template <typename VertexFormat>
unsigned int VertexBuffer<VertexFormat>::AddVerts(const VertexFormat* pVertices, unsigned int unNumVerts, bool bDynamic)
{
	m_bDynamic = bDynamic;
	size_t ret;
	// Test if this buffer has already been finalized
	if(!m_pVertexBuffer)
	{
		ret = 0;

		D3D11_BUFFER_DESC vbd;
		vbd.Usage = bDynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
		vbd.ByteWidth = sizeof(VertexFormat) * unNumVerts;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = bDynamic ? D3D11_CPU_ACCESS_WRITE : NULL;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = pVertices;
		Renderer::m_pDevice->CreateBuffer( &vbd, &vinitData, &m_pVertexBuffer);

		string szType = typeid(VertexFormat).name();
		szType = szType.substr(14);
		string szName = "Vertex Buffer: " + szType;
		SetD3DName(m_pVertexBuffer, szName.c_str());
	}
	else
	{
		D3D11_BUFFER_DESC ibd;

		m_pVertexBuffer->GetDesc(&ibd);

		D3D11_SUBRESOURCE_DATA iinitData;
		UINT oldBuffeSize = ibd.ByteWidth / sizeof(VertexFormat);
		UINT newBuffeSize = oldBuffeSize + unNumVerts;

		iinitData.pSysMem = new VertexFormat[newBuffeSize];
		memcpy((char *)(iinitData.pSysMem) + ibd.ByteWidth, pVertices, sizeof(VertexFormat) * unNumVerts);
		ibd.ByteWidth += sizeof(VertexFormat) * unNumVerts;

		ID3D11Buffer* newVertexBufferPtr;
		Renderer::m_pDevice->CreateBuffer(&ibd, &iinitData, &newVertexBufferPtr);
		Renderer::m_pImmediateContext->CopySubresourceRegion(newVertexBufferPtr, 0, 0, 0, 0, m_pVertexBuffer, 0, 0);
		SafeRelease(&m_pVertexBuffer);
		m_pVertexBuffer = newVertexBufferPtr;

		delete [] iinitData.pSysMem;
		ret = oldBuffeSize;
	}
	return (unsigned int)ret;
}

template <typename VertexFormat>
void VertexBuffer<VertexFormat>::UpdateVerts(unsigned int unStartIndex, const VertexFormat* pVertices, unsigned int unNumVerts)
{
	if (m_pVertexBuffer && m_bDynamic)
	{
		D3D11_MAPPED_SUBRESOURCE resource;
		Renderer::m_pImmediateContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy((VertexFormat*)resource.pData + unStartIndex, pVertices, sizeof(VertexFormat) * unNumVerts);
		Renderer::m_pImmediateContext->Unmap(m_pVertexBuffer, NULL);
	}
}