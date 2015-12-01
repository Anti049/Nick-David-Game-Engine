#include "Precompiled.h"
#include "IndexBuffer.h"
#include RendererPath

IndexBuffer* IndexBuffer::s_pInstance = nullptr;

IndexBuffer::IndexBuffer(void)
{
	m_pIndexBuffer = nullptr;
}

IndexBuffer::~IndexBuffer(void)
{
	SafeRelease(&m_pIndexBuffer);
}

unsigned int IndexBuffer::AddIndices(unsigned int* pIndices, unsigned int unIndices)
{
	unsigned int unReturn = 0;
	if (!m_pIndexBuffer)
	{
		D3D11_BUFFER_DESC descriptor;
		ZeroMemory( &descriptor, sizeof(descriptor) );	
		descriptor.Usage = D3D11_USAGE_DEFAULT;
		descriptor.ByteWidth = sizeof(unsigned int) * unIndices;
		descriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;
		descriptor.CPUAccessFlags = 0;
		descriptor.MiscFlags = 0;
		descriptor.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA contents;
		contents.pSysMem = pIndices;
		RendererType::m_pDevice->CreateBuffer(&descriptor, &contents, &m_pIndexBuffer);
		SetD3DName(m_pIndexBuffer, "Index Buffer");
	}
	else
	{
		D3D11_BUFFER_DESC descriptor;

		m_pIndexBuffer->GetDesc(&descriptor);

		D3D11_SUBRESOURCE_DATA contents;
		UINT prevSize = descriptor.ByteWidth / sizeof(unsigned int);
		unsigned int newSize = prevSize + unIndices;

		contents.pSysMem = new unsigned int[newSize];
		memcpy((char*)(contents.pSysMem) + descriptor.ByteWidth, pIndices, sizeof(unsigned int) * unIndices);
		descriptor.ByteWidth += (sizeof(unsigned int) * unIndices);

		ID3D11Buffer* newIndexBufferPtr;
		RendererType::m_pDevice->CreateBuffer(&descriptor, &contents, &newIndexBufferPtr);
		RendererType::m_pImmediateContext->CopySubresourceRegion(newIndexBufferPtr, 0, 0, 0, 0, m_pIndexBuffer, 0, 0);
		SafeRelease(&m_pIndexBuffer);
		m_pIndexBuffer = newIndexBufferPtr;

		delete [] contents.pSysMem;
		unReturn = prevSize;
	}
	return unReturn;
}