#pragma once

#define REGISTER_SLOT(Type) Type::REGISTER_SLOT

template <typename Type>
class StructuredBuffer
{
public:
	StructuredBuffer(void);
	StructuredBuffer(ID3D11Device* pDevice, int nElements, const Type* pData, unsigned int unBindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE, bool bDynamic = false);
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
StructuredBuffer<Type>::StructuredBuffer(ID3D11Device* pDevice, int nElements, const Type* pData, unsigned int unBindFlags, bool bDynamic)
	: m_nElements(nElements), m_pBuffer(nullptr), m_pSRV(nullptr), m_pUAV(nullptr)
{
	HRESULT hr;

	D3D11_SUBRESOURCE_DATA contents;
	ZeroMemory(&contents, sizeof(contents));
	contents.pSysMem = pData;

	CD3D11_BUFFER_DESC structDesc(sizeof(Type) * nElements, D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT, 0, D3D11_RESOURCE_MISC_BUFFER_STRUCTURED, sizeof(Type));
	hr = RendererType::m_pDevice->CreateBuffer(&structDesc, &contents, &m_pBuffer);

	CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(m_pBuffer, DXGI_FORMAT_UNKNOWN, 0, nElements);
	hr = RendererType::m_pDevice->CreateShaderResourceView(m_pBuffer, &srvDesc, &m_pSRV);

	CD3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc(m_pBuffer, DXGI_FORMAT_UNKNOWN, 0, nElements, 0);
	hr = RendererType::m_pDevice->CreateUnorderedAccessView(m_pBuffer, &uavDesc, &m_pUAV);
}

template <typename Type>
StructuredBuffer<Type>::~StructuredBuffer(void)
{
	SafeRelease(&m_pBuffer);
	SafeRelease(&m_pUAV);
	SafeRelease(&m_pSRV);
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
	unsigned int numParticles = MAX_PARTICLES;
	pContext->CSSetUnorderedAccessViews(REGISTER_SLOT(Type), 1, &m_pUAV, &numParticles);
}