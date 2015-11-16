#pragma once

class RenderContext;

enum RenderTextureTypes { eAmbient, eDiffuse, eSpecular, eEmissive, eNormal, eReflect, eCube, eDSS = 13, eCount = 8 };

class RenderNode
{
public:
	typedef void (*RenderFunc)(RenderNode* pNode);

	RenderNode(void) : m_pRenderFunc(nullptr), m_pNextNode(nullptr), m_pContext(nullptr) {}
	virtual ~RenderNode(void) {}

	inline void			RenderProcess(void)
	{
		if (m_pRenderFunc)
			m_pRenderFunc(this);
	}

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	RenderFunc			GetRenderFunc(void)						{ return m_pRenderFunc; }
	RenderNode*			GetNextNode(void)						{ return m_pNextNode; }
	RenderContext*		GetContext(void)						{ return m_pContext; }

	//////////////////////////////////////////////////////////////////////////
	// Mutators
	void				SetRenderFunc(RenderFunc pFunc)			{ m_pRenderFunc = pFunc; }
	void				SetNextNode(RenderNode* pNext)			{ m_pNextNode = pNext; }
	void				SetContext(RenderContext* pContext)		{ m_pContext = pContext; }

private:
	RenderFunc			m_pRenderFunc;
	RenderNode*			m_pNextNode;
	RenderContext*		m_pContext;
};