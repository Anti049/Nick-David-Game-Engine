#pragma once
#include "RenderShape.h"

struct PointLightStruct;

class RenderShapePLight : public RenderShape
{
public:
	RenderShapePLight(void);
	~RenderShapePLight(void);

	static void				IndexedPrimitiveRenderFunc(RenderNode* pNode);

	void					SetLight(PointLightStruct* pPointLight) { m_pPointLight = pPointLight; }

private:
	PointLightStruct*		m_pPointLight;
};