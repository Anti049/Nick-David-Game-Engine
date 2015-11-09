#pragma once

#include "RenderNode.hpp"

class ShaderTechnique;
class RenderSet;

class RenderContext : public RenderNode
{
public:
	RenderContext(void);
	~RenderContext(void);

	static void				ContextSharedRenderFunc(RenderNode* pNode);
	static void				Context2DFlatRenderFunc(RenderNode* pNode);
	static void				Context3DFlatRenderFunc(RenderNode* pNode);
	static void				Context2DTextureRenderFunc(RenderNode* pNode);
	static void				Context3DTextureRenderFunc(RenderNode* pNode);

	void					EffectsSetTextureMap(RenderTextureTypes eTextureType, ID3D11ShaderResourceView* pTexture);
	void					EffectsSetTextureMapsAll(ID3D11ShaderResourceView** pTexture, unsigned int unStartIndex = eAmbient, unsigned int unCount = eCount);
	void					EffectsClearTextureMaps(bool bEnabled);

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	ShaderTechnique*		GetShaderTechnique(void)									{ return m_pShaderTechnique; }
	RenderSet*				GetRenderSet(void)											{ return m_pRenderSet; }
	bool					GetTexturesEnabled(void)									{ return m_bTexturesEnabled; }

	//////////////////////////////////////////////////////////////////////////
	// Mutators
	void					SetShaderTechnique(ShaderTechnique* cShaderTechnique)		{ m_pShaderTechnique = cShaderTechnique; }
	void					SetRenderSet(RenderSet* pRenderSet)							{ m_pRenderSet = pRenderSet; }
	void					SetTexturesEnabled(bool bTexturesEnabled)					{ m_bTexturesEnabled = bTexturesEnabled; }

private:
	ShaderTechnique*		m_pShaderTechnique;
	RenderSet*				m_pRenderSet;
	bool					m_bTexturesEnabled;
};