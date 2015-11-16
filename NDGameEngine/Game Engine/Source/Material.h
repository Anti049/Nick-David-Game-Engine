#pragma once

class Texture;

class Material
{
public:
	Material(void);
	Material(Texture* pAmbient, Texture* pDiffuse, Texture* pSpecular, Texture* pEmissive, Texture* pNormal, Texture* pReflect, Texture* pCube);
	~Material(void);

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	Texture*			GetAmbient(void)						{ return m_pAmbient; }
	Texture*			GetDiffuse(void)						{ return m_pDiffuse; }
	Texture*			GetSpecular(void)						{ return m_pSpecular; }
	Texture*			GetEmissive(void)						{ return m_pEmissive; }
	Texture*			GetNormal(void)							{ return m_pNormal; }
	Texture*			GetReflect(void)						{ return m_pReflect; }
	Texture*			GetCube(void)							{ return m_pCube; }
	Texture*			GetDSS(void)							{ return m_pDSS; }

	//////////////////////////////////////////////////////////////////////////
	// Mutators
	void				SetAmbient(Texture* pTexture)			{ m_pAmbient = pTexture; }
	void				SetDiffuse(Texture* pTexture)			{ m_pDiffuse = pTexture; }
	void				SetSpecular(Texture* pTexture)			{ m_pSpecular = pTexture; }
	void				SetEmissive(Texture* pTexture)			{ m_pEmissive = pTexture; }
	void				SetNormal(Texture* pTexture)			{ m_pNormal = pTexture; }
	void				SetReflect(Texture* pTexture)			{ m_pReflect = pTexture; }
	void				SetCube(Texture* pTexture)				{ m_pCube = pTexture; }
	void				SetDSS(Texture* pTexture)				{ m_pDSS = pTexture; }

private:
	Texture*			m_pAmbient;
	Texture*			m_pDiffuse;
	Texture*			m_pSpecular;
	Texture*			m_pEmissive;
	Texture*			m_pNormal;
	Texture*			m_pReflect;
	Texture*			m_pCube;
	Texture*			m_pDSS;
};