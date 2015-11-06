#include "Precompiled.h"
#include "RenderContext.h"
#include "Renderer.h"
#include "VertexBufferManager.h"
#include "IndexBuffer.h"
#include "ShaderTechnique.h"
#include "ShaderPass.h"
#include "RenderSet.h"

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

		ContextSharedRenderFunc(pNode);
	}
}