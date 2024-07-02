// DataInquiryDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MainTool.h"
#include "afxdialogex.h"
#include "DataInquiryDlg.h"
#include "ToolManager.h"
#include "ServerForm.h"
#include "AWS.h"

// DataInquiryDlg 대화 상자

IMPLEMENT_DYNAMIC(DataInquiryDlg, CDialogEx)

DataInquiryDlg::DataInquiryDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DataInquiry, pParent)
{

}

DataInquiryDlg::~DataInquiryDlg()
{
}

void DataInquiryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, StartTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, EndTime);
	DDX_Control(pDX, IDC_COMBO2, m_ColorBox);
	DDX_Control(pDX, IDC_COMBO3, m_FaultyBox);
	DDX_Control(pDX, IDC_LIST1, m_DataList);
	DDX_Control(pDX, IDC_EDIT1, m_Url);
	DDX_Control(pDX, IDC_EDIT2, m_Color);
	DDX_Control(pDX, IDC_EDIT3, m_Faulty);
	DDX_Control(pDX, IDC_EDIT4, m_Date);
	DDX_Control(pDX, IDC_BUTTON1, m_Id);
	DDX_Control(pDX, IDC_StaticId, m_staticId);
	DDX_Control(pDX, IDC_Downloadck, m_Downloadck);
}


BEGIN_MESSAGE_MAP(DataInquiryDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &DataInquiryDlg::OnBnClickedButton1)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON4, &DataInquiryDlg::OnBnClickedButton4)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &DataInquiryDlg::OnNMClickList1)
	ON_BN_CLICKED(IDC_BUTTON3, &DataInquiryDlg::OnBnClickedButton3)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &DataInquiryDlg::OnNMDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON5, &DataInquiryDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &DataInquiryDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// DataInquiryDlg 메시지 처리기




BOOL DataInquiryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ToolManager::GetInstance()->SetDataDlg(this);

	Updatevec.clear();

	LVCOLUMN Column;

	m_DataList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	UpdateData();

	Column.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	Column.fmt = LVCFMT_LEFT;

	LPWSTR Column_list[5] = { _T("ID"), _T("Url") , _T("Color"),_T("Fine"),_T("Date") };
	int cx[5] = { 50,500,100,100,400 };

	int Column_size = sizeof(Column_list) / sizeof(Column_list[0]);
	for (int i = 0; i < Column_size; i++) {
		Column.cx = cx[i];
		Column.pszText = Column_list[i];
		m_DataList.InsertColumn(i, &Column);
	}

	Update();

	m_ColorBox.InsertString(0, L"red");
	m_ColorBox.InsertString(1, L"yellow");
	m_ColorBox.InsertString(2, L"green");
	m_ColorBox.InsertString(3, L"fail");

	m_FaultyBox.InsertString(0, L"OK");
	m_FaultyBox.InsertString(0, L"Faulty");

	m_Downloadck.SetWindowText(_T(""));
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
}



void DataInquiryDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	UpdateData(TRUE);


	CString url;
	GetDlgItemText(IDC_EDIT1, url);


	if (url != L"")
		LoadOnlineImage(url, dc);
	else
	{
		CImage png;
		png.Load(L"ns.png");
		png.StretchBlt(dc, 60, 10, 340, 220);
	}

	UpdateData(FALSE);
}





void DataInquiryDlg::OnBnClickedButton1()
{
	//다운로드 

	if (ToolManager::GetInstance()->m_Serverform->GetServerSwitch() == STATUCOLOR::SERVERGREEN)
	{
		ofstream file("output.csv");

		if (file.is_open()) {
			// Header
			file << "ID,Color,Faulty,Date,URL\n";


			if (!Updatevec.empty())
			{
				for (const auto& row : Updatevec) {
					file << row.id << "," << row.color << "," << row.faulty << "," << row.date << "," << row.url << "\n";
				}
			}
			else
			{
				vector<AWSLIST> vecInfo;
				vecInfo = ToolManager::GetInstance()->m_Serverform->GetBoxlist();
				for (const auto& row : vecInfo) {
					file << row.id << "," << row.color << "," << row.faulty << "," << row.date << "," << row.url << "\n";
				}
			}


			file.close();
			cout << "CSV file created successfully!" << endl;
			m_Downloadck.SetWindowText(_T("Download successfully!"));
			UpdateData(false);
		}
		else {
			cerr << "Could not open the file!" << endl;
			m_Downloadck.SetWindowText(_T("download  fail!"));
			UpdateData(false);

		}
	}
	else
	{
		m_Downloadck.SetWindowText(_T("Server not connected"));
		UpdateData(false);
	}

}

void DataInquiryDlg::OnBnClickedButton4()
{
	CString starttime;
	CString endtime;
	CString Color;
	CString Faulty;

	GetDlgItemText(IDC_DATETIMEPICKER1, starttime);
	GetDlgItemText(IDC_DATETIMEPICKER2, endtime);
	GetDlgItemText(IDC_COMBO2, Color);
	GetDlgItemText(IDC_COMBO3, Faulty);

	CString syear;
	CString smonth;
	CString sday;

	CString eyear;
	CString emonth;
	CString eday;

	AfxExtractSubString(syear, starttime, 0, '-');
	AfxExtractSubString(smonth, starttime, 1, '-');
	AfxExtractSubString(sday, starttime, 2, '-');

	AfxExtractSubString(eyear, endtime, 0, '-');
	AfxExtractSubString(emonth, endtime, 1, '-');
	AfxExtractSubString(eday, endtime, 2, '-');

	Updatevec = OptionDate(eday, sday, emonth, smonth, eyear, syear);
	Updatevec = OptionColor();
	Updatevec = OptionFaulty();

	Update();
}

bool DataInquiryDlg::Update()
{

	if (Updatevec.empty())
	{
		vector<AWSLIST> vecInfo;
		vecInfo = ToolManager::GetInstance()->m_Serverform->GetBoxlist();

		m_DataList.DeleteAllItems();

		for (int i = 0; i < vecInfo.size(); ++i)
		{
			m_DataList.InsertItem(i, CString(vecInfo[i].id.c_str()));
			m_DataList.SetItemText(i, 1, CString(vecInfo[i].url.c_str()));
			m_DataList.SetItemText(i, 2, CString(vecInfo[i].color.c_str()));
			m_DataList.SetItemText(i, 3, CString(vecInfo[i].faulty.c_str()));
			m_DataList.SetItemText(i, 4, CString(vecInfo[i].date.c_str()));
		}
		if (vecInfo.empty())
		{
			m_DataList.DeleteAllItems();
		}
	}
	else
	{
		m_DataList.DeleteAllItems();

		for (int i = 0; i < Updatevec.size(); ++i)
		{
			m_DataList.InsertItem(i, CString(Updatevec[i].id.c_str()));
			m_DataList.SetItemText(i, 1, CString(Updatevec[i].url.c_str()));
			m_DataList.SetItemText(i, 2, CString(Updatevec[i].color.c_str()));
			m_DataList.SetItemText(i, 3, CString(Updatevec[i].faulty.c_str()));
			m_DataList.SetItemText(i, 4, CString(Updatevec[i].date.c_str()));
		}
	}



	return true;
}

void DataInquiryDlg::Reset()
{

	vector<AWSLIST> vecInfo;
	vecInfo = ToolManager::GetInstance()->m_Serverform->GetBoxlist();

	m_DataList.DeleteAllItems();
	Updatevec.clear();

	for (int i = 0; i < vecInfo.size(); ++i)
	{
		m_DataList.InsertItem(i, CString(vecInfo[i].id.c_str()));
		m_DataList.SetItemText(i, 1, CString(vecInfo[i].url.c_str()));
		m_DataList.SetItemText(i, 2, CString(vecInfo[i].color.c_str()));
		m_DataList.SetItemText(i, 3, CString(vecInfo[i].faulty.c_str()));
		m_DataList.SetItemText(i, 4, CString(vecInfo[i].date.c_str()));
	}
	if (vecInfo.empty())
	{
		m_DataList.DeleteAllItems();
	}

	m_ColorBox.SetCurSel(-1);
	m_FaultyBox.SetCurSel(-1);



}

void DataInquiryDlg::ResetAndUpdate()
{
	Updatevec.clear();

	CString starttime;
	CString endtime;
	CString Color;
	CString Faulty;

	GetDlgItemText(IDC_DATETIMEPICKER1, starttime);
	GetDlgItemText(IDC_DATETIMEPICKER2, endtime);
	GetDlgItemText(IDC_COMBO2, Color);
	GetDlgItemText(IDC_COMBO3, Faulty);

	CString syear;
	CString smonth;
	CString sday;

	CString eyear;
	CString emonth;
	CString eday;

	AfxExtractSubString(syear, starttime, 0, '-');
	AfxExtractSubString(smonth, starttime, 1, '-');
	AfxExtractSubString(sday, starttime, 2, '-');

	AfxExtractSubString(eyear, endtime, 0, '-');
	AfxExtractSubString(emonth, endtime, 1, '-');
	AfxExtractSubString(eday, endtime, 2, '-');

	Updatevec = OptionDate(eday, sday, emonth, smonth, eyear, syear);
	Updatevec = OptionColor();
	Updatevec = OptionFaulty();

	Update();

}

vector<AWSLIST> DataInquiryDlg::OptionDate(CString eday, CString sday, CString emon, CString smon, CString ey, CString sy)
{
	Updatevec.clear();


	vector<AWSLIST> vecInfo;
	vecInfo = ToolManager::GetInstance()->m_Serverform->GetBoxlist();

	if (vecInfo.empty())
		return vecInfo;

	CString Y;
	CString M;
	CString D;

	bool Start = false;
	bool Last = false;

	//lastidx가 더 0에 가깝게 들어감  즉 last 부터 start까지 
	int Startidx = 0;
	int Lastidx = 0;
	int cnt = 0;



	int eyy = _ttoi(ey);
	int emm = _ttoi(emon);
	int edd = _ttoi(eday);

	CString date = CString(vecInfo[0].date.c_str());
	AfxExtractSubString(Y, date, 0, '/');
	AfxExtractSubString(M, date, 1, '/');
	AfxExtractSubString(D, date, 2, '/');


	if (edd > _ttoi(D))
	{
		while (!(edd == _ttoi(D)))
			--edd;
	}


	int LasttoStart = 0;

	//같은날
	for (int i = 0; i < vecInfo.size(); ++i)
	{
		CString date = CString(vecInfo[i].date.c_str());
		AfxExtractSubString(Y, date, 0, '/');
		AfxExtractSubString(M, date, 1, '/');
		AfxExtractSubString(D, date, 2, '/');
		int t = _ttoi(emon);

		if (_ttoi(Y) == eyy && _ttoi(M) == emm && _ttoi(D) == edd && Last == false)
		{
			//가장 최근데이터가 같을때 즉 넣을인자의 마지막 
			Lastidx = i;
			Last = true;
		}
		if (_ttoi(Y) == _ttoi(sy) && _ttoi(M) == _ttoi(smon) && _ttoi(D) == _ttoi(sday) && Start == false)
		{
			//가장 최근데이터가 같을때 즉 넣을인자의 마지막 
			Start = true;
		}
		if (_ttoi(Y) == _ttoi(sy) && _ttoi(M) == _ttoi(smon) && _ttoi(D) == _ttoi(sday) && Start == true)
		{
			cnt++;
		}
		if (Start == true && !(_ttoi(Y) == _ttoi(sy) && _ttoi(M) == _ttoi(smon) && _ttoi(D) == _ttoi(sday)))
		{
			Start = false;
			break;
		}

		if (Last == true && Start == false)
		{
			++LasttoStart;
		}
	}


	if (Start == true)
		cnt += Lastidx;


	if(Lastidx!=0 && Start==false)
		cnt += Lastidx;


	vector<AWSLIST> retvec;

	for (int i = Lastidx; i < cnt + LasttoStart; ++i)
	{
		retvec.emplace_back(vecInfo[i]);
	}

	return retvec;
}

vector<AWSLIST> DataInquiryDlg::OptionColor()
{
	int idx = m_ColorBox.GetCurSel();
	if (idx == -1)
		return Updatevec;

	CString color;
	m_ColorBox.GetLBText(idx, color);


	vector<AWSLIST> retvec;


	for (int i = 0; i < Updatevec.size(); ++i)
	{
		if (color == CString(Updatevec[i].color.c_str()))
		{
			retvec.emplace_back(Updatevec[i]);
		}
	}

	if (retvec.empty())
		return Updatevec;


	return retvec;
}

vector<AWSLIST> DataInquiryDlg::OptionFaulty()
{
	int idx = m_FaultyBox.GetCurSel();
	if (idx == -1)
		return Updatevec;

	CString faulty;
	m_FaultyBox.GetLBText(idx, faulty);


	vector<AWSLIST> retvec;


	for (int i = 0; i < Updatevec.size(); ++i)
	{
		if (faulty == CString(Updatevec[i].faulty.c_str()))
		{
			retvec.emplace_back(Updatevec[i]);
		}
	}


	if (retvec.empty())
		return Updatevec;


	return retvec;
}



CString DataInquiryDlg::GetLast(CString url)
{
	vector<CString> vec;
	CString res = L"";
	for (int i = 0; i < url.GetLength(); ++i)
	{
		if (url[i] != '/')
			res += url[i];
		else
		{
			vec.emplace_back(res);
			res = L"";
		}
		if (i == url.GetLength() - 1)
		{
			vec.emplace_back(res);
			res = L"";
		}
	}


	return CString(vec[vec.size() - 1]);
}

void DataInquiryDlg::LoadOnlineImage(LPCTSTR url, CPaintDC& dc)
{
	CInternetSession session;
	CHttpFile* pFile = NULL;

	try {
		pFile = (CHttpFile*)session.OpenURL(url);
		DWORD dwStatusCode;
		pFile->QueryInfoStatusCode(dwStatusCode);
		if (dwStatusCode == HTTP_STATUS_OK) {
			CFile file;
			CString tempPath;
			GetTempPath(MAX_PATH, tempPath.GetBuffer(MAX_PATH));
			tempPath.ReleaseBuffer();

			CString tempFile = tempPath + GetLast(url);
			file.Open(tempFile, CFile::modeCreate | CFile::modeWrite);
			DWORD dwRead;
			BYTE pBuffer[1024];
			while ((dwRead = pFile->Read(pBuffer, sizeof(pBuffer))) > 0) {
				file.Write(pBuffer, dwRead);
			}
			file.Close();


			CImage image;
			image.Load(tempFile);

			SetStretchBltMode(dc, HALFTONE);

			image.StretchBlt(dc, 60, 10, 340, 220);


		}
		else {
			// 오류
		}
	}
	catch (CInternetException* pEx) {
		pEx->Delete();
	}

	if (pFile)
		pFile->Close();
	session.Close();
}

void DataInquiryDlg::OnNMClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.






	*pResult = 0;
}


void DataInquiryDlg::OnBnClickedButton3()
{
	// reset

	Reset();



}


void DataInquiryDlg::OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);


	int clickindex = m_DataList.GetSelectionMark();

	CString id = m_DataList.GetItemText(clickindex, 0);
	CString url = m_DataList.GetItemText(clickindex, 1);
	CString Color = m_DataList.GetItemText(clickindex, 2);
	CString Faulty = m_DataList.GetItemText(clickindex, 3);
	CString Date = m_DataList.GetItemText(clickindex, 4);

	SetDlgItemText(IDC_StaticId, id);
	SetDlgItemText(IDC_EDIT1, url);
	SetDlgItemText(IDC_EDIT2, Color);
	SetDlgItemText(IDC_EDIT3, Faulty);
	SetDlgItemText(IDC_EDIT4, Date);


	CurId = id;



	Invalidate(true);

	*pResult = 0;
}


void DataInquiryDlg::GetButtonState(bool n)
{
	GetDlgItem(IDC_BUTTON6)->EnableWindow(n);
	GetDlgItem(IDC_BUTTON5)->EnableWindow(n);
}

void DataInquiryDlg::OnBnClickedButton5()
{
	//삭제
	GetButtonState(false);
	ToolManager::GetInstance()->m_Serverform->SetAwsInfo(AWSINFO::AWSDELETE);

}


void DataInquiryDlg::OnBnClickedButton6()
{
	//수정 

	UpdateData(TRUE);
	GetButtonState(false);
	CString c;
	CString f;
	GetDlgItemText(IDC_EDIT2, c);
	GetDlgItemText(IDC_EDIT3, f);
	string co = CT2CA(c);
	string fa = CT2CA(f);
	string id = CT2CA(CurId);
	ToolManager::GetInstance()->m_Serverform->SetModify(co, fa, id);

	UpdateData(FALSE);
}

void DataInquiryDlg::Set_Text(int curidx, int idx, CString cstr)
{
	UpdateData(TRUE);
	m_DataList.SetItemText(curidx, idx, cstr);
	UpdateData(FALSE);
}
