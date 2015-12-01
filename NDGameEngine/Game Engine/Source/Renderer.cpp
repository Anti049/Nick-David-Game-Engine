#include "Precompiled.h"
#include "Renderer.h"
#include RendererPath

Renderer* Renderer::s_pInstance = nullptr;

Renderer* RendererType::GetInstance(void)
{
	if (!s_pInstance)
		s_pInstance = new RendererType;
	return s_pInstance;
}
void RendererType::DeleteInstance(void)
{
	RendererType* pInstance = (RendererType*)s_pInstance;
	SafeDelete(pInstance);
}