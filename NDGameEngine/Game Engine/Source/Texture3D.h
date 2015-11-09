#pragma once

#include "Texture.h"

class Texture3D : public Texture
{
public:
	Texture3D(void);
	~Texture3D(void);

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	ID3D11Texture3D*			GetTexture(void)							{ return m_pTexture; }

	//////////////////////////////////////////////////////////////////////////
	// Mutators
	void						SetTexture(ID3D11Texture3D* pTexture)		{ m_pTexture = pTexture; }

private:
	ID3D11Texture3D*		m_pTexture;
};