#include "Precompiled.h"
#include "RenderContext.h"
#include "Renderer.h"
#include "VertexBufferManager.h"
#include "IndexBuffer.h"
#include "ShaderTechnique.h"
#include "ShaderPass.h"
#include "RenderSet.h"
#include "LightManager.h"
#include "BlendStateManager.h"
#include "DepthStencilStateManager.h"
#include "GBuffer.h"

RenderContext::RenderContext(void)
{
	m_pShaderTechnique = nullptr;
	m_pRenderSet = nullptr;
}

RenderContext::~RenderContext(void)
{
	SafeDelete(m_pShaderTechnique);
	SafeDelete(m_pRenderSet);
}

void RenderContext::EffectsSetTextureMap(RenderTextureTypes eTextureType, ID3D11ShaderResourceView* pTexture)
{
	if (m_bTexturesEnabled)
	{
		Renderer::m_pImmediateContext->PSSetShaderResources(eTextureType, 1, &pTexture);
		Renderer::m_pImmediateContext->DSSetShaderResources(eTextureType, 1, &pTexture);
	}
}

void RenderContext::EffectsSetTextureMapsAll(ID3D11ShaderResourceView** pTexture, unsigned int unStartIndex, unsigned int unCount)
{
	if (m_bTexturesEnabled)
	{
		Renderer::m_pImmediateContext->PSSetShaderResources(unStartIndex, unCount, &pTexture[0]);
		Renderer::m_pImmediateContext->DSSetShaderResources(unStartIndex, unCount, &pTexture[0]);
	}
}

void RenderContext::EffectsClearTextureMaps(bool bEnabled)
{
	ID3D11ShaderResourceView** pTextures = new ID3D11ShaderResourceView*[eCount];
	for (int i = 0; i < eCount; i++)
		pTextures[i] = nullptr;

	m_bTexturesEnabled = true;
	EffectsSetTextureMapsAll(pTextures);
	m_bTexturesEnabled = bEnabled;
	SafeDelete(pTextures);
}

void RenderContext::ContextComputeRenderFunc(RenderNode* pNode)
{
	RenderContext* pContext = (RenderContext*)pNode;
	ShaderTechnique* pShaderTechnique = pContext->GetShaderTechnique();
	if (pShaderTechnique)
	{
		ShaderPass* pShaderPass = pShaderTechnique->GetPass(0);
		ID3D11ComputeShader* pCompute = pShaderPass->GetComputeShader();

		// Set Constant Buffers
		Renderer::SetCameraData();
		// Set Shader Resources
		ID3D11ShaderResourceView* pGBufferTextures[] = 
		{
			Renderer::m_pGBuffer->m_pGBufferTargets[DIFFUSE]->GetSRV(),
			Renderer::m_pGBuffer->m_pGBufferTargets[SPECULAR]->GetSRV(),
			Renderer::m_pGBuffer->m_pGBufferTargets[NORMAL]->GetSRV(),
			Renderer::m_pGBuffer->m_pGBufferTargets[DEPTH]->GetSRV(),
		};
		Renderer::m_pImmediateContext->CSSetShaderResources(9, sizeof(pGBufferTextures) / sizeof(pGBufferTextures[0]), pGBufferTextures);
		// Set Unordered Access Views
		Renderer::m_pImmediateContext->CSSetUnorderedAccessViews(0, 1, &Renderer::m_pUAV, NULL);
		// Set Shader
		Renderer::m_pImmediateContext->CSSetShader(pCompute, NULL, NULL);

		// Dispatch
		unsigned int unDispatchWidth = (Renderer::m_pMainRenderTarget->GetWidth() + BLOCK_SIZE - 1) / BLOCK_SIZE;
		unsigned int unDispatchHeight = (Renderer::m_pMainRenderTarget->GetHeight() + BLOCK_SIZE - 1) / BLOCK_SIZE;
		Renderer::m_pImmediateContext->Dispatch(unDispatchWidth, unDispatchHeight, 1);

		Renderer::m_pImmediateContext->CSSetShader(NULL, NULL, NULL);
		ID3D11UnorderedAccessView* pNullUAV = NULL;
		Renderer::m_pImmediateContext->CSSetUnorderedAccessViews(0, 1, &pNullUAV, NULL);
		ID3D11ShaderResourceView* pNullSRV[4] = {NULL, NULL, NULL, NULL};
		Renderer::m_pImmediateContext->CSSetShaderResources(9, sizeof(pGBufferTextures) / sizeof(pGBufferTextures[0]), pNullSRV);
	}
}

void RenderContext::ContextSharedRenderFunc(RenderNode* pNode)
{
	RenderContext* pContext = (RenderContext*)pNode;
	ShaderTechnique* pShaderTechnique = pContext->GetShaderTechnique();
	if (pShaderTechnique)
	{
		ShaderPass* pShaderPass = pShaderTechnique->GetPass(0);
		ID3D11VertexShader*		pVertex			= pShaderPass->GetVertexShader();
		ID3D11GeometryShader*	pGeometry		= pShaderPass->GetGeometryShader();
		ID3D11HullShader*		pHull			= pShaderPass->GetHullShader();
		ID3D11DomainShader*		pDomain			= pShaderPass->GetDomainShader();
		ID3D11PixelShader*		pPixel			= pShaderPass->GetPixelShader();

		// Set Index Buffer
		ID3D11Buffer* pIndexBuffer = IndexBuffer::GetInstance()->GetIndexBuffer();
		Renderer::m_pImmediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		// Set Shaders
		Renderer::m_pImmediateContext->VSSetShader(pVertex, NULL, NULL);
		Renderer::m_pImmediateContext->GSSetShader(pGeometry, NULL, NULL);
		Renderer::m_pImmediateContext->HSSetShader(pHull, NULL, NULL);
		Renderer::m_pImmediateContext->DSSetShader(pDomain, NULL, NULL);
		Renderer::m_pImmediateContext->PSSetShader(pPixel, NULL, NULL);

		if (pContext->GetRenderSet())
			pContext->GetRenderSet()->Render();
	}
}

void RenderContext::Context2DFlatRenderFunc(RenderNode* pNode)
{
	RenderContext* pContext = (RenderContext*)pNode;
	ShaderTechnique* pShaderTechnique = pContext->GetShaderTechnique();
	if (pShaderTechnique)
	{
		ShaderPass* pShaderPass = pShaderTechnique->GetPass(0);
		ID3D11InputLayout*		pInputLayout	= pShaderPass->GetInputLayout();

		// Set Input Layout
		Renderer::m_pImmediateContext->IASetInputLayout(pInputLayout);
		// Set Primitive Topology
		Renderer::m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// Set Vertex Buffer
		ID3D11Buffer* pVertexBuffer = VertexBufferManager::GetInstance()->GetVertexBuffer<VERTEX_POSCOLOR2D>().GetVertexBuffer();
		unsigned int unStride = sizeof(VERTEX_POSCOLOR2D), unOffset = 0;
		Renderer::m_pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &unStride, &unOffset);

		// Clear all Textures and Disable Textures
		pContext->SetTexturesEnabled(false);

		ContextSharedRenderFunc(pNode);
	}
}

void RenderContext::Context3DFlatRenderFunc(RenderNode* pNode)
{
	RenderContext* pContext = (RenderContext*)pNode;
	ShaderTechnique* pShaderTechnique = pContext->GetShaderTechnique();
	if (pShaderTechnique)
	{
		ShaderPass* pShaderPass = pShaderTechnique->GetPass(0);
		ID3D11InputLayout*		pInputLayout	= pShaderPass->GetInputLayout();

		// Set Input Layout
		Renderer::m_pImmediateContext->IASetInputLayout(pInputLayout);
		// Set Primitive Topology
		Renderer::m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// Set Vertex Buffer
		ID3D11Buffer* pVertexBuffer = VertexBufferManager::GetInstance()->GetVertexBuffer<VERTEX_POSCOLOR>().GetVertexBuffer();
		unsigned int unStride = sizeof(VERTEX_POSCOLOR), unOffset = 0;
		Renderer::m_pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &unStride, &unOffset);

		// Clear all Textures and Disable Textures
		pContext->SetTexturesEnabled(false);

		ContextSharedRenderFunc(pNode);
	}
}

void RenderContext::Context2DTextureRenderFunc(RenderNode* pNode)
{
	RenderContext* pContext = (RenderContext*)pNode;
	ShaderTechnique* pShaderTechnique = pContext->GetShaderTechnique();
	if (pShaderTechnique)
	{
		ShaderPass* pShaderPass = pShaderTechnique->GetPass(0);
		ID3D11InputLayout*		pInputLayout	= pShaderPass->GetInputLayout();

		// Set Input Layout
		Renderer::m_pImmediateContext->IASetInputLayout(pInputLayout);
		// Set Primitive Topology
		Renderer::m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// Set Vertex Buffer
		ID3D11Buffer* pVertexBuffer = VertexBufferManager::GetInstance()->GetVertexBuffer<VERTEX_POSTEX2D>().GetVertexBuffer();
		unsigned int unStride = sizeof(VERTEX_POSTEX2D), unOffset = 0;
		Renderer::m_pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &unStride, &unOffset);

		// Clear all Textures and Enable Textures
		pContext->SetTexturesEnabled(true);

		ContextSharedRenderFunc(pNode);
	}
}

void RenderContext::Context3DTextureRenderFunc(RenderNode* pNode)
{
	RenderContext* pContext = (RenderContext*)pNode;
	ShaderTechnique* pShaderTechnique = pContext->GetShaderTechnique();
	if (pShaderTechnique)
	{
		ShaderPass* pShaderPass = pShaderTechnique->GetPass(0);
		ID3D11InputLayout*		pInputLayout	= pShaderPass->GetInputLayout();

		// Set Input Layout
		Renderer::m_pImmediateContext->IASetInputLayout(pInputLayout);
		// Set Primitive Topology
		Renderer::m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// Set Vertex Buffer
		ID3D11Buffer* pVertexBuffer = VertexBufferManager::GetInstance()->GetVertexBuffer<VERTEX_POSTEX>().GetVertexBuffer();
		unsigned int unStride = sizeof(VERTEX_POSTEX), unOffset = 0;
		Renderer::m_pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &unStride, &unOffset);

		// Clear all Textures and Enable Textures
		pContext->SetTexturesEnabled(true);

		ContextSharedRenderFunc(pNode);
	}
}

void RenderContext::Context3DForwardLightingRenderFunc(RenderNode* pNode)
{
	RenderContext* pContext = (RenderContext*)pNode;
	ShaderTechnique* pShaderTechnique = pContext->GetShaderTechnique();
	if (pShaderTechnique)
	{
		ShaderPass* pShaderPass = pShaderTechnique->GetPass(0);
		ID3D11InputLayout*		pInputLayout	= pShaderPass->GetInputLayout();

		// Set Input Layout
		Renderer::m_pImmediateContext->IASetInputLayout(pInputLayout);
		// Set Primitive Topology
		Renderer::m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// Set Vertex Buffer
		ID3D11Buffer* pVertexBuffer = VertexBufferManager::GetInstance()->GetVertexBuffer<VERTEX_POSNORMTEX>().GetVertexBuffer();
		unsigned int unStride = sizeof(VERTEX_POSNORMTEX), unOffset = 0;
		Renderer::m_pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &unStride, &unOffset);

		Renderer::m_pLightManager->BindDirLight(0);

		// Clear all Textures and Enable Textures
		pContext->SetTexturesEnabled(true);

		ContextSharedRenderFunc(pNode);
	}
}

void RenderContext::ContextGBufferRenderFunc(RenderNode* pNode)
{
	RenderContext* pContext = (RenderContext*)pNode;
	ShaderTechnique* pShaderTechnique = pContext->GetShaderTechnique();
	if (pShaderTechnique)
	{
		ShaderPass* pShaderPass = pShaderTechnique->GetPass(0);
		ID3D11InputLayout*		pInputLayout	= pShaderPass->GetInputLayout();

		// Set Input Layout
		Renderer::m_pImmediateContext->IASetInputLayout(pInputLayout);
		// Set Primitive Topology
		Renderer::m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// Set Vertex Buffer
		ID3D11Buffer* pVertexBuffer = VertexBufferManager::GetInstance()->GetVertexBuffer<VERTEX_POSNORMTEX>().GetVertexBuffer();
		unsigned int unStride = sizeof(VERTEX_POSNORMTEX), unOffset = 0;
		Renderer::m_pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &unStride, &unOffset);

		// Clear all Textures and Enable Textures
		pContext->SetTexturesEnabled(true);

		Renderer::SetCameraData();

		ContextSharedRenderFunc(pNode);
	}
}

void RenderContext::ContextParticleRenderFunc(RenderNode* pNode)
{
	RenderContext* pContext = (RenderContext*)pNode;
	ShaderTechnique* pShaderTechnique = pContext->GetShaderTechnique();
	if (pShaderTechnique)
	{
		ShaderPass* pShaderPass = pShaderTechnique->GetPass(0);
		ID3D11InputLayout*		pInputLayout	= pShaderPass->GetInputLayout();

		// Set Input Layout
		Renderer::m_pImmediateContext->IASetInputLayout(pInputLayout);
		// Set Primitive Topology
		Renderer::m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		// Set Vertex Buffer
		ID3D11Buffer* pVertexBuffer = VertexBufferManager::GetInstance()->GetVertexBuffer<ParticleVertex>().GetVertexBuffer();
		unsigned int unStride = sizeof(ParticleVertex), unOffset = 0;
		Renderer::m_pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &unStride, &unOffset);

		// Clear all Textures and Enable Textures
		pContext->SetTexturesEnabled(true);

		Renderer::SetCameraData();

		ContextSharedRenderFunc(pNode);
	}
}

void RenderContext::ContextDRDirLightRenderFunc(RenderNode* pNode)
{
	RenderContext* pContext = (RenderContext*)pNode;
	ShaderTechnique* pShaderTechnique = pContext->GetShaderTechnique();
	if (pShaderTechnique)
	{
		ShaderPass* pShaderPass = pShaderTechnique->GetPass(0);
		ID3D11InputLayout*		pInputLayout	= pShaderPass->GetInputLayout();

		// Set Input Layout
		Renderer::m_pImmediateContext->IASetInputLayout(pInputLayout);
		// Set Primitive Topology
		Renderer::m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// Set Vertex Buffer
		ID3D11Buffer* pVertexBuffer = VertexBufferManager::GetInstance()->GetVertexBuffer<VERTEX_POSTEX2D>().GetVertexBuffer();
		unsigned int unStride = sizeof(VERTEX_POSTEX2D), unOffset = 0;
		Renderer::m_pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &unStride, &unOffset);

		// Clear all Textures and Enable Textures
		pContext->SetTexturesEnabled(true);

		Renderer::SetCameraData();

		ID3D11ShaderResourceView* pGBufferTextures[] = 
		{
			Renderer::m_pGBuffer->m_pGBufferTargets[DIFFUSE]->GetSRV(),
			Renderer::m_pGBuffer->m_pGBufferTargets[SPECULAR]->GetSRV(),
			Renderer::m_pGBuffer->m_pGBufferTargets[NORMAL]->GetSRV(),
			Renderer::m_pGBuffer->m_pGBufferTargets[DEPTH]->GetSRV(),
			Renderer::m_pGBuffer->m_pGBufferTargets[EMISSIVE]->GetSRV(),
		};
		Renderer::m_pImmediateContext->PSSetShaderResources(9, sizeof(pGBufferTextures) / sizeof(pGBufferTextures[0]), pGBufferTextures);

		// Set Depth Stencil State and Blend State
		// TOOK OUT BLEND STATE TO FIX BLENDING WITH SKYBOX
		Renderer::m_pDepthStencilStateManager->ApplyState(DSS_NO_DEPTH);
		if (Renderer::m_pLightManager->GetActiveIndex())
			Renderer::m_pBlendStateManager->ApplyState(BS_ADDITIVE);

		ContextSharedRenderFunc(pNode);
		Renderer::m_pImmediateContext->ClearDepthStencilView(Renderer::m_pMainRenderTarget->GetDSV(), D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}

void RenderContext::ContextDRPointLightRenderFunc(RenderNode* pNode)
{
	RenderContext* pContext = (RenderContext*)pNode;
	ShaderTechnique* pShaderTechnique = pContext->GetShaderTechnique();
	if (pShaderTechnique)
	{
		ShaderPass* pShaderPass = pShaderTechnique->GetPass(0);
		ID3D11InputLayout*		pInputLayout	= pShaderPass->GetInputLayout();

		// Set Input Layout
		Renderer::m_pImmediateContext->IASetInputLayout(pInputLayout);
		// Set Primitive Topology
		Renderer::m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// Set Vertex Buffer
		ID3D11Buffer* pVertexBuffer = VertexBufferManager::GetInstance()->GetVertexBuffer<VERTEX_POSITION>().GetVertexBuffer();
		unsigned int unStride = sizeof(VERTEX_POSITION), unOffset = 0;
		Renderer::m_pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &unStride, &unOffset);

		// Clear all Textures and Enable Textures
		pContext->SetTexturesEnabled(true);

		Renderer::SetCameraData();

		ID3D11ShaderResourceView* pGBufferTextures[] = 
		{
			Renderer::m_pGBuffer->m_pGBufferTargets[DIFFUSE]->GetSRV(),
			Renderer::m_pGBuffer->m_pGBufferTargets[SPECULAR]->GetSRV(),
			Renderer::m_pGBuffer->m_pGBufferTargets[NORMAL]->GetSRV(),
			Renderer::m_pGBuffer->m_pGBufferTargets[DEPTH]->GetSRV(),
		};
		Renderer::m_pImmediateContext->PSSetShaderResources(9, sizeof(pGBufferTextures) / sizeof(pGBufferTextures[0]), pGBufferTextures);

		Renderer::m_pDepthStencilStateManager->ApplyState(DSS_NO_DEPTH);
		Renderer::m_pBlendStateManager->ApplyState(BS_ADDITIVE);

		ContextSharedRenderFunc(pNode);
		Renderer::m_pImmediateContext->ClearDepthStencilView(Renderer::m_pMainRenderTarget->GetDSV(), D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}