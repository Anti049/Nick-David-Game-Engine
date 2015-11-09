#pragma once

class FPS
{
public:
	FPS(void);
	~FPS(void);

	void				Initialize(void);
	unsigned int		Get(void);

private:
	unsigned int		m_unFPS;
	unsigned int		m_unFPSCount;
	unsigned int		m_unInitialTime;
};