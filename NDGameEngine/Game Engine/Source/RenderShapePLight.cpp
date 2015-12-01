#include "Precompiled.h"
#include "RenderShapePLight.h"
#include "LightManager.h"
#include "RenderMesh.h"
#include "RenderContext.h"
#include RendererPath
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
		RendererType::BeginEvent(0, L"Begin Point Light");
		// Set Per Object Data
		PointLightStruct* pPointLight = RendererType::m_pLightManager->GetPointLight(RendererType::m_pLightManager->GetActiveIndex());
		DirectX::SimpleMath::Matrix mWorld = DirectX::SimpleMath::Matrix::CreateScale(pPointLight->fRange) * 
			DirectX::SimpleMath::Matrix::CreateTranslation(pPointLight->vPosition.x, pPointLight->vPosition.y, pPointLight->vPosition.z);
		DirectX::SimpleMath::Matrix mViewProjection = RendererType::m_cActiveCamera.GetViewMatrix() * RendererType::m_cActiveCamera.GetProjectionMatrix();
		DirectX::SimpleMath::Matrix mMVP = mWorld * mViewProjection;
		RendererType::SetPerObjectData(mMVP, mWorld, true);
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
			Vector3 vToLight = mWorld.Translation() - RendererType::m_cActiveCamera.GetPosition();
			float fRange = RendererType::m_pLightManager->GetPointLight(RendererType::m_pLightManager->GetActiveIndex())->fRange;
			if (vToLight.LengthSquared() < fRange * fRange)
			{
				// Inside
				RendererType::m_pDepthStencilStateManager->ApplyState(DSS_DEFERRED_LIGHT_1);
				RendererType::m_pRasterizerStateManager->ApplyState(RS_CCW);
				RendererType::m_pImmediateContext->VSSetShader(pShaderTechnique->GetPass(0)->GetVertexShader(), NULL, 0);
				RendererType::m_pImmediateContext->PSSetShader(pShaderTechnique->GetPass(0)->GetPixelShader(), NULL, 0);
				RendererType::m_pImmediateContext->DrawIndexed(pMesh->GetNumIndices(), pMesh->GetStartIndex(), pMesh->GetStartVertex());

				RendererType::m_pDepthStencilStateManager->ApplyState(DSS_LIGHT_INSIDE_FINAL);
				RendererType::m_pImmediateContext->PSSetShader(pShaderTechnique->GetPass(2)->GetPixelShader(), NULL, 0);
				RendererType::m_pImmediateContext->DrawIndexed(pMesh->GetNumIndices(), pMesh->GetStartIndex(), pMesh->GetStartVertex());
				RendererType::m_pRasterizerStateManager->ApplyState(RS_DEFAULT);
			}
			else
			{
				// Outside
				RendererType::m_pDepthStencilStateManager->ApplyState(DSS_DEFERRED_LIGHT_1);
				RendererType::m_pRasterizerStateManager->ApplyState(RS_CCW);
				RendererType::m_pImmediateContext->VSSetShader(pShaderTechnique->GetPass(0)->GetVertexShader(), NULL, 0);
				RendererType::m_pImmediateContext->PSSetShader(pShaderTechnique->GetPass(0)->GetPixelShader(), NULL, 0);
				RendererType::m_pImmediateContext->DrawIndexed(pMesh->GetNumIndices(), pMesh->GetStartIndex(), pMesh->GetStartVertex());

				RendererType::m_pDepthStencilStateManager->ApplyState(DSS_DEFERRED_LIGHT_2);
				RendererType::m_pRasterizerStateManager->ApplyState(RS_DEFAULT);
				RendererType::m_pImmediateContext->DrawIndexed(pMesh->GetNumIndices(), pMesh->GetStartIndex(), pMesh->GetStartVertex());

				RendererType::m_pDepthStencilStateManager->ApplyState(DSS_LIGHT_OUTSIDE_FINAL);
				RendererType::m_pImmediateContext->PSSetShader(pShaderTechnique->GetPass(2)->GetPixelShader(), NULL, 0);
				RendererType::m_pImmediateContext->DrawIndexed(pMesh->GetNumIndices(), pMesh->GetStartIndex(), pMesh->GetStartVertex());
			}
		}
		RendererType::m_pImmediateContext->ClearDepthStencilView(RendererType::m_pMainRenderTarget->GetDSV(), D3D11_CLEAR_STENCIL, 1.0f, 0);
		RendererType::EndEvent();
	}
}