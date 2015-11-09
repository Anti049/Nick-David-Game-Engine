#pragma once

#include "Texture.h"

class Texture2D : public Texture
{
public:
	Texture2D(void);
	~Texture2D(void);

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	ID3D11Texture2D*			GetTexture(void)							{ return m_pTexture; }

	//////////////////////////////////////////////////////////////////////////
	// Mutators
	void						SetTexture(ID3D11Texture2D* pTexture)		{ m_pTexture = pTexture; }

private:
	ID3D11Texture2D*			m_pTexture;
};