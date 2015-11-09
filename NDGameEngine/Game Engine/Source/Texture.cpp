#include "Precompiled.h"
#include "Texture.h"

Texture::Texture(void)
{
	m_pSRV = nullptr;
}

Texture::~Texture(void)
{
	SafeRelease(&m_pSRV);
}