#pragma once

struct DirLightStruct;
struct PointLightStruct;
struct SpotLightStruct;

class LightManager
{
public:
	LightManager(void);
	~LightManager(void);

	void										BindDirLight(unsigned int unIndex);
	void										BindPointLight(unsigned int unIndex);
	void										BindSpotLight(unsigned int unIndex);

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	vector<DirLightStruct*>						GetDirLights(void)									{ return m_pDirLights; }
	vector<PointLightStruct*>					GetPointLights(void)								{ return m_pPointLights; }
	vector<SpotLightStruct*>					GetSpotLights(void)									{ return m_pSpotLights; }
	DirLightStruct*								GetDirLight(unsigned int unIndex)
	{
		if (unIndex < m_pDirLights.size())
			return m_pDirLights[unIndex];
		return nullptr;
	}
	unsigned int								GetNumDirLights(void)								{ return (unsigned int)m_pDirLights.size(); }
	PointLightStruct*							GetPointLight(unsigned int unIndex)
	{
		if (unIndex < m_pPointLights.size())
			return m_pPointLights[unIndex];
		return nullptr;
	}
	unsigned int								GetNumPointLights(void)								{ return (unsigned int)m_pPointLights.size(); }
	SpotLightStruct*							GetSpotLight(unsigned int unIndex)
	{
		if (unIndex < m_pSpotLights.size())
			return m_pSpotLights[unIndex];
		return nullptr;
	}
	unsigned int								GetNumSpotLights(void)								{ return (unsigned int)m_pSpotLights.size(); }
	unsigned int								GetActiveIndex(void)								{ return m_unActiveIndex; }
	//////////////////////////////////////////////////////////////////////////
	// Mutators
	void										AddDirLight(DirLightStruct* pDirLight)				{ m_pDirLights.push_back(pDirLight); }
	void										AddPointLight(PointLightStruct* pPointLight)		{ m_pPointLights.push_back(pPointLight); }
	void										AddSpotLight(SpotLightStruct* pSpotLight)			{ m_pSpotLights.push_back(pSpotLight); }
	void										SetActiveIndex(unsigned int unActiveIndex)			{ m_unActiveIndex = unActiveIndex; }

private:
	vector<DirLightStruct*>						m_pDirLights;
	vector<PointLightStruct*>					m_pPointLights;
	vector<SpotLightStruct*>					m_pSpotLights;

	unsigned int								m_unActiveIndex;
};