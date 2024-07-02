#pragma once
#include "afxdialogex.h"


// ColortTestDlg 대화 상자

class ColortTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ColortTestDlg)

public:
	ColortTestDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~ColortTestDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLORTEST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CEdit redmin;
	CEdit redmax;
	CEdit greenmin;
	CEdit greenmax;
	CEdit yellowmin;
	CEdit yellowmax;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton5();
	int grmin;
	int grmax;
	int ggmin;
	int ggmax;
	int gbmin;
	int gbmax;
	int yrmin;
	int yrmax;
	int ygmin;
	int ygmax;
	int ybmin;
	int ybmax;
};
