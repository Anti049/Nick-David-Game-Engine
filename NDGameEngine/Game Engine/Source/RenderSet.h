#pragma once

class RenderNode;

class RenderSet
{
public:
	RenderSet(void);
	~RenderSet(void);

	void						Render(void);
	virtual void				AddNode(RenderNode* pNode);
	virtual void				RemoveNode(RenderNode* pNode);
	void						ClearSet(void);

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	RenderNode*				GetHeadPtr(void)							{ return m_pHead; }
	RenderNode*				GetTailPtr(void)							{ return m_pTail; }
	//////////////////////////////////////////////////////////////////////////
	// Mutators
	void						SetHeadPtr(RenderNode* pHead)				{ m_pHead = pHead; }
	void						SetTailPtr(RenderNode* pTail)				{ m_pTail = pTail; }

protected:
	RenderNode*				m_pHead;
	RenderNode*				m_pTail;
};