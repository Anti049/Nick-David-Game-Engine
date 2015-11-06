#include "Precompiled.h"
#include "RenderSet.h"
#include "RenderNode.hpp"

RenderSet::RenderSet(void)
{
	m_pHead = m_pTail = nullptr;
}


RenderSet::~RenderSet(void)
{
	ClearSet();
}

void RenderSet::Render(void)
{
	RenderNode* pNode = m_pHead;
	while (pNode)
	{
		pNode->RenderProcess();
		//pNode->m_pShape->Draw();
		pNode = pNode->GetNextNode();
	}
}

void RenderSet::AddNode(RenderNode* pNode)
{
	if (m_pTail)
	{
		m_pTail->SetNextNode(pNode);
		m_pTail = pNode;
	}
	else
		m_pHead = m_pTail = pNode;
}

void RenderSet::ClearSet(void)
{
	RenderNode* pNode = m_pHead;
	while (pNode)
	{
		RenderNode* pPrev = pNode;
		pNode = pNode->GetNextNode();
		SafeDelete(pPrev);
	}
	m_pHead = m_pTail = nullptr;
}