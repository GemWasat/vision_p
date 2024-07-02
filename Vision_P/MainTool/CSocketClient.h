#pragma once

// CSocketClient 명령 대상
#include "resource.h"
#include <afxsock.h>

class CSocketClient : public CSocket
{
public:
	CSocketClient();
	virtual ~CSocketClient();
	virtual void OnClose(int nErrorCode);		// 통신 닫히면 실행
	virtual void OnReceive(int nErrorCode);		// 서버에서 통신 오면 실행


public:
	// 문자 나누기
	vector<CString> CSocketClient::SplitCString(const CString& str, const CString& delimiter);
	// 실행될 명령 
	void CSocketClient::ProcessString(const CString& str);

	void SocketEXIT();
	// 닫는 명령 확인
	void recvinfo(CString strMessage);
};

