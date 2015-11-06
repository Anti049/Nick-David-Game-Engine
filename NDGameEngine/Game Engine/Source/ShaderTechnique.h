#pragma once

class ShaderPass;

class ShaderTechnique
{
public:
	ShaderTechnique(void);
	~ShaderTechnique(void);

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	ShaderPass*					GetPass(unsigned int unPass)
	{
		if (unPass < m_pShaderPasses.size())
			return m_pShaderPasses[unPass];
		return nullptr;
	}
	std::string&					GetName(void) { return m_szTechniqueName; }
	unsigned int					GetNumPasses(void) { return (unsigned int)m_pShaderPasses.size(); }
	//////////////////////////////////////////////////////////////////////////
	// Mutators
	void							AddPass(ShaderPass* pShaderPass) { m_pShaderPasses.push_back(pShaderPass); }
	void							SetName(std::string& szTechniqueName) { m_szTechniqueName = szTechniqueName; }

private:
	std::vector<ShaderPass*>		m_pShaderPasses;
	std::string						m_szTechniqueName;
};