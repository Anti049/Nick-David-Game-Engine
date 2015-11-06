#pragma once

#define REGISTER_SLOT(Type) Type::REGISTER_SLOT

template <typename Type>
class ConstantBuffer
{
public:
	ConstantBuffer(void);
	ConstantBuffer(ID3D11Device* pDevice, string szName, unsigned int unBindFlags = D3D11_BIND_CONSTANT_BUFFER, bool bDynamic = true);
	~ConstantBuffer(void);

	// Dynamic ONLY
	Type*								MapDiscard(ID3D11DeviceContext* pContext);
	void								Unmap(ID3D11DeviceContext* pContext);

	void								Bind(ID3D11DeviceContext* pContext);

	void								ModifyData(ID3D11DeviceContext* pContext, Type* tData);

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	ID3D11Buffer*						GetBuffer(void)				{ return m_pBuffer; }
	string								GetName(void)				{ return m_szName; }

private:
	ID3D11Buffer*						m_pBuffer;
	string								m_szName;
	Type								m_tData;
};

template <typename Type>
ConstantBuffer<Type>::ConstantBuffer(void) : m_pBuffer(nullptr), m_szName("")
{

}

template <typename Type>
ConstantBuffer<Type>::ConstantBuffer(ID3D11Device* pDevice, string szName, unsigned int unBindFlags, bool bDynamic) : m_pBuffer(nullptr), m_szName(szName)
{
	D3D11_BUFFER_DESC bdPerObject;
	ZeroMemory( &bdPerObject, sizeof(bdPerObject) );
	bdPerObject.Usage = D3D11_USAGE_DYNAMIC;
	bdPerObject.ByteWidth = sizeof(Type);
	bdPerObject.BindFlags = unBindFlags;
	bdPerObject.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pDevice->CreateBuffer(&bdPerObject, nullptr, &m_pBuffer);
#ifdef _DEBUG
	SetD3DName(m_pBuffer, szName.c_str());
#endif
}

template <typename Type>
ConstantBuffer<Type>::~ConstantBuffer(void)
{
	SafeRelease(&m_pBuffer);
}

template <typename Type>
Type* ConstantBuffer<Type>::MapDiscard(ID3D11DeviceContext* pContext)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	pContext->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	return static_cast<Type*>(mappedResource.pData);
}


template <typename Type>
void ConstantBuffer<Type>::Unmap(ID3D11DeviceContext* pContext)
{
	pContext->Unmap(m_pBuffer, 0);
}

template <typename Type>
void ConstantBuffer<Type>::Bind(ID3D11DeviceContext* pContext)
{
	pContext->VSSetConstantBuffers(REGISTER_SLOT(Type), 1, &m_pBuffer);
	pContext->GSSetConstantBuffers(REGISTER_SLOT(Type), 1, &m_pBuffer);
	pContext->DSSetConstantBuffers(REGISTER_SLOT(Type), 1, &m_pBuffer);
	pContext->HSSetConstantBuffers(REGISTER_SLOT(Type), 1, &m_pBuffer);
	pContext->PSSetConstantBuffers(REGISTER_SLOT(Type), 1, &m_pBuffer);
}