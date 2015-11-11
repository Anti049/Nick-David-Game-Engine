#include "Precompiled.h"
#include "LightManager.h"
#include "LightBuffers.h"
#include "Renderer.h"

LightManager::LightManager(void)
{
	m_pDirLights.clear();
	m_pPointLights.clear();
	m_pSpotLights.clear();
	m_unActiveIndex = 0;
}

LightManager::~LightManager(void)
{
	for (unsigned int i = 0; i < m_pDirLights.size(); i++)
		SafeDelete(m_pDirLights[i]);
	for (unsigned int i = 0; i < m_pPointLights.size(); i++)
		SafeDelete(m_pPointLights[i]);
	for (unsigned int i = 0; i < m_pSpotLights.size(); i++)
		SafeDelete(m_pSpotLights[i]);

	m_pDirLights.clear();
	m_pPointLights.clear();
	m_pSpotLights.clear();
	m_unActiveIndex = 0;
}

void LightManager::BindDirLight(unsigned int unIndex)
{
	if (unIndex < m_pDirLights.size())
		Renderer::SetDirLightData(m_pDirLights[unIndex]);
}

void LightManager::BindPointLight(unsigned int unIndex)
{
	if (unIndex < m_pPointLights.size())
		Renderer::SetPointLightData(m_pPointLights[unIndex]);
}

void LightManager::BindSpotLight(unsigned int unIndex)
{
	if (unIndex < m_pSpotLights.size())
		Renderer::SetSpotLightData(m_pSpotLights[unIndex]);
}