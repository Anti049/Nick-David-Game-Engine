#include "Precompiled.h"
#include "Texture3D.h"

Texture3D::Texture3D(void)
{
	m_pTexture = nullptr;
}

Texture3D::~Texture3D(void)
{
	SafeRelease(&m_pTexture);
}