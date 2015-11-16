#pragma once

#include "VertexFormats.h"

class Material;

class Particle
{
public:
	Particle(void);
	~Particle(void);

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	Material*				GetMaterial(void)								{ return m_pMaterial; }
	VERTEX_POSITION			GetVertex(void)									{ return m_vVertex; }

	//////////////////////////////////////////////////////////////////////////
	// Mutators
	void					SetMaterial(Material* pMaterial)				{ m_pMaterial = pMaterial; }
	void					SetVertex(VERTEX_POSITION vVertex)				{ m_vVertex = vVertex; }

private:
	Material*				m_pMaterial;
	VERTEX_POSITION			m_vVertex;
};