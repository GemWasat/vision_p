#pragma once
#include "afxdialogex.h"


// TestTab 대화 상자

class TestTab : public CDialog
{
	DECLARE_DYNAMIC(TestTab)

public:
	TestTab(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~TestTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TestTab };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	void LoadOnlineImage(LPCTSTR url);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton5();

	CString GetLast(CString url);

public:
	void Setcnt(int cnt);
	int Getcnt();
	void GetButtonState(bool n);





public:
	int Curcnt = 0;
	int imgcnt = 0;
	string CurId;
	CEdit m_FaultyEdit;
	CEdit m_ColorEdit;

	CString Url;
	CString Color;
	CString Faulty;
	CString Date;


	CEdit UrlEdit;
	CEdit DateEdit;
	CStatic idinsert;

	bool DeleteSwitch = true;
};
