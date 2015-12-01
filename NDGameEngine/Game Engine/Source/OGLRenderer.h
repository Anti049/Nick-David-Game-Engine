#pragma once

#include "Renderer.h"

class OGLRenderer : public Renderer
{
public:
	static Renderer*							GetInstance(void);
	static void									DeleteInstance(void);

	void										Initialize(HWND hWnd, int nScreenWidth, int nScreenHeight, bool bFullscreen);
	void										Terminate(void);

	void										Render(void);
	void										ComputeLighting(void);

	void										SetupGeometryContexts(void);
	void										SetupLightingContexts(void);

	void										CreateContext(map<std::string, RenderContext*>& pMap, RenderSet* pSet, string szShaderName, RenderNode::RenderFunc pFunc, ShaderType eType);
	void										AddRenderShape(RenderShape* pShape, string szContext);
	void										RemoveRenderShape(RenderShape* pShape, string szContext);

	void										InitializeTextureSamplers(void);

	void										SetUpUI(void);

	void										InitializeRenderSystem(void);
	void										InitializeConstantBuffers(void);

private:
	OGLRenderer(void);
	~OGLRenderer(void);
};