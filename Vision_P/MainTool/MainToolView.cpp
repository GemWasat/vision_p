
// MainToolView.cpp: CMainToolView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MainTool.h"
#endif

#include "MainToolDoc.h"
#include "MainToolView.h"

#include "ToolManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainToolView

IMPLEMENT_DYNCREATE(CMainToolView, CView)

BEGIN_MESSAGE_MAP(CMainToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMainToolView 생성/소멸

CMainToolView::CMainToolView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMainToolView::~CMainToolView()
{
}

BOOL CMainToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	
  


	return CView::PreCreateWindow(cs);
}

void CMainToolView::OnInitialUpdate()
{
	ToolManager::GetInstance()->SetToolviewhdl(this->GetSafeHwnd());
	ToolManager::GetInstance()->Initialize();

}


// CMainToolView 그리기

void CMainToolView::OnDraw(CDC* /*pDC*/)
{
	CMainToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

   

}


// CMainToolView 인쇄

BOOL CMainToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMainToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMainToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMainToolView 진단

#ifdef _DEBUG
void CMainToolView::AssertValid() const
{
	CView::AssertValid();
}

void CMainToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

BOOL CMainToolView::OnIdle(LONG lCount)
{

	return TRUE;
}

CMainToolDoc* CMainToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMainToolDoc)));
	return (CMainToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMainToolView 메시지 처리기
