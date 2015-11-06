#include "Precompiled.h"
#include "RenderShape.h"
#include "RenderMesh.h"
#include "RenderContext.h"
#include "Renderer.h"

RenderShape::RenderShape(void)
{
	m_pMesh = nullptr;
	m_mWorldMatrix = Matrix::Identity;
	SetRenderFunc(IndexedPrimitiveRenderFunc);
}

RenderShape::~RenderShape(void)
{
	m_pMesh = nullptr;
	m_mWorldMatrix = Matrix::Identity;
	SetRenderFunc(IndexedPrimitiveRenderFunc);
}

void RenderShape::IndexedPrimitiveRenderFunc(RenderNode* pNode)
{
	RenderShape* pShape = (RenderShape*)pNode;
	RenderMesh* pMesh = pShape->GetMesh();
	RenderContext* pContext = pShape->GetContext();
	if (pMesh)
	{
		// Set Per Object Data
		DirectX::SimpleMath::Matrix mWorld = pShape->GetWorldMatrix();
		DirectX::SimpleMath::Matrix mViewProjection = Renderer::m_cActiveCamera.GetViewMatrix() * Renderer::m_cActiveCamera.GetProjectionMatrix();
		DirectX::SimpleMath::Matrix mMVP = mWorld * mViewProjection;
		Renderer::SetPerObjectData(mMVP, mWorld);
		// Draw Indexed
		Renderer::m_pImmediateContext->DrawIndexed(pMesh->GetNumIndices(), pMesh->GetStartIndex(), pMesh->GetStartVertex());
	}
}