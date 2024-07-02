// ColortTestDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MainTool.h"
#include "afxdialogex.h"
#include "ColortTestDlg.h"
#include "ToolManager.h"


// ColortTestDlg 대화 상자

IMPLEMENT_DYNAMIC(ColortTestDlg, CDialogEx)

ColortTestDlg::ColortTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COLORTEST, pParent)
	, grmin(0)
	, grmax(0)
	, ggmin(0)
	, ggmax(0)
	, gbmin(0)
	, gbmax(0)
	, yrmin(0)
	, yrmax(0)
	, ygmin(0)
	, ygmax(0)
	, ybmin(0)
	, ybmax(0)
{

}

ColortTestDlg::~ColortTestDlg()
{
}

void ColortTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, redmin);
	DDX_Control(pDX, IDC_EDIT2, redmax);
	DDX_Control(pDX, IDC_EDIT5, greenmin);
	DDX_Control(pDX, IDC_EDIT6, greenmax);
	DDX_Control(pDX, IDC_EDIT3, yellowmin);
	DDX_Control(pDX, IDC_EDIT4, yellowmax);
	DDX_Text(pDX, IDC_EDIT7, grmin);
	DDX_Text(pDX, IDC_EDIT8, grmax);
	DDX_Text(pDX, IDC_EDIT9, ggmin);
	DDX_Text(pDX, IDC_EDIT10, ggmax);
	DDX_Text(pDX, IDC_EDIT11, gbmin);
	DDX_Text(pDX, IDC_EDIT12, gbmax);
	DDX_Text(pDX, IDC_EDIT13, yrmin);
	DDX_Text(pDX, IDC_EDIT14, yrmax);
	DDX_Text(pDX, IDC_EDIT15, ygmin);
	DDX_Text(pDX, IDC_EDIT16, ygmax);
	DDX_Text(pDX, IDC_EDIT17, ybmin);
	DDX_Text(pDX, IDC_EDIT18, ybmax);
}


BEGIN_MESSAGE_MAP(ColortTestDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &ColortTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &ColortTestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON5, &ColortTestDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// ColortTestDlg 메시지 처리기


void ColortTestDlg::OnBnClickedButton1()
{
	CString Credmin;
	CString Credmax;

	CString Cgreenmin;
	CString Cgreenmax;

	CString Cbluewmin;
	CString Cbluewmax;

	GetDlgItemText(IDC_EDIT1, Credmin);
	GetDlgItemText(IDC_EDIT2, Credmax);
	GetDlgItemText(IDC_EDIT3, Cgreenmin);
	GetDlgItemText(IDC_EDIT4, Cgreenmax);
	GetDlgItemText(IDC_EDIT5, Cbluewmin);
	GetDlgItemText(IDC_EDIT6, Cbluewmax);


	int rmin = stoi(string(CT2CA(Credmin)));
	int rmax = stoi(string(CT2CA(Credmax)));
	
	int gmin = stoi(string(CT2CA(Cgreenmin)));
	int gmax = stoi(string(CT2CA(Cgreenmax)));

	int bmin = stoi(string(CT2CA(Cbluewmin)));
	int bmax = stoi(string(CT2CA(Cbluewmax)));

	ToolManager::GetInstance()->forcolor[0][0] = rmin;
	ToolManager::GetInstance()->forcolor[0][1] = rmax;
	ToolManager::GetInstance()->forcolor[0][2] = gmin;
	ToolManager::GetInstance()->forcolor[0][3] = gmax;
	ToolManager::GetInstance()->forcolor[0][4] = bmin;
	ToolManager::GetInstance()->forcolor[0][5] = bmax;

}


void ColortTestDlg::OnBnClickedButton2()
{
	UpdateData(true);
	ToolManager::GetInstance()->forcolor[1][0] = grmin;
	ToolManager::GetInstance()->forcolor[1][1] = grmax;
	ToolManager::GetInstance()->forcolor[1][2] = ggmin;
	ToolManager::GetInstance()->forcolor[1][3] = ggmax;
	ToolManager::GetInstance()->forcolor[1][4] = gbmin;
	ToolManager::GetInstance()->forcolor[1][5] = gbmax;
	UpdateData(false);
}


void ColortTestDlg::OnBnClickedButton5()
{
	UpdateData(true);
	ToolManager::GetInstance()->forcolor[2][0] = yrmin;
	ToolManager::GetInstance()->forcolor[2][1] = yrmax;
	ToolManager::GetInstance()->forcolor[2][2] = ygmin;
	ToolManager::GetInstance()->forcolor[2][3] = ygmax;
	ToolManager::GetInstance()->forcolor[2][4] = ybmin;
	ToolManager::GetInstance()->forcolor[2][5] = ybmax;
	UpdateData(false);
}
