#pragma once

#include "RenderNode.hpp"

class ShaderTechnique;
class RenderSet;

class RenderContext : public RenderNode
{
public:
	RenderContext(void);
	~RenderContext(void);

	static void					ContextSharedRenderFunc(RenderNode* pNode);
	static void					Context2DFlatRenderFunc(RenderNode* pNode);
	static void					Context3DFlatRenderFunc(RenderNode* pNode);

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	ShaderTechnique*		GetShaderTechnique(void)									{ return m_pShaderTechnique; }
	RenderSet*				GetRenderSet(void)											{ return m_pRenderSet; }

	//////////////////////////////////////////////////////////////////////////
	// Mutators
	void					SetShaderTechnique(ShaderTechnique* cShaderTechnique)		{ m_pShaderTechnique = cShaderTechnique; }
	void					SetRenderSet(RenderSet* pRenderSet)							{ m_pRenderSet = pRenderSet; }

private:
	ShaderTechnique*		m_pShaderTechnique;
	RenderSet*				m_pRenderSet;
};