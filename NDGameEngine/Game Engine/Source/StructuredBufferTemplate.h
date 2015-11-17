#pragma once

#define REGISTER_SLOT(Type) Type::REGISTER_SLOT

template <typename Type>
class StructuredBuffer
{
public:
	StructuredBuffer(void);
	StructuredBuffer(ID3D11Device* pDevice, int nElements, unsigned int unBindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE, bool bDynamic = false);
	~StructuredBuffer(void);

	// Dynamic ONLY
	void								Bind(ID3D11DeviceContext* pContext);
	Type*								MapDiscard(ID3D11DeviceContext* pContext);
	void								Unmap(ID3D11DeviceContext* pContext);

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	int									GetElements(void)			{ return m_nElements; }
	ID3D11Buffer*						GetBuffer(void)				{ return m_pBuffer; }
	ID3D11ShaderResourceView*			GetSRV(void)				{ return m_pSRV; }
	ID3D11UnorderedAccessView*			GetUAV(void)				{ return m_pUAV; }

private:
	int									m_nElements;
	ID3D11Buffer*						m_pBuffer;
	ID3D11ShaderResourceView*			m_pSRV;
	ID3D11UnorderedAccessView*			m_pUAV;
};

template <typename Type>
StructuredBuffer<Type>::StructuredBuffer(void)
	: m_nElements(0), m_pBuffer(nullptr), m_pSRV(nullptr), m_pUAV(nullptr)
{

}

template <typename Type>
StructuredBuffer<Type>::StructuredBuffer(ID3D11Device* pDevice, int nElements, unsigned int unBindFlags, bool bDynamic)
	: m_nElements(nElements), m_pBuffer(nullptr), m_pSRV(nullptr), m_pUAV(nullptr)
{
	CD3D11_BUFFER_DESC desc(sizeof(Type) * nElements, unBindFlags, 
		bDynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT,
		bDynamic ? D3D11_CPU_ACCESS_WRITE : 0,
		D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
		sizeof(Type));

	if (nElements)
	{
		pDevice->CreateBuffer(&desc, NULL, &m_pBuffer);
		if (unBindFlags & D3D11_BIND_UNORDERED_ACCESS)
			pDevice->CreateUnorderedAccessView(m_pBuffer, NULL, &m_pUAV);
		if (unBindFlags & D3D11_BIND_SHADER_RESOURCE)
			pDevice->CreateShaderResourceView(m_pBuffer, NULL, &m_pSRV);
	}
}

template <typename Type>
StructuredBuffer<Type>::~StructuredBuffer(void)
{
	SafeRelease(&m_pUAV);
	SafeRelease(&m_pSRV);
	SafeRelease(&m_pBuffer);
}

template <typename Type>
Type* StructuredBuffer<Type>::MapDiscard(ID3D11DeviceContext* pContext)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	pContext->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	return static_cast<Type*>(mappedResource.pData);
}

template <typename Type>
void StructuredBuffer<Type>::Unmap(ID3D11DeviceContext* pContext)
{
	pContext->Unmap(m_pBuffer, 0);
}

template <typename Type>
void StructuredBuffer<Type>::Bind(ID3D11DeviceContext* pContext)
{
	unsigned int numParticles = NUM_PARTICLES;
	pContext->CSSetUnorderedAccessViews(REGISTER_SLOT(Type), 1, &m_pUAV, &numParticles);
}