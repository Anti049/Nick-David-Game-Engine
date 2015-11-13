#pragma once

enum DSStates { DSS_DEFAULT, DSS_LESS_EQUAL, DSS_NO_DEPTH, DSS_DEFERRED_LIGHT_1,
	DSS_DEFERRED_LIGHT_2, DSS_LIGHT_OUTSIDE_FINAL, DSS_LIGHT_INSIDE_FINAL, DSS_COUNT };

class DepthStencilStateManager
{
public:
	DepthStencilStateManager(void);
	~DepthStencilStateManager(void);

	bool									ApplyState(DSStates eState);
	ID3D11DepthStencilState*				GetState(DSStates eState)
	{
		if (eState < DSS_COUNT)
			return m_pDepthStencilStates[eState];
		return nullptr;
	}
	DSStates								GetCurrentState(void) { return m_eCurrentState; }

private:
	void									CreateStates(void);

	ID3D11DepthStencilState*				m_pDepthStencilStates[DSS_COUNT];
	unsigned int							m_unStencilRefs[DSS_COUNT];
	DSStates								m_eCurrentState;
};