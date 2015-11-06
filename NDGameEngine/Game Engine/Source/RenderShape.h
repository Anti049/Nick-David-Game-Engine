#pragma once

#include "RenderNode.hpp"

class RenderMesh;

class RenderShape : public RenderNode
{
public:
	RenderShape(void);
	~RenderShape(void);

	static void				IndexedPrimitiveRenderFunc(RenderNode* pNode);

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	RenderMesh*				GetMesh(void)									{ return m_pMesh; }
	Matrix					GetWorldMatrix(void)							{ return m_mWorldMatrix; }

	//////////////////////////////////////////////////////////////////////////
	// Mutators
	void					SetMesh(RenderMesh* pMesh)						{ m_pMesh = pMesh; }
	void					SetWorldMatrix(Matrix mWorld)					{ m_mWorldMatrix = mWorld; }

private:
	RenderMesh*				m_pMesh;
	Matrix					m_mWorldMatrix;
};