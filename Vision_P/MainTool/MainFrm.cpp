
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "MainTool.h"

#include "MainFrm.h"
#include "MainToolView.h"
#include "ToolManager.h"
#include "ListForm.h"
#include "ServerForm.h"
#include "ResultForm.h"
#include "CntrlForm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame() noexcept
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
	ToolManager::GetInstance()->SetKillFrm(true);
	ToolManager::GetInstance()->SetForceQuit(true);

}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int t = 0;

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{

	m_MainSplitt.CreateStatic(this, 2, 2);
	m_SecSplitt.CreateStatic(&m_MainSplitt, 1, 2,
		WS_CHILD | WS_VISIBLE , m_MainSplitt.IdFromRowCol(0, 0));


	m_MainSplitt.CreateView(0,1, RUNTIME_CLASS(ServerForm), CSize(550, 480), pContext);
	m_MainSplitt.CreateView(1, 0, RUNTIME_CLASS(ListForm), CSize(1220, 450), pContext);
	m_MainSplitt.CreateView(1,1, RUNTIME_CLASS(CntrlForm), CSize(620, 450), pContext);

	m_SecSplitt.CreateView(0, 0, RUNTIME_CLASS(CMainToolView), CSize(640, 480), pContext);
	m_SecSplitt.CreateView(0, 1, RUNTIME_CLASS(ResultForm), CSize(1220-640, 480), pContext);



	return TRUE;
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{

	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.cx = GetSystemMetrics(SM_CXSCREEN);;
	cs.cy = GetSystemMetrics(SM_CYSCREEN);;


	cs.style &= ~WS_THICKFRAME;
	cs.style &= ~FWS_ADDTOTITLE;
	cs.hMenu = NULL;

	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}















#endif //_DEBUG



// CMainFrame 메시지 처리기

