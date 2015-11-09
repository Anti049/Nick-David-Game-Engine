#pragma once

class Texture
{
public:
	Texture(void);
	virtual ~Texture(void);

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	ID3D11ShaderResourceView*		GetSRV(void)								{ return m_pSRV; }

	//////////////////////////////////////////////////////////////////////////
	// Mutators
	void							SetSRV(ID3D11ShaderResourceView* pSRV)		{ m_pSRV = pSRV; }

protected:
	ID3D11ShaderResourceView*		m_pSRV;
};