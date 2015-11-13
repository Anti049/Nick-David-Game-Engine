#pragma once

enum BStates { BS_DEFAULT, BS_ALPHA, BS_ADDITIVE, BS_COUNT };

class BlendStateManager
{
public:
	BlendStateManager(void);
	~BlendStateManager(void);

	bool									ApplyState(BStates eState);
	ID3D11BlendState*						GetState(BStates eState)
	{
		if (eState < BS_COUNT)
			return m_pBlendStates[eState];
		return nullptr;
	}
	BStates									GetCurrentState(void) { return m_eCurrentState; }

private:
	void									CreateStates(void);

	ID3D11BlendState*						m_pBlendStates[BS_COUNT];
	BStates									m_eCurrentState;
};