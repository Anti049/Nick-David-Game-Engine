#include "Precompiled.h"
#include "RenderShapePLight.h"
#include "LightManager.h"
#include "RenderMesh.h"
#include "RenderContext.h"
#include "Renderer.h"
#include "ShaderTechnique.h"
#include "ShaderPass.h"
#include "DepthStencilStateManager.h"
#include "RasterizerStateManager.h"
#include "Material.h"
#include "Texture.h"
#include "RenderTarget.h"

RenderShapePLight::RenderShapePLight(void)
{
	m_pPointLight = nullptr;
	SetRenderFunc(RenderShapePLight::IndexedPrimitiveRenderFunc);
}

RenderShapePLight::~RenderShapePLight(void)
{
	m_pPointLight = nullptr;
}

void RenderShapePLight::IndexedPrimitiveRenderFunc(RenderNode* pNode)
{
	RenderShape* pShape = (RenderShape*)pNode;
	RenderMesh* pMesh = pShape->GetMesh();
	RenderContext* pContext = pShape->GetContext();
	if (pMesh)
	{
		Renderer::BeginEvent(0, L"Begin Point Light");
		// Set Per Object Data
		PointLightStruct* pPointLight = Renderer::m_pLightManager->GetPointLight(Renderer::m_pLightManager->GetActiveIndex());
		DirectX::SimpleMath::Matrix mWorld = DirectX::SimpleMath::Matrix::CreateScale(pPointLight->fRange) * 
			DirectX::SimpleMath::Matrix::CreateTranslation(pPointLight->vPosition.x, pPointLight->vPosition.y, pPointLight->vPosition.z);
		DirectX::SimpleMath::Matrix mViewProjection = Renderer::m_cActiveCamera.GetViewMatrix() * Renderer::m_cActiveCamera.GetProjectionMatrix();
		DirectX::SimpleMath::Matrix mMVP = mWorld * mViewProjection;
		Renderer::SetPerObjectData(mMVP, mWorld, true);
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
				if (pMaterial->GetEmissive())
					pContext->EffectsSetTextureMap(eEmissive, pMaterial->GetEmissive()->GetSRV());
			}
		}

		ShaderTechnique* pShaderTechnique = pContext->GetShaderTechnique();
		unsigned int unPassCount = pShaderTechnique->GetNumPasses();
		if (unPassCount == 3)
		{
			Vector3 vToLight = mWorld.Translation() - Renderer::m_cActiveCamera.GetPosition();
			float fRange = Renderer::m_pLightManager->GetPointLight(Renderer::m_pLightManager->GetActiveIndex())->fRange;
			if (vToLight.LengthSquared() < fRange * fRange)
			{
				// Inside
				Renderer::m_pDepthStencilStateManager->ApplyState(DSS_DEFERRED_LIGHT_1);
				Renderer::m_pRasterizerStateManager->ApplyState(RS_CCW);
				Renderer::m_pImmediateContext->VSSetShader(pShaderTechnique->GetPass(0)->GetVertexShader(), NULL, 0);
				Renderer::m_pImmediateContext->PSSetShader(pShaderTechnique->GetPass(0)->GetPixelShader(), NULL, 0);
				Renderer::m_pImmediateContext->DrawIndexed(pMesh->GetNumIndices(), pMesh->GetStartIndex(), pMesh->GetStartVertex());

				Renderer::m_pDepthStencilStateManager->ApplyState(DSS_LIGHT_INSIDE_FINAL);
				Renderer::m_pImmediateContext->PSSetShader(pShaderTechnique->GetPass(2)->GetPixelShader(), NULL, 0);
				Renderer::m_pImmediateContext->DrawIndexed(pMesh->GetNumIndices(), pMesh->GetStartIndex(), pMesh->GetStartVertex());
				Renderer::m_pRasterizerStateManager->ApplyState(RS_DEFAULT);
			}
			else
			{
				// Outside
				Renderer::m_pDepthStencilStateManager->ApplyState(DSS_DEFERRED_LIGHT_1);
				Renderer::m_pRasterizerStateManager->ApplyState(RS_CCW);
				Renderer::m_pImmediateContext->VSSetShader(pShaderTechnique->GetPass(0)->GetVertexShader(), NULL, 0);
				Renderer::m_pImmediateContext->PSSetShader(pShaderTechnique->GetPass(0)->GetPixelShader(), NULL, 0);
				Renderer::m_pImmediateContext->DrawIndexed(pMesh->GetNumIndices(), pMesh->GetStartIndex(), pMesh->GetStartVertex());

				Renderer::m_pDepthStencilStateManager->ApplyState(DSS_DEFERRED_LIGHT_2);
				Renderer::m_pRasterizerStateManager->ApplyState(RS_DEFAULT);
				Renderer::m_pImmediateContext->DrawIndexed(pMesh->GetNumIndices(), pMesh->GetStartIndex(), pMesh->GetStartVertex());

				Renderer::m_pDepthStencilStateManager->ApplyState(DSS_LIGHT_OUTSIDE_FINAL);
				Renderer::m_pImmediateContext->PSSetShader(pShaderTechnique->GetPass(2)->GetPixelShader(), NULL, 0);
				Renderer::m_pImmediateContext->DrawIndexed(pMesh->GetNumIndices(), pMesh->GetStartIndex(), pMesh->GetStartVertex());
			}
		}
		Renderer::m_pImmediateContext->ClearDepthStencilView(Renderer::m_pMainRenderTarget->GetDSV(), D3D11_CLEAR_STENCIL, 1.0f, 0);
		Renderer::EndEvent();
	}
}