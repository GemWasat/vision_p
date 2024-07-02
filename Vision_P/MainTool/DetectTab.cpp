// DetectTab.cpp: 구현 파일
//

#include "pch.h"
#include "MainTool.h"
#include "afxdialogex.h"
#include "DetectTab.h"
#include "ToolManager.h"
#include "TestTab.h"
#include "ServerForm.h"
#include "ConnectTread.h"


// DetectTab 대화 상자

IMPLEMENT_DYNAMIC(DetectTab, CDialog)

DetectTab::DetectTab(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DetectTab, pParent)
{

}

DetectTab::~DetectTab()
{
	Threadupdate = true;
}

void DetectTab::DoDataExchange(CDataExchange* pDX)
{

	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(DetectTab, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &DetectTab::OnNMClickList1)
END_MESSAGE_MAP()


// DetectTab 메시지 처리기


BOOL DetectTab::OnInitDialog()
{
	CDialog::OnInitDialog();

	LVCOLUMN Column;

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	UpdateData();

	Column.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	Column.fmt = LVCFMT_LEFT;

	LPWSTR Column_list[5] = { _T("ID"), _T("Url") , _T("Color"),_T("Fine"),_T("Date") };
	int cx[5] = { 50,600,100,100,350 };

	int Column_size = sizeof(Column_list) / sizeof(Column_list[0]);
	for (int i = 0; i < Column_size; i++) {
		Column.cx = cx[i];
		Column.pszText = Column_list[i];
		m_List.InsertColumn(i, &Column);
	}
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}




void DetectTab::OnNMClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int clickindex = m_List.GetSelectionMark();

	ToolManager::GetInstance()->m_tab->SetCurSel(1);
	ToolManager::GetInstance()->m_detecttab->ShowWindow(SW_HIDE);
	ToolManager::GetInstance()->m_Testtab->ShowWindow(SW_SHOW);
	CString id = m_List.GetItemText(clickindex, 0);
	CString url = m_List.GetItemText(clickindex, 1);
	CString Color = m_List.GetItemText(clickindex, 2);
	CString Faulty = m_List.GetItemText(clickindex, 3);
	CString Date = m_List.GetItemText(clickindex, 4);

	ToolManager::GetInstance()->m_strPickinLst = url;
	ToolManager::GetInstance()->m_Testtab->Setcnt(clickindex);

	ToolManager::GetInstance()->m_Testtab->CurId = CT2CA(id);

	ToolManager::GetInstance()->m_Testtab->Url = url;
	ToolManager::GetInstance()->m_Testtab->Color = Color;
	ToolManager::GetInstance()->m_Testtab->Faulty = Faulty;
	ToolManager::GetInstance()->m_Testtab->Date = Date;



	*pResult = 0;
}

void DetectTab::Update()
{
	vector<AWSLIST> vecInfo;
	vecInfo = ToolManager::GetInstance()->m_Serverform->GetBoxlist();

	m_List.DeleteAllItems();

	for (int i = 0; i < vecInfo.size(); ++i)
	{
		m_List.InsertItem(i, CString(vecInfo[i].id.c_str()));
		m_List.SetItemText(i, 1, CString(vecInfo[i].url.c_str()));
		m_List.SetItemText(i, 2, CString(vecInfo[i].color.c_str()));
		m_List.SetItemText(i, 3, CString(vecInfo[i].faulty.c_str()));
		m_List.SetItemText(i, 4, CString(vecInfo[i].date.c_str()));
	}


	if (vecInfo.empty())
	{
		m_List.DeleteAllItems();
	}

}

void DetectTab::Set_Text(int curidx, int idx, CString cstr)
{

	UpdateData(FALSE);
	m_List.SetItemText(curidx, idx, cstr);
	UpdateData(TRUE);

}
