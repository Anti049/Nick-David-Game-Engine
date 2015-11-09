#include "Precompiled.h"
#include "Texture2D.h"

Texture2D::Texture2D(void)
{
	m_pTexture = nullptr;
}

Texture2D::~Texture2D(void)
{
	SafeRelease(&m_pTexture);
}