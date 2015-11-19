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

void RenderSet::RemoveNode(RenderNode* pNode)
{
	if (m_pHead == m_pTail)
		m_pHead = m_pTail = nullptr;
	else if (pNode == m_pHead)
	{
		m_pHead = pNode->GetNextNode();
	}
	else if (pNode == m_pTail)
	{
		RenderNode* pPrev = m_pHead;
		while (pPrev != pNode)
		{
			if (pPrev->GetNextNode() == pNode)
			{
				m_pTail = pPrev;
				break;
			}
			pPrev = pPrev->GetNextNode();
		}
	}
	else
	{
		RenderNode* pPrev = m_pHead;
		while (pPrev != pNode)
		{
			if (pPrev->GetNextNode() == pNode)
			{
				pPrev->SetNextNode(pNode->GetNextNode());
				break;
			}
			pPrev = pPrev->GetNextNode();
		}
	}
	pNode->SetNextNode(nullptr);
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