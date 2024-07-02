#include "pch.h"
#include "ConnectTread.h"
#include "ServerForm.h"
#include "ToolManager.h"
#include "ResultForm.h"
#include "DataInquiryDlg.h"

#define WM_SOCKET_THREAD_FINISHED (WM_USER + 1)


// AWS 서버 쓰레드
UINT initawsT(LPVOID pParam)
{
	ServerForm* thisObj;
	thisObj = (ServerForm*)pParam;
	queue<AWSINFO>& infoALL = thisObj->GetAwsInfo();

	while (1) 
	{
		
		AWSINFO info = AWSINFO::STAY;
		if (!infoALL.empty()) {
			info = infoALL.front();
			infoALL.pop();
		}

		switch (info)
		{

			// AWS 서버 쓰레드 종료
			case AWSINFO::AWSEXIT:
			{
				return 0;
			}


			// AWS 서버 시작
			case AWSINFO::SEVERSTART:
			{
				TRACE(_T("Thread Index "));
				thisObj->initaws();

				// 서버 리트라이
				if (thisObj->GetAWS() == nullptr)
				{
					thisObj->ServerState(STATUCOLOR::SERVERRED);
					thisObj->SetAwsInfo(AWSINFO::SEVERSTART);
				}
				else
				{
					thisObj->GetServerList();
					thisObj->InvalidateRect(NULL, FALSE);
				}
				break;
			}


			// AWS 서버 저장
			case AWSINFO::AWSSEND:
			{
				string info = "('" + thisObj->GetAwsColor() + "', '" + thisObj->GetAwsFaulty() + "')";
				thisObj->SetAwsColor("");
				thisObj->SetAwsFaulty("");

				thisObj->GetAWS()->Allinput(thisObj->GetDate(), info.c_str(), thisObj->GetAwsFilename());
				thisObj->GetServerList();
				break;
			}


			// 로컬때 저장된 정보 서버에 저장
			case AWSINFO::AWSTEMPLIST:
			{
				ToolManager* pInstance = ToolManager::GetInstance();
				for (int i = 0; i < pInstance->m_TempVec.size(); ++i)
				{
					string info = "('" + pInstance->m_TempVec[i].color + "', '" + pInstance->m_TempVec[i].faulty + "')";
					thisObj->GetAWS()->Allinput(pInstance->m_TempVec[i].date, info.c_str(), pInstance->m_TempVec[i].filename);
				}
				pInstance->m_TempVec.clear();
				thisObj->GetServerList();
				break;
			}


			// 서버 연결 체크
			case AWSINFO::AWSCHEAK:
			{
				thisObj->GetAWS()->RDSckeckConnection();
				thisObj->GetServerList();
				break;
			}


			// 저장된 정보 수정
			case AWSINFO::AWSMODIFY:
			{
				thisObj->GetAWS()->RDSupdateData("color", thisObj->GetModifyColor().c_str(), thisObj->GetModifyCurId().c_str());
				thisObj->GetAWS()->RDSupdateData("faulty", thisObj->GetModifyFaulty().c_str(), thisObj->GetModifyCurId().c_str());
				ToolManager::GetInstance()->m_DataInquiryDlg->GetButtonState(true);
				thisObj->SetData();
				ToolManager::GetInstance()->m_DataInquiryDlg->ResetAndUpdate();

				break;
			}

			case AWSINFO::AWSDELETE:
			{
				string id = CT2CA(ToolManager::GetInstance()->m_DataInquiryDlg->CurId);
				ToolManager::GetInstance()->m_Serverform->GetAWS()->AlldeleteData(id.c_str());
				ToolManager::GetInstance()->m_DataInquiryDlg->GetButtonState(true);
				thisObj->SetData();
				ToolManager::GetInstance()->m_DataInquiryDlg->ResetAndUpdate();
			}

			// 서버 리스트 불러오기
			default:
			{
				int a = 0;
				break;
			}
		}

		Sleep(100);
	}

	return 0;
}


// 소켓통신 첫 접속 쓰레드
UINT ThreadSocket(LPVOID pParam)
{
	ServerForm* thisObj = static_cast<ServerForm*>(pParam);

	CSocketClient hSocket;
	hSocket.Create();
	CString strMessage = _T("END");
	int nLength = strMessage.GetLength() * sizeof(TCHAR);


	BOOL success = FALSE;

	// 연결 체크
	if (thisObj->GetTCPConnect()) {
		if (hSocket.Connect(thisObj->IPAddress(), thisObj->m_Port) == FALSE)
		{
			hSocket.Close();
			success = FALSE;
		}
		else
		{
			Sleep(100);
			hSocket.Send((LPCTSTR)strMessage, nLength);
			hSocket.Close();
			success = TRUE;
		}
	}

	// 연결 끊기
	else {
		thisObj->SetControlColor(STATUCOLOR::SOCKETRED);
		success = TRUE;
	}

	// 메인 스레드에 메시지 보내기 (이벤트)
	::PostMessage(thisObj->GetSafeHwnd(), WM_SOCKET_THREAD_FINISHED, (WPARAM)success, 0);

	return 0;
}

UINT ThreadCamera(LPVOID pParam) {
	ToolManager* thisObj;
	thisObj = (ToolManager*)pParam;
	int Count = 1;

	// 카메라 실행
	while (thisObj->cameraR) {
		if (Count < 0) {
			AfxMessageBox(_T("NO CAMERA"));
			return 0;
		}

		if (thisObj->SetCap(Count) == true)
		{
			thisObj->SetInference();
			thisObj->m_strPickinLst = L"";
			thisObj->m_Res = RESULT::RES_END;

			thisObj->cap >> thisObj->frame;

			thisObj->cameraR = false;
			break;
		}
		else {
			Count--;
		}
	}
	
	vector<Detection> output = thisObj->inf->runInference(thisObj->frame);
	thisObj->m_Resform->SetCameraButton(true);
	
	return 0;
}

