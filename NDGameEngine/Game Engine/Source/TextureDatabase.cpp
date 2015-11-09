#include "Precompiled.h"
#include "TextureDatabase.h"
#include "Texture2D.h"
#include "Texture3D.h"
#include "Renderer.h"

TextureDatabase::TextureDatabase(void)
{
	ClearTextureMap();
}

TextureDatabase::~TextureDatabase(void)
{
	ClearTextureMap();
}

Texture* TextureDatabase::LoadTexture(wstring szFilePath)
{
	if (m_pTextureMap.find(szFilePath) == m_pTextureMap.end())
	{
		ID3D11Resource* pTexture = nullptr;
		ID3D11ShaderResourceView* pSRV = nullptr;
		HRESULT hr = CreateDDSTextureFromFile(Renderer::m_pDevice, szFilePath.c_str(), &pTexture, &pSRV);

		D3D11_RESOURCE_DIMENSION tDimension;
		pTexture->GetType(&tDimension);

		if (tDimension == D3D11_RESOURCE_DIMENSION_TEXTURE2D)
		{
			m_pTextureMap[szFilePath] = new Texture2D;
			((Texture2D*)m_pTextureMap[szFilePath])->SetTexture((ID3D11Texture2D*)pTexture);
		}
		else if (tDimension == D3D11_RESOURCE_DIMENSION_TEXTURE3D)
		{
			m_pTextureMap[szFilePath] = new Texture3D;
			((Texture3D*)m_pTextureMap[szFilePath])->SetTexture((ID3D11Texture3D*)pTexture);
		}
		m_pTextureMap[szFilePath]->SetSRV(pSRV);
	}
	return m_pTextureMap[szFilePath];
}

void TextureDatabase::AddTexture(wstring szTexture, Texture* pTexture)
{
	if (m_pTextureMap.find(szTexture) == m_pTextureMap.end())
	{
		m_pTextureMap[szTexture] = pTexture;
	}
}

void TextureDatabase::RemoveTexture(Texture* pTexture)
{
	if (pTexture)
	{
		for (auto iter = m_pTextureMap.begin(); iter != m_pTextureMap.end(); iter++)
		{
			if ((*iter).second == pTexture)
			{
				SafeDelete((*iter).second);
				m_pTextureMap.erase(iter);
				pTexture = nullptr;
				return;
			}
		}
	}
}

void TextureDatabase::RemoveTexture(wstring szTexture)
{
	if (m_pTextureMap.find(szTexture) != m_pTextureMap.end())
	{
		if (m_pTextureMap[szTexture])
			SafeDelete(m_pTextureMap[szTexture]);
		m_pTextureMap.erase(szTexture);
	}
}

void TextureDatabase::ClearTextureMap(void)
{
	for (auto iter = m_pTextureMap.begin(); iter != m_pTextureMap.end(); iter++)
		SafeDelete((*iter).second);
	m_pTextureMap.clear();
}