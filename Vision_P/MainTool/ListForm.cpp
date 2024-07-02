// ListForm.cpp: 구현 파일
//

#include "pch.h"
#include "MainTool.h"
#include "ListForm.h"
#include "ToolManager.h"

// ListForm

IMPLEMENT_DYNCREATE(ListForm, CFormView)

ListForm::ListForm()
	: CFormView(IDD_ListForm)
{

}

ListForm::~ListForm()
{
	delete m_dlg;
}

void ListForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

void ListForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();




}

BEGIN_MESSAGE_MAP(ListForm, CFormView)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// ListForm 진단

#ifdef _DEBUG
void ListForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void ListForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// ListForm 메시지 처리기






int ListForm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_dlg = new DataInquiryDlg;
	m_dlg->Create(IDD_DataInquiry, this);
	m_dlg->ShowWindow(SW_SHOW);





	return 0;
}


void ListForm::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	m_dlg->SetWindowPos(NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOZORDER);
}
