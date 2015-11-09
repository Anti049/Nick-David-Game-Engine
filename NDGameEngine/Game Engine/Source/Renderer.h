#pragma once

#include "RenderNode.hpp"
#include "VertexFormats.h"
#include "ConstantBuffers.h"
#include "ConstantBuffer.h"
#include "Camera.h"

class RenderTarget;
class RenderContext;
class RenderSet;
class ShaderTechnique;
class RenderShape;
class MeshDatabase;
class TextureDatabase;

typedef int (WINAPI *BeginEventSignature)(DWORD, LPCWSTR);
typedef int (WINAPI *EndEventSignature)(void);

class Renderer
{
public:
	static Renderer*							GetInstance(void)
	{
		if (!s_pInstance)
			s_pInstance = new Renderer;
		return s_pInstance;
	}
	static void									DeleteInstance(void) { SafeDelete(s_pInstance); }

	void										Initialize(HWND hWnd, int nScreenWidth, int nScreenHeight, bool bFullscreen);
	void										Terminate(void);

	void										Render(void);

	void										SetupGeometryContexts(void);
	void										CreateContext(map<std::string, RenderContext*>& pMap, RenderSet* pSet, string szShaderName, RenderNode::RenderFunc pFunc, ShaderType eType);
	void										AddRenderShape(RenderShape* pShape, string szContext);

	void										InitializeTextureSamplers(void);

	//////////////////////////////////////////////////////////////////////////
	// Static Members
	static IDXGISwapChain*						m_pSwapChain;
	static ID3D11Device*						m_pDevice;
	static ID3D11DeviceContext*					m_pImmediateContext;
	static RenderTarget*						m_pMainRenderTarget;
	static ID3D11Resource*						m_pBackbuffer;
	static ID3D11Texture2D*						m_pZBuffer;
	static ID3D11DepthStencilView*				m_pDepthStencilView;
	static D3D11_VIEWPORT						m_tViewPort;
	static HINSTANCE							dx9DLL;
	static BeginEventSignature					BeginEvent;
	static EndEventSignature					EndEvent;
	static MeshDatabase*						m_pMeshDatabase;
	static TextureDatabase*						m_pTextureDatabase;
	static Camera								m_cActiveCamera;

	static void									SetPerObjectData(Matrix mMVP, Matrix mWorld);
	static ConstantBuffer<cbPerObject>*			m_pPerObjectCBuffer;
	static void									SetCameraData(void);
	static ConstantBuffer<cbCamera>*			m_pCameraCBuffer;

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	int											GetScreenWidth(void)						{ return m_nScreenWidth; }
	int											GetScreenHeight(void)						{ return m_nScreenHeight; }
	bool										GetFullscreen(void)							{ return m_bFullscreen; }
	Vector4										GetClearColor(void)							{ return m_vClearColor; }
	//////////////////////////////////////////////////////////////////////////
	// Mutators
	void										SetScreenWidth(int nScreenWidth)			{ m_nScreenWidth = nScreenWidth; }
	void										SetScreenHeight(int nScreenHeight)			{ m_nScreenHeight = nScreenHeight; }
	void										SetFullscreen(bool bFullscreen)				{ m_bFullscreen = bFullscreen; }
	void										SetClearColor(Vector4 vColor)				{ m_vClearColor = vColor; }

private:
	static Renderer*							s_pInstance;
	Renderer(void);
	~Renderer(void);

	ID3D11SamplerState*							m_pSamplerStates[7];

	IDXGIDebug*									m_pDebug;

	HWND										m_hWnd;
	int											m_nScreenWidth;
	int											m_nScreenHeight;
	bool										m_bFullscreen;
	Vector4										m_vClearColor;
	RenderSet*									m_pGeometryContextList;
	map<std::string, RenderContext*>			m_pGeometryContextMap;
};