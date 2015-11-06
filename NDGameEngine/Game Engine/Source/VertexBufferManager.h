#pragma once

#include "VertexBuffer.hpp"

class VertexBufferManager
{
public:
	static VertexBufferManager*				GetInstance(void)
	{
		if (!s_pInstance)
			s_pInstance = new VertexBufferManager;
		return s_pInstance;
	}
	static void									DeleteInstance(void)
	{
		SafeDelete(s_pInstance);
	}

	template <typename VertexFormat>
	VertexBuffer<VertexFormat>&					GetVertexBuffer(void);

private:
	static VertexBufferManager*					s_pInstance;

	VertexBufferManager(void);
	~VertexBufferManager(void);

	VertexBuffer<VERTEX_POSITION>				m_pPositionBuffer;
	VertexBuffer<VERTEX_POSCOLOR>				m_pPosColorBuffer;

	VertexBuffer<VERTEX_POSITION>&				GetPositionBuffer(void)				{ return m_pPositionBuffer; }
	VertexBuffer<VERTEX_POSCOLOR>&				GetPosColorBuffer(void)				{ return m_pPosColorBuffer; }
};