#include "Precompiled.h"
#include "GBuffer.h"
#include "Renderer.h"
#include "RenderContext.h"
#include "RenderShape.h"
#include "MeshDatabase.h"
#include "Material.h"
#include "Texture2D.h"
#include "TextureDatabase.h"
#include "EngineUIManager.h"
#include "ShaderPass.h"
#include "ShaderTechnique.h"
#include "RenderSet.h"

RenderShape* pView = nullptr;

GBuffer::GBuffer(void)
{
	m_pRenderTarget = nullptr;
	for (int i = 0; i < BUFFER_COUNT; i++)
		m_pGBufferTargets[i] = nullptr;
	m_pViewGBuffer = nullptr;
}

GBuffer::~GBuffer(void)
{
	SafeDelete(m_pRenderTarget);
	SafeDelete(m_pViewGBuffer);

	/*m_pActiveViewTexture->SetTexture(nullptr);
	m_pActiveViewTexture->SetSRV(nullptr);
	SafeDelete(m_pActiveViewTexture);*/
}

void GBuffer::Bind(void)
{
	m_pRenderTarget->ActivateTarget(true, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL);
}

void GBuffer::Unbind(void)
{
	m_pRenderTarget->DeactivateTarget();
}

void GBuffer::Initialize(unsigned int unWidth, unsigned int unHeight)
{
	m_pRenderTarget = new RenderTarget;
	m_pRenderTarget->Initialize(std::string("GBuffer"), unWidth, unHeight, Renderer::m_pMainRenderTarget->GetDepthTexture(), Renderer::m_pMainRenderTarget->GetDSV());
	m_pRenderTarget->SetClearColor(DirectX::SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	for (int i = 0; i < BUFFER_COUNT; i++)
		m_pGBufferTargets[i] = new RenderSurface;
	m_pGBufferTargets[DIFFUSE]->Create(unWidth, unHeight, DXGI_FORMAT_R8G8B8A8_UNORM, "GBufferDiffuse");
	m_pGBufferTargets[SPECULAR]->Create(unWidth, unHeight, DXGI_FORMAT_R10G10B10A2_UNORM, "GBufferSpecular");
	m_pGBufferTargets[NORMAL]->Create(unWidth, unHeight, DXGI_FORMAT_R10G10B10A2_UNORM, "GBufferNormal");
	m_pGBufferTargets[DEPTH]->Create(unWidth, unHeight, DXGI_FORMAT_R8G8B8A8_UNORM, "GBufferDepth");
	m_pGBufferTargets[EMISSIVE]->Create(unWidth, unHeight, DXGI_FORMAT_R8G8B8A8_UNORM, "GBufferEmissve");
	for (int i = 0; i < BUFFER_COUNT; i++)
		m_pRenderTarget->AddTarget(m_pGBufferTargets[i]);

	m_pViewGBuffer = new RenderContext;
	ShaderPass* pViewGBuffer = new ShaderPass;
	m_pViewGBuffer->SetRenderSet(new RenderSet);
	m_pViewGBuffer->SetRenderFunc(RenderContext::Context2DTextureRenderFunc);
	pViewGBuffer->CreateVertexShaderFromCompiledFile("../Assets/Shaders/ViewGBuffer_VS.cso");
	pViewGBuffer->CreatePixelShaderFromCompiledFile("../Assets/Shaders/ViewGBuffer_PS.cso");
	pViewGBuffer->CreateInputLayout("../Assets/Shaders/ViewGBuffer_VS.cso", eVERTEX_POSTEX2D);
	ShaderTechnique* pGBufferTechnique = new ShaderTechnique;
	pGBufferTechnique->AddPass(pViewGBuffer);
	m_pViewGBuffer->SetShaderTechnique(pGBufferTechnique);

	pView = new RenderShape;
	pView->SetMesh(Renderer::m_pMeshDatabase->CreateScreenQuadTex(std::string("GBuffer View Quad"), -1.0f, 1.0f, 1.0f, -1.0f));
	pView->SetContext(m_pViewGBuffer);
	pView->SetMaterial(new Material);
	m_pViewGBuffer->GetRenderSet()->AddNode(pView);
}

void GBuffer::RenderGBuffers(void)
{
	ID3D11ShaderResourceView* pGBufferTextures[] = 
	{
		m_pGBufferTargets[DIFFUSE]->GetSRV(),
		m_pGBufferTargets[SPECULAR]->GetSRV(),
		m_pGBufferTargets[NORMAL]->GetSRV(),
		m_pGBufferTargets[DEPTH]->GetSRV(),
		m_pGBufferTargets[EMISSIVE]->GetSRV(),
	};
	Renderer::m_pImmediateContext->PSSetShaderResources(9, sizeof(pGBufferTextures) / sizeof(pGBufferTextures[0]), pGBufferTextures);

	static bool bFirst = true;
	if (bFirst)
	{
		SetViewGBuffer(DIFFUSE);
		bFirst = false;
	}
	m_pViewGBuffer->RenderProcess();
}

void GBuffer::SetViewGBuffer(GBufferTarget eTarget)
{
	if (eTarget != BUFFER_COUNT)
	{
		cbRenderOptions* tRenderOptions = Renderer::m_pRenderOptionsCBuffer->MapDiscard(Renderer::m_pImmediateContext);
		Renderer::m_pRenderOptionsCBuffer->Unmap(Renderer::m_pImmediateContext);
		if (eTarget == DIFFUSE)
			Renderer::SetRenderOptionsData(1, 0, 0, 0, 0, tRenderOptions->nViewLightingOnly);
		if (eTarget == SPECULAR)					   
			Renderer::SetRenderOptionsData(0, 1, 0, 0, 0, tRenderOptions->nViewLightingOnly);
		if (eTarget == NORMAL)						   
			Renderer::SetRenderOptionsData(0, 0, 1, 0, 0, tRenderOptions->nViewLightingOnly);
		if (eTarget == DEPTH)						   
			Renderer::SetRenderOptionsData(0, 0, 0, 1, 0, tRenderOptions->nViewLightingOnly);
		if (eTarget == EMISSIVE)						   
			Renderer::SetRenderOptionsData(0, 0, 0, 0, 1, tRenderOptions->nViewLightingOnly);
	}
}