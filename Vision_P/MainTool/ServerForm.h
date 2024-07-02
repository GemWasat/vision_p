#pragma once



// ServerForm 폼 보기
#include "CSocketClient.h"
#include "AWS.h"
#include<queue>

class ServerForm : public CFormView
{
	DECLARE_DYNCREATE(ServerForm)

protected:
	ServerForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~ServerForm();
public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ServerForm };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	// 이벤트 함수
	afx_msg void OnBnClickedTcpBut();
	afx_msg void OnPaint();
	afx_msg void OnLvnItemchangedListTcp(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnSocketThreadFinished(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedLogBut();


	// 가상함수
	virtual void OnInitialUpdate();


	// 도구 자료형
	UINT m_Port;						// PORT를 가져옴
	CIPAddressCtrl m_IP;				// IP를 가져옴
	CButton m_TCP_BUTTON;				// 소켓 통신 연결 버튼
	CListCtrl m_ListTcp;				// 소켓 통신 로그 표시
	CStatic m_StateColor;				// 소켓 통신상태 픽쳐 컨트롤
	CStatic m_ServerColor;				// 서버 통신상태 픽쳐 컨트롤

private:
	// 통신상태 색깔 자료형
	STATUCOLOR m_ControlColor = STATUCOLOR::STAY;			// 통신상태 색깔 정할 수 있음
	STATUCOLOR m_ServerSwitch = STATUCOLOR::SERVERRED;
	COLORTHREAD m_ThreadColor = COLORTHREAD::THREADRUN;		// 통신상태 색깔 쓰레드 종료
	CString m_SColor = L"yellow";							// 서버 통신상태 색깔
	CString m_KColor = L"red";								// 소켓 통신상태 색깔

private:
	// 스위치
	BOOL m_TCPConnect = TRUE;			// 연결, 연결끊기 스위치
	BOOL AWSRUN = TRUE;					// 서버 연결 스위치
	BOOL m_SocketThreadSWICHT = TRUE;	// 소켓 쓰레드 병목 막는 스위치
	bool m_boxrun = false;
	

private:
	// DB 리스트 불러오면 여기에 저장
	vector<AWSLIST> m_boxlist;
	vector<string> LogList;				// 로그 파일 이름 저장
	string LogName;						// 선택한 파일 이름 저장
	
private:
	//서버 기동 자료형
	SDKOptions m_options;
	AWS* m_aws = nullptr;

private:
	// 서버 저장 자료형
	string m_awscolor = "";
	string m_awsfaulty = "";
	string m_modifyColor;
	string m_modifyFaulty;
	string m_modifyCurId;
	string m_awsdate;
	string m_awsfilename;
	queue<AWSINFO> m_AwsAllList;

	// 소켓 저장 자료형
	CString SocketMessage = _T("");

	// 소켓 통신 자료형
	CSocketClient m_Client;										// 소켓 연결
	int Count = 0;												// 소켓 로그 카운트
public:
	vector<CString> SplitCString(const CString& str, const CString& delimiter);
	vector<AWSLIST> GetBoxlist();

	// 소켓통신 함수
	const CString IPAddress() const;							// IP Address를 호출합니다.
	void ClientTCP(CString strMessage);							// 서버로 정보를 전송할때 사용합니다.
	void SetList(CString str, CString strMessage);				// 리스트 컨트롤에 로그 추가
	void ClinetSetting(bool set);

	// 서버 동작 함수
	void exit_s3();												// 서버 종료 함수
	void initaws();												// 서버 연결 함수
	void ServerState(STATUCOLOR set);
	void GetServerList();

public:
	queue<AWSINFO>& GetAwsInfo();								// 서버 동작 상태
	BOOL GetTCPConnect();										// 소켓 통신 연결 상태
	AWS* GetAWS();												// 서버 연결 동작
	string GetAwsColor();										// 서버 저장 상자 색깔 상태
	string GetAwsFaulty();										// 서버 저장 상자 불량 상태
	string GetAwsFilename();									// 서버 저장 이미지 파일 이름
	string GetModifyColor();									// 서버 색깔 수정
	string GetModifyFaulty();									// 서버 불량 유무 수정
	string GetModifyCurId();									// 서버 상태 수정
	string GetDate();											// 날짜 얻어옴
	COLORTHREAD GetThreadColor();								// 상태 그리기 쓰레드 스위치
	STATUCOLOR GetControlColor();								// 서버 모든 상태 스위치
	STATUCOLOR GetServerSwitch();								// 서버만 상태 확인 스위치
	vector<string> GetLog();									// 저장된 로그 얻어오기
	vector<string> GetLogList();								// 로그 파일 이름 얻어오기
	
public:
	void SetAwsInfo(AWSINFO pAWS);								// 서버 동작 
	void SetModify(string color, string faulty, string curId);	// 서버 리스트 수정
	void SetAwsColor(string set);								// 서버 저장 상자 색깔 상태
	void SetAwsFaulty(string set);								// 서버 저장 상자 불량 상태
	void SetAwsFilename(string set);							// 박스 사진 이름
	void SetModifyColor(string set);							// 서버 색깔 수정
	void SetModifyFaulty(string set);							// 서버 불량 수정
	void SetModifyCurId(string set);							// 서버 수정
	void SetDate(string set);									// 날짜 세팅
	void SetClientClose();										// 소켓 통신 종료
	void SetThreadColor(COLORTHREAD set);						// 상태 그리기 쓰레드 스위치
	void SetControlColor(STATUCOLOR set);						// 서버 모든 상태 스위치
	void SetServerSwitch(STATUCOLOR set);						// 서버만 상태 확인 스위치
	void SETTCPConnect(BOOL set);								// 소켓통신 연결 상태 확인
	void SETBoxlist(vector<AWSLIST> set);						// 서버 리스트 저장
	bool SetLog(CString Log);									// 로그 저장


	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);


	void SetData();
};


