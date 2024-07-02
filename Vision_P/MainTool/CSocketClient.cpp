// CSocketClient.cpp: 구현 파일
//

#include "pch.h"
#include "MainTool.h"
#include "CSocketClient.h"
#include "ServerForm.h"
#include "ToolManager.h"
#include "CntrlForm.h"
#include "ResultForm.h"

// CSocketClient

CSocketClient::CSocketClient()
{
}

CSocketClient::~CSocketClient()
{
}


// 소켓 닫히면 실행됨
void CSocketClient::OnClose(int nErrorCode)
{
	// 서버 연결이 끊길 때를 정의합니다.
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
    SocketEXIT();

	CSocket::OnClose(nErrorCode);
}


// 서버에서 오는 데이터 탐색
void CSocketClient::OnReceive(int nErrorCode)
{
	// 서버에서 오는 데이터를 받습니다.
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	char szBuffer[1024];
	memset(szBuffer, 0, sizeof(szBuffer));

	int nRead = Receive(szBuffer, sizeof(szBuffer));
	if (nRead > 0) 
    {
		szBuffer[nRead] = '\0';
		CString strReceive(szBuffer);
		ToolManager::GetInstance()->m_Serverform->SetList(_T("수신"),strReceive);
		recvinfo(strReceive);
	}

	CSocket::OnReceive(nErrorCode);
}

// 데이터가 들어오면 문자를 받습니다.
void CSocketClient::recvinfo(CString strMessage)
{
    if (strMessage == _T("END"))
    {
        SocketEXIT();
    }
    else
    {
        ProcessString(strMessage);
    }
}

// 문자열을 주어진 구분자로 나누는 함수
vector<CString> CSocketClient::SplitCString(const CString& str, const CString& delimiter)
{
    // 문자열을 분할하여 저장할 벡터 생성
    std::vector<CString> tokens;

    // 시작 위치 초기화
    int startPos = 0;

    // 문자열을 주어진 구분자(delimiter)로 분할하고 첫 번째 토큰 가져오기
    CString token = str.Tokenize(delimiter, startPos);

    // 토큰이 비어 있지 않은 동안 반복
    while (!token.IsEmpty())
    {
        // 토큰을 벡터에 추가
        tokens.push_back(token);
        // 다음 토큰을 가져오기 위해 시작 위치 이동
        token = str.Tokenize(delimiter, startPos);
    }

    // 분할된 토큰들을 저장한 벡터 반환
    return tokens;
}

void CSocketClient::ProcessString(const CString& str)
{
    // 문자열에 '/'가 포함되어 있는지 검사
    if (str.Find(_T('/')) != -1)
    {
        // '/'가 포함되어 있는 경우

        // '/'로 문자열을 분할하여 첫 번째 단계의 결과를 저장할 벡터 생성
        std::vector<CString> firstSplit = SplitCString(str, _T("/"));

        // 첫 번째 단계의 결과를 반복하여 처리
        for (const auto& part : firstSplit)
        {
            int colonPos = part.Find(':');
            if (colonPos != -1)
            {
                CString betweenColonAndSlash = part.Mid(colonPos + 1);

                if (betweenColonAndSlash.Find(_T("GRAB")) != -1)
                {
                    // "GRAP" 포함하면 전달
                    ToolManager::GetInstance()->bGrap = true;
                    ToolManager::GetInstance()->m_Serverform->ClientTCP(betweenColonAndSlash);
                }

                else if (betweenColonAndSlash.Find(_T("DONE")) != -1)
                {
                    // "DONE을 포함하면 전달
                    CString str = _T("START");
                    ToolManager::GetInstance()->m_Serverform->ClientTCP(str);

                    ToolManager::GetInstance()->bDone = true;


                }

                else if (betweenColonAndSlash.Find(_T("HOME")) != -1)
                {
                    
                    ToolManager::GetInstance()->m_CntrlForm->Grab_on = false;
                    ToolManager::GetInstance()->m_CntrlForm->Morter_on = false;
                    ToolManager::GetInstance()->m_CntrlForm->Morter2_on = false;
                    ToolManager::GetInstance()->m_CntrlForm->Morter3_on = false;
                    ToolManager::GetInstance()->m_CntrlForm->Invalidate(true);
                }

                else if (betweenColonAndSlash.Find(_T("M1_P10")) != -1)
                {
                    
                    ToolManager::GetInstance()->m_CntrlForm->Grab_on = false;
                    ToolManager::GetInstance()->m_CntrlForm->Invalidate(true);
                }

                else if (betweenColonAndSlash.Find(_T("M1_M10")) != -1)
                {
                    
                    ToolManager::GetInstance()->m_CntrlForm->Grab_on = false;
                    ToolManager::GetInstance()->m_CntrlForm->Invalidate(true);
                }

                else if (betweenColonAndSlash.Find(_T("M2_P10")) != -1)
                {
                    
                    ToolManager::GetInstance()->m_CntrlForm->Morter_on = false;
                    ToolManager::GetInstance()->m_CntrlForm->Invalidate(true);
                }

                else if (betweenColonAndSlash.Find(_T("M2_M10")) != -1)
                {
                   
                    ToolManager::GetInstance()->m_CntrlForm->Morter_on = false;
                    ToolManager::GetInstance()->m_CntrlForm->Invalidate(true);
                }

                else if (betweenColonAndSlash.Find(_T("M3_P10")) != -1)
                {
                    
                    ToolManager::GetInstance()->m_CntrlForm->Morter2_on = false;
                    ToolManager::GetInstance()->m_CntrlForm->Invalidate(true);
                }
                else if (betweenColonAndSlash.Find(_T("M3_M10")) != -1)
                {
                    
                    ToolManager::GetInstance()->m_CntrlForm->Morter2_on = false;
                    ToolManager::GetInstance()->m_CntrlForm->Invalidate(true);
                }
                else if (betweenColonAndSlash.Find(_T("M4_P10")) != -1)
                {
                    
                    ToolManager::GetInstance()->m_CntrlForm->Morter3_on = false;
                    ToolManager::GetInstance()->m_CntrlForm->Invalidate(true);
                }
                else if (betweenColonAndSlash.Find(_T("M4_M10")) != -1)
                {
                   
                    ToolManager::GetInstance()->m_CntrlForm->Morter3_on = false;
                    ToolManager::GetInstance()->m_CntrlForm->Invalidate(true);
                }



            }
        }
    }
    else
    {
        // '/'가 포함되어 있지 않은 경우
        // 전체 문자열을 그대로 출력
        CString str_ar = _T("올바른 명령이 아닙니다");
        ToolManager::GetInstance()->m_Serverform->ClientTCP(str_ar);
    }
}

void CSocketClient::SocketEXIT()
{
    ToolManager::GetInstance()->m_Serverform->ClinetSetting(false);
}
