#pragma once

class IndexBuffer
{
public:
	static IndexBuffer*			GetInstance(void)
	{
		if (!s_pInstance)
			s_pInstance = new IndexBuffer;
		return s_pInstance;
	}
	static void					DeleteInstance(void)
	{
		SafeDelete(s_pInstance);
	};

	unsigned int				AddIndices(unsigned int* pIndices, unsigned int unIndices);

	ID3D11Buffer*				GetIndexBuffer(void) { return m_pIndexBuffer; }

private:
	static IndexBuffer*			s_pInstance;
	IndexBuffer(void);
	~IndexBuffer(void);

	ID3D11Buffer*				m_pIndexBuffer;
};