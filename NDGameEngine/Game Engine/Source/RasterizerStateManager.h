#pragma once

enum RStates { RS_DEFAULT, RS_LINE, RS_CCW, RS_NOCULL, RS_COUNT };

class RasterizerStateManager
{
public:
	RasterizerStateManager(void);
	~RasterizerStateManager(void);

	bool									ApplyState(RStates eState);
	ID3D11RasterizerState*					GetState(RStates eState)
	{
		if (eState < RS_COUNT)
			return m_pRasterizerStates[eState];
		return nullptr;
	}
	RStates									GetCurrentState(void) { return m_eCurrentState; }

private:
	void									CreateStates(void);

	ID3D11RasterizerState*					m_pRasterizerStates[RS_COUNT];
	unsigned int							m_unStencilRefs[RS_COUNT];
	RStates									m_eCurrentState;
};