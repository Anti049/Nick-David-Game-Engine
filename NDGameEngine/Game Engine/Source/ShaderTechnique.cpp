#include "Precompiled.h"
#include "ShaderTechnique.h"
#include "ShaderPass.h"

ShaderTechnique::ShaderTechnique(void)
{
	m_szTechniqueName = "Default";
}

ShaderTechnique::~ShaderTechnique(void)
{
	for (unsigned int i = 0; i < m_pShaderPasses.size(); i++)
		SafeDelete(m_pShaderPasses[i]);
	m_pShaderPasses.clear();
}