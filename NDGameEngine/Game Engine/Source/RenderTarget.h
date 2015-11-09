#pragma once

class RenderSurface
{
public:
	RenderSurface(void);
	~RenderSurface(void);

	void							Create(unsigned int unWidth, unsigned int unHeight, DXGI_FORMAT eFormat, std::string szName);

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	unsigned int					GetWidth(void)							{ return m_unWidth; }
	unsigned int					GetHeight(void)							{ return m_unHeight; }
	ID3D11Texture2D*				GetTexture2D(void)						{ return m_pTexture2D; }
	ID3D11RenderTargetView*			GetRTV(void)							{ return m_pTarget; }
	ID3D11ShaderResourceView*		GetSRV(void)							{ return m_pShaderResourceView; }
	//////////////////////////////////////////////////////////////////////////
	// Mutators
	void							SetWidth(unsigned int unWidth)			{ m_unWidth = unWidth; }
	void							SetHeight(unsigned int unHeight)		{ m_unHeight = unHeight; }
	void							SetTexture2D(ID3D11Texture2D* pTex)		{ m_pTexture2D = pTex; }
	void							SetRTV(ID3D11RenderTargetView* pRTV)	{ m_pTarget = pRTV; }
	void							SetSRV(ID3D11ShaderResourceView* pSRV)	{ m_pShaderResourceView = pSRV; }

private:
	unsigned int					m_unWidth;
	unsigned int					m_unHeight;
	ID3D11Texture2D*				m_pTexture2D;
	ID3D11RenderTargetView*			m_pTarget;
	ID3D11ShaderResourceView*		m_pShaderResourceView;
};

class DepthSurface
{
public:
	DepthSurface(void);
	~DepthSurface(void);

	void							Create(unsigned int unWidth, unsigned int unHeight, DXGI_FORMAT eFormat, std::string szName);

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	unsigned int					GetWidth(void)							{ return m_unWidth; }
	unsigned int					GetHeight(void)							{ return m_unHeight; }
	ID3D11Texture2D*				GetTexture2D(void)						{ return m_pTexture2D; }
	ID3D11DepthStencilView*			GetDSV(void)							{ return m_pDSV; }
	ID3D11ShaderResourceView*		GetSRV(void)							{ return m_pShaderResourceView; }
	//////////////////////////////////////////////////////////////////////////
	// Mutators
	void							SetWidth(unsigned int unWidth)			{ m_unWidth = unWidth; }
	void							SetHeight(unsigned int unHeight)		{ m_unHeight = unHeight; }
	void							SetTexture2D(ID3D11Texture2D* pTex)		{ m_pTexture2D = pTex; }
	void							SetDSV(ID3D11DepthStencilView* pRTV)	{ m_pDSV = pRTV; }
	void							SetSRV(ID3D11ShaderResourceView* pSRV)	{ m_pShaderResourceView = pSRV; }

private:
	unsigned int					m_unWidth;
	unsigned int					m_unHeight;
	ID3D11Texture2D*				m_pTexture2D;
	ID3D11DepthStencilView*			m_pDSV;
	ID3D11ShaderResourceView*		m_pShaderResourceView;
};

class RenderTarget
{
public:
	RenderTarget(void);
	~RenderTarget(void);

	void							Initialize(std::string& szName, int nWidth, int nHeight, Vector4 vClearColor = Vector4(0.0f), unsigned int unNumTargets = 1);
	void							InitializeWithBackbuffer(std::string& szName, int nWidth, int nHeight, ID3D11Texture2D* pTexture, Vector4 vClearColor = Vector4(0.0f));
	void							Initialize(std::string& szName, unsigned int unWidth, unsigned int unHeight, ID3D11Texture2D* pDepthTexture, ID3D11DepthStencilView* pDepthStencilView);
	void							Terminate(void);
	void							ActivateTarget(bool bClear = false, unsigned int unClearFlag = D3D11_CLEAR_DEPTH);
	void							DeactivateTarget(void);
	bool							AddTarget(RenderSurface* pSurface);

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	ID3D11ShaderResourceView*		GetShaderResourceView(int nIndex)										{ return m_pRenderTargets[nIndex]->GetSRV(); }
	std::string						GetName(void)															{ return m_szName; }
	Vector4	GetClearColor(void)														{ return m_vClearColor; }
	int								GetWidth(void)															{ return m_nWidth; }
	int								GetHeight(void)															{ return m_nHeight; }
	ID3D11Texture2D*				GetDepthTexture(void)													{ return m_pDepthTexture; }
	ID3D11DepthStencilView*			GetDSV(void)															{ return m_pDepthStencilView; }
	//////////////////////////////////////////////////////////////////////////
	// Mutators
	void							SetName(std::string& szName)											{ m_szName = szName; }
	void							SetClearColor(Vector4 vClearColor)					{ m_vClearColor = vClearColor; }
	void							SetWidth(int nWidth)													{ m_nWidth = nWidth; }
	void							SetHeight(int nHeight)													{ m_nHeight = nHeight; }

private:
	std::vector<RenderSurface*>	m_pRenderTargets;
	ID3D11Texture2D*				m_pDepthTexture;
	ID3D11DepthStencilView*			m_pDepthStencilView;
	std::string						m_szName;
	Vector4	m_vClearColor;
	int								m_nWidth;
	int								m_nHeight;
	D3D11_VIEWPORT					m_sViewPort;

	unsigned int					m_unNumRenderTargets;

	bool							m_bReleaseDepth;
};