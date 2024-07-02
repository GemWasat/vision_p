#pragma once
class CFrameMgr
{
	DECLARE_SINGLETON(CFrameMgr)

public:
	void InitFrame();
	double GetTimeDelta();
	//void RenderFPS();

private:
	double fFPS;
	double m_Time;
	LARGE_INTEGER m_OldTime;
	LARGE_INTEGER m_CurTime;
	LARGE_INTEGER m_CPUTick;
};

