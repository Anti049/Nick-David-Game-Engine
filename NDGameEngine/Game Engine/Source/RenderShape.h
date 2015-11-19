#pragma once

#include "RenderNode.hpp"

class RenderMesh;
class Material;

class RenderShape : public RenderNode
{
public:
	RenderShape(void);
	~RenderShape(void);

	static void				IndexedPrimitiveRenderFunc(RenderNode* pNode);
	static void				ParticleRenderFunc(RenderNode* pNode);

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	RenderMesh*				GetMesh(void)									{ return m_pMesh; }
	Matrix					GetWorldMatrix(void)							{ return m_mWorldMatrix; }
	Material*				GetMaterial(void)								{ return m_pMaterial; }

	//////////////////////////////////////////////////////////////////////////
	// Mutators
	void					SetMesh(RenderMesh* pMesh)						{ m_pMesh = pMesh; }
	void					SetWorldMatrix(Matrix mWorld)					{ m_mWorldMatrix = mWorld; }
	void					SetMaterial(Material* pMaterial)				{ m_pMaterial = pMaterial; }

private:
	RenderMesh*				m_pMesh;
	Matrix					m_mWorldMatrix;
	Material*				m_pMaterial;
};