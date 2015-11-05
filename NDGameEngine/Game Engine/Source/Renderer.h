#pragma once

class Renderer
{
public:
	static Renderer* GetInstance(void)
	{
		if (!s_pInstance)
			s_pInstance = new Renderer;
		return s_pInstance;
	}
	static void DeleteInstance(void) { SafeDelete(s_pInstance); }

	void Initialize(HWND hWnd, int nScreenWidth, int nScreenHeight, bool bFullscreen);
	void Terminate(void);

	void Render(void);

	//////////////////////////////////////////////////////////////////////////
	// Static Members
	static IDXGISwapChain* m_pSwapChain;
	static ID3D11Device* m_pDevice;
	static ID3D11DeviceContext* m_pImmediateContext;
	static ID3D11RenderTargetView* m_pBackBuffer;
	static D3D11_VIEWPORT m_tViewPort;

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	int				GetScreenWidth(void)										{ return m_nScreenWidth; }
	int				GetScreenHeight(void)										{ return m_nScreenHeight; }
	bool			GetFullscreen(void)											{ return m_bFullscreen; }
	float*			GetClearColor(void)											{ return m_fClearColor; }
	//////////////////////////////////////////////////////////////////////////
	// Mutators
	void			SetScreenWidth(int nScreenWidth)							{ m_nScreenWidth = nScreenWidth; }
	void			SetScreenHeight(int nScreenHeight)							{ m_nScreenHeight = nScreenHeight; }
	void			SetFullscreen(bool bFullscreen)								{ m_bFullscreen = bFullscreen; }
	void			SetClearColor(float r, float g, float b, float a)			{ m_fClearColor[0] = r; m_fClearColor[1] = g; m_fClearColor[2] = b; m_fClearColor[3] = a; }

private:
	static Renderer* s_pInstance;
	Renderer(void);
	~Renderer(void);

	HWND m_hWnd;
	int m_nScreenWidth;
	int m_nScreenHeight;
	bool m_bFullscreen;
	float m_fClearColor[4];
};