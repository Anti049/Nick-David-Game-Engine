#include "Precompiled.h"
#include "RenderShape.h"
#include "RenderMesh.h"
#include "RenderContext.h"
#include "Renderer.h"
#include "Material.h"
#include "Texture.h"

RenderShape::RenderShape(void)
{
	m_pMesh = nullptr;
	m_mWorldMatrix = Matrix::Identity;
	m_pMaterial = nullptr;
	SetRenderFunc(IndexedPrimitiveRenderFunc);
}

RenderShape::~RenderShape(void)
{
	m_pMesh = nullptr;
	m_mWorldMatrix = Matrix::Identity;
	SafeDelete(m_pMaterial);
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
		// Set Textures
		if (pContext)
		{
			Material* pMaterial = pShape->GetMaterial();
			if (pMaterial)
			{
				pContext->EffectsClearTextureMaps(true);
				if (pMaterial->GetAmbient())
					pContext->EffectsSetTextureMap(eAmbient, pMaterial->GetAmbient()->GetSRV());
				if (pMaterial->GetDiffuse())
					pContext->EffectsSetTextureMap(eDiffuse, pMaterial->GetDiffuse()->GetSRV());
				if (pMaterial->GetSpecular())
					pContext->EffectsSetTextureMap(eSpecular, pMaterial->GetSpecular()->GetSRV());
				if (pMaterial->GetNormal())
					pContext->EffectsSetTextureMap(eNormal, pMaterial->GetNormal()->GetSRV());
			}
		}
		// Draw Indexed
		Renderer::m_pImmediateContext->DrawIndexed(pMesh->GetNumIndices(), pMesh->GetStartIndex(), pMesh->GetStartVertex());
	}
}