#pragma once

#include "RenderTarget.h"

enum GBufferTarget { DIFFUSE, SPECULAR, NORMAL, DEPTH, EMISSIVE, BUFFER_COUNT };
class RenderContext;
class Texture2D;

class GBuffer
{
public:
	GBuffer(void);
	~GBuffer(void);

	void					Bind(void);
	void					Unbind(void);
	void					Initialize(unsigned int unWidth, unsigned int unHeight);
	void					RenderGBuffers(void);

	void					SetViewGBuffer(GBufferTarget eTarget);

	RenderTarget*			m_pRenderTarget;
	RenderSurface*			m_pGBufferTargets[BUFFER_COUNT];
	RenderContext*			m_pViewGBuffer;
	GBufferTarget			m_eActiveBufferVisual;
};