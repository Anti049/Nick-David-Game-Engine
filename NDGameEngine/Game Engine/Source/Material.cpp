#include "Precompiled.h"
#include "Material.h"
#include "Texture.h"
#include "Renderer.h"
#include "TextureDatabase.h"

Material::Material(void)
{
	m_pAmbient = nullptr;
	m_pDiffuse = nullptr;
	m_pSpecular = nullptr;
	m_pEmissive = nullptr;
	m_pNormal = nullptr;
	m_pReflect = nullptr;
	m_pCube = nullptr;
}

Material::Material(Texture* pAmbient, Texture* pDiffuse, Texture* pSpecular, Texture* pEmissive, Texture* pNormal, Texture* pReflect, Texture* pCube)
{
	m_pAmbient = pAmbient;
	m_pDiffuse = pDiffuse;
	m_pSpecular = pSpecular;
	m_pEmissive = pEmissive;
	m_pNormal = pNormal;
	m_pReflect = pReflect;
	m_pCube = pCube;
}

Material::~Material(void)
{
	/*Renderer::m_pTextureDatabase->RemoveTexture(m_pAmbient);
	SafeDelete(m_pAmbient);
	Renderer::m_pTextureDatabase->RemoveTexture(m_pDiffuse);
	SafeDelete(m_pDiffuse);
	Renderer::m_pTextureDatabase->RemoveTexture(m_pSpecular);
	SafeDelete(m_pSpecular);
	Renderer::m_pTextureDatabase->RemoveTexture(m_pEmissive);
	SafeDelete(m_pEmissive);
	Renderer::m_pTextureDatabase->RemoveTexture(m_pNormal);
	SafeDelete(m_pNormal);
	Renderer::m_pTextureDatabase->RemoveTexture(m_pReflect);
	SafeDelete(m_pReflect);*/
}