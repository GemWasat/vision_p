#pragma once
#include "afxdialogex.h"


// DetectTab 대화 상자

class DetectTab : public CDialog
{
	DECLARE_DYNAMIC(DetectTab)

public:
	DetectTab(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~DetectTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DetectTab };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnNMClickList1(NMHDR* pNMHDR, LRESULT* pResult);


public:
	void Update();

public:
	void Set_Text(int curidx, int idx, CString cstr);
public:
	CListCtrl m_List;
	bool Threadupdate = false;
	bool Toolupdate = false;
};
