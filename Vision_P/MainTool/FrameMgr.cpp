#include "pch.h"
#include "FrameMgr.h"
#include "ToolManager.h"

IMPLEMENT_SINGLETON(CFrameMgr)



CFrameMgr::CFrameMgr()
{
}

CFrameMgr::~CFrameMgr()
{
	
}

void CFrameMgr::InitFrame()
{
	fFPS = 0.f;
	QueryPerformanceCounter(&m_OldTime);
	QueryPerformanceCounter(&m_CurTime);
	QueryPerformanceFrequency(&m_CPUTick);
}

double CFrameMgr::GetTimeDelta()
{
	QueryPerformanceCounter(&m_CurTime);

	if (m_CurTime.QuadPart - m_OldTime.QuadPart > m_CPUTick.QuadPart)
	{
		QueryPerformanceCounter(&m_OldTime);
		QueryPerformanceCounter(&m_CurTime);
		QueryPerformanceFrequency(&m_CPUTick);
	}

	m_Time = (m_CurTime.QuadPart - m_OldTime.QuadPart) / (double)m_CPUTick.QuadPart;
	
	m_OldTime = m_CurTime;

	return m_Time;
}

//void CFrameMgr::RenderFPS()
//{
//	if (1.f <= m_Time)
//	{
//		putText(ToolManager::GetInstance()->GetFrame(), "fps: " + to_string(fFPS), Point(10, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 2);
//		m_Time = 0;
//		fFPS = 0;
//	}
//
//}
