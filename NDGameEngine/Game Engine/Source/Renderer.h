#pragma once

#include "RenderNode.hpp"
#include "VertexFormats.h"
#include "ConstantBuffers.h"
#include "ConstantBufferTemplate.h"
#include "Camera.h"
#include "LightBuffers.h"

class RenderTarget;
class RenderContext;
class RenderSet;
class ShaderTechnique;
class RenderShape;
class MeshDatabase;
class TextureDatabase;
class BlendStateManager;
class DepthStencilStateManager;
class RasterizerStateManager;
class LightManager;
class GBuffer;
class ParticleSystem;

typedef int (WINAPI *BeginEventSignature)(DWORD, LPCWSTR);
typedef int (WINAPI *EndEventSignature)(void);

class Renderer
{
public:
	Renderer(void) {}
	~Renderer(void) {}

	virtual void								Initialize(HWND hWnd, int nScreenWidth, int nScreenHeight, bool bFullscreen) = 0;
	virtual void								Terminate(void) = 0;
	virtual void								Render(void) = 0;
	virtual void								ComputeLighting(void) = 0;
	virtual void								SetupGeometryContexts(void) = 0;
	virtual void								SetupLightingContexts(void) = 0;
	virtual void								CreateContext(map<std::string, RenderContext*>& pMap, RenderSet* pSet, string szShaderName, RenderNode::RenderFunc pFunc, ShaderType eType) = 0;
	virtual void								AddRenderShape(RenderShape* pShape, string szContext) = 0;
	virtual void								RemoveRenderShape(RenderShape* pShape, string szContext) = 0;
	virtual void								InitializeTextureSamplers(void) = 0;
	virtual void								SetUpUI(void) = 0;
	virtual void								InitializeRenderSystem(void) = 0;
	virtual void								InitializeConstantBuffers(void) = 0;

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
	HWND										m_hWnd;
	int											m_nScreenWidth;
	int											m_nScreenHeight;
	bool										m_bFullscreen;
	Vector4										m_vClearColor;
	Vector4										m_vGBufferClearColor;

	bool										m_bViewGBuffer;

	ParticleSystem*								m_pParticleSystem;

protected:
	static Renderer*							s_pInstance;
};