#pragma once
#include "afxdialogex.h"


// DataInquiryDlg 대화 상자

class DataInquiryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(DataInquiryDlg)

public:
	DataInquiryDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~DataInquiryDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DataInquiry };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedButton1();

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButton4();




public:
	CDateTimeCtrl StartTime;
	CDateTimeCtrl EndTime;
	CComboBox m_ColorBox;
	CComboBox m_FaultyBox;

	CListCtrl m_DataList;
	CEdit m_Url;
	CEdit m_Color;
	CEdit m_Faulty;
	CEdit m_Date;
	CButton m_Id;

public:
	bool Update();
	void Reset();
	void ResetAndUpdate();

	vector<AWSLIST> OptionDate(CString eday, CString sday, CString emon, CString smon, CString ey, CString sy);
	vector<AWSLIST> OptionColor();
	vector<AWSLIST> OptionFaulty();

	CString GetLast(CString url);

	void LoadOnlineImage(LPCTSTR url, CPaintDC& dc);


	afx_msg void OnNMClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	CStatic m_staticId;

	vector<AWSLIST> Updatevec;

	afx_msg void OnBnClickedButton3();
	afx_msg void OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	void GetButtonState(bool n);
	CStatic m_Downloadck;
	CString CurId;
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();




	void Set_Text(int curidx, int idx, CString cstr);

	

};
