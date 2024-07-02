#include "pch.h"
#include "ToolManager.h"

#include "ResultForm.h"
#include "ServerForm.h"
#include "CntrlForm.h"
#include "ConnectTread.h"
#include "struct.h"
#include "DataInquiryDlg.h"

ToolManager* ToolManager::m_pInstance = nullptr;



ToolManager::ToolManager()
{
	ForceQuit = false;
	FrmKilled = true;
	fpscnt = 0;
	dfps = 0;
	updatetime = 0;
	m_OldState = PROCESSSTATE::STANDBY;
}

ToolManager::~ToolManager()
{

}

ToolManager* ToolManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new ToolManager();
	}
	return m_pInstance;
}

void ToolManager::DestroyInstance()
{
	cap.release();

	delete inf;
	inf = nullptr;
	delete m_pInstance;
	m_pInstance = nullptr;

}

void ToolManager::Initialize()
{
	AfxBeginThread(ThreadCamera, this);

	for (int i = 0; i < 3; ++i)
	{
		for (int k = 0; k < 6; ++k)
		{
			forcolor[i][k] = 0;
		}
	}

}

bool ToolManager::Update(double t)
{

	if (ForceQuit == true)
		return false;

	++fpscnt;
	dtime += t;
	updatetime += t;
	

	

	if (FrmKilled == false)
	{
		if (onlycam == true && m_bReadyState == false)
		{
			cap >> frame;
			resize(frame, frame, Size(600, 450));
			Detecttest();
		}
		else
		{
			if (m_bReadyState == false)
			{
				return false;
			}

			cap >> frame;
			resize(frame, frame, Size(600, 450));

			if (m_OldState != m_CurState)
				m_OldState = m_CurState;

			switch (m_OldState)
			{
			case PROCESSSTATE::STANDBY:
				SetProcessState(PROCESSSTATE::WAIT_CAM_GRAB);
				break;
			case PROCESSSTATE::WAIT_CAM_GRAB:
				// 카메라 촬상 조건 추가
				if (bGrap == true)
				{
					m_Serverform->SetList(_T(""), _T("INSPECT START"));
					SetProcessState(PROCESSSTATE::INSPECT);
					bGrap = false;
				}
				break;
			case PROCESSSTATE::INSPECT:
				bDone = false;
				m_Res = Detect();

				if (m_Res == RES_END)
				{
					m_Serverform->SetList(_T(""), _T("PROC:ERROR"));
					m_Serverform->ClientTCP(_T("ST/PROC:ERROR/END"));
				}
				else if (m_Res == RES_NONE)
				{
					m_Serverform->SetList(_T(""), _T("PROC:RETRY"));
					m_Serverform->ClientTCP(_T("ST/PROC:RETRY/END"));
				}
				else
				{
					SendResult(m_Res);

					m_Serverform->SetAwsInfo(AWSINFO::AWSSEND);
				}
				m_Resform->RedrawWindow();
				SetProcessState(PROCESSSTATE::STANDBY);
				break;
			case PROCESSSTATE::ABNORMAL:
				break;
			default:
				break;
			}
		}
	}
	else
	{
		if (SpecialOn == false)
			frame = imread("ns.png");
		else
			frame = imread(specFileName);

		resize(frame, frame, Size(600, 450));
	}




	if (1.0 <= dtime)
	{
		dfps = fpscnt;
		fpscnt = 0;
		dtime = 0;
	}

	return true;
}

void ToolManager::LateUpdate()
{



}

void ToolManager::Render()
{
	if (ForceQuit == false)
	{
		putText(frame, "R:" + to_string(maxR), Point(0, 70), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 1);
		putText(frame, "G:" + to_string(maxG), Point(0, 110), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 1);
		putText(frame, "B:" + to_string(maxB), Point(0, 150), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 1);
		putText(frame, "fps: " + to_string(dfps), Point(10, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 1);
		cv::imshow("FINAL", frame);
		int key = cv::waitKey(1);
		if (key == 27)
			::PostQuitMessage(WM_QUIT);
	}
}

RESULT ToolManager::Detect()
{
	vector<Detection> output = inf->runInference(frame);
	int detections = output.size();
	std::cout << "Number of detections:" << detections << std::endl;

	if (detections <= 0)
		return RESULT::RES_NONE;

	for (int i = 0; i < detections; ++i)
	{
		Detection detection = output[i];
		if (detection.confidence < 0.8)
			continue;
		cv::Rect box = detection.box;
		std::string classString = detection.className + ' ' + std::to_string(detection.confidence).substr(0, 4);
		cv::Scalar color = detection.color;
		cv::Size textSize = cv::getTextSize(classString, cv::FONT_HERSHEY_DUPLEX, 1, 2, 0);
		cv::Rect textBox(box.x, box.y - 40, textSize.width + 10, textSize.height + 20);

		if (box.x > 0 && box.y > 0 && box.x + box.width < frame.cols && box.y + box.height < frame.rows)
		{

			roi = Mat(frame, box);
			findMostFrequentColor(roi, maxR, maxG, maxB);
			if (detection.className == "fail")
			{
				cv::putText(frame, classString, cv::Point(box.x + 5, box.y - 10), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 0), 2, 0);
				return RESULT::FAIL;
			}
			else
			{
				if ((maxR >= 150 && maxR <= 255) && (maxG >= 150 && maxG <= 255) && (maxB >= 0 && maxB <= 140))
				{
					putText(frame, "YELLOW" + std::to_string(detection.confidence).substr(0, 4), cv::Point(box.x + 5, box.y - 10), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 0), 2);
					return RESULT::YELLOW;
				}
				else if ((maxR >= 130 && maxR <= 255) && (maxG >= 0 && maxG <= 140) && (maxB >= 0 && maxB <= 140))
				{
					putText(frame, "RED" + std::to_string(detection.confidence).substr(0, 4), cv::Point(box.x + 5, box.y - 10), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 0), 2);
					return RESULT::RED;
				}
				else if ((maxR >= 0 && maxR <= 140) && (maxG >= 100 && maxG <= 255) && (maxB >= 0 && maxB <= 140))
				{
					putText(frame, "GREEN" + std::to_string(detection.confidence).substr(0, 4), cv::Point(box.x + 5, box.y - 10), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 0), 2);
					return RESULT::GREEN;
				}

			}
		}
	}
	return RESULT::RES_END;
}

void ToolManager::Save()
{
	if (FrmKilled == false)
	{
		imwrite(to_string(testcnt) + ".png", frame);
		++testcnt;
	}
}

void ToolManager::ShowPic(string Filename)
{
	specFileName = Filename;

	if (FrmKilled == true)
	{
		SpecialOn = true;
	}
}

void ToolManager::RenderImg(CStatic* p, CString filepath)
{
	CImage image;
	image.Load(filepath);//레드
	CRect rect;
	p->GetWindowRect(rect);
	CDC* dc;
	dc = p->GetDC();
	image.StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
	ReleaseDC(p->GetSafeHwnd(), dc->m_hDC);
}

RESULT ToolManager::Detecttest()
{
	vector<Detection> output = inf->runInference(frame);
	int detections = output.size();
	std::cout << "Number of detections:" << detections << std::endl;

	if (detections <= 0)
		return RESULT::RES_NONE;

	for (int i = 0; i < detections; ++i)
	{
		Detection detection = output[i];
		if (detection.confidence < 0.8)
			continue;
		cv::Rect box = detection.box;
		std::string classString = detection.className + ' ' + std::to_string(detection.confidence).substr(0, 4);
		cv::Scalar color = detection.color;
		cv::Size textSize = cv::getTextSize(classString, cv::FONT_HERSHEY_DUPLEX, 1, 2, 0);
		cv::Rect textBox(box.x, box.y - 40, textSize.width + 10, textSize.height + 20);

		if (box.x > 0 && box.y > 0 && box.x + box.width < frame.cols && box.y + box.height < frame.rows)
		{

			roi = Mat(frame, box);
			findMostFrequentColor(roi, maxR, maxG, maxB);
			if (detection.className == "fail")
			{
				cv::putText(frame, classString, cv::Point(box.x + 5, box.y - 10), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 0), 2, 0);
				return RESULT::FAIL;
			}
			else
			{
				if ((maxR >= forcolor[0][0] && maxR <= forcolor[0][1]) && (maxG >= forcolor[0][2] && maxG <= forcolor[0][3]) && (maxB >= forcolor[0][4] && maxB <= forcolor[0][5]))
				{
					putText(frame, "RED" + std::to_string(detection.confidence).substr(0, 4), cv::Point(box.x + 5, box.y - 10), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 0), 2);
					return RESULT::RED;
				}
				else if ((maxR >= forcolor[1][0] && maxR <= forcolor[1][1]) && (maxG >= forcolor[1][2] && maxG <= forcolor[1][3]) && (maxB >= forcolor[1][4] && maxB <= forcolor[1][5]))
				{
					putText(frame, "GREEN" + std::to_string(detection.confidence).substr(0, 4), cv::Point(box.x + 5, box.y - 10), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 0), 2);
					return RESULT::GREEN;
				}
				else if ((maxR >= forcolor[2][0] && maxR <= forcolor[2][1]) && (maxG >= forcolor[2][2] && maxG <= forcolor[2][3]) && (maxB >= forcolor[2][4] && maxB <= forcolor[2][5]))
				{
					putText(frame, "YELLOW" + std::to_string(detection.confidence).substr(0, 4), cv::Point(box.x + 5, box.y - 10), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 0), 2);
					return RESULT::YELLOW;
				}

			}
		}
	}
	return RESULT::RES_END;
}

void ToolManager::Setserverform(ServerForm* s)
{
	m_Serverform = s;
}

void ToolManager::SetCntrlForm(CntrlForm* c)
{
	m_CntrlForm = c;
}


void ToolManager::SetMainHndl(HWND mhd)
{
	MainHndle = mhd;
}

void ToolManager::SetToolviewhdl(HWND tvhd)
{
	//툴뷰에 cv로생성한 윈도우창 자식으로 넣기 

	Toolviewhandle = tvhd;
	cv::namedWindow("FINAL", 0);
	HWND hWnd = FindWindowA(NULL, "FINAL");
	::SetParent(hWnd, Toolviewhandle);
	::SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_CAPTION);
	::SetWindowPos(hWnd, NULL, 0, 0, 640, 480, SWP_NOZORDER | SWP_FRAMECHANGED);

}

void ToolManager::SetListFormHndle(HWND lhd)
{
	ListFormHndle = lhd;
}



void ToolManager::SetDataDlg(DataInquiryDlg* d)
{
	m_DataInquiryDlg = d;
}

void ToolManager::SetForceQuit(bool set)
{
	ForceQuit = set;
}

void ToolManager::SetKillFrm(bool set)
{
	FrmKilled = set;
}





void ToolManager::SetSpecialOn(bool set)
{
	SpecialOn = set;
}

CTabCtrl* ToolManager::GetTabctrl()
{
	return m_tab;
}


bool ToolManager::GetFramekill()
{
	return FrmKilled;
}

void ToolManager::Mod_Txt(int cur, int idx, CString cstr)
{
	m_DataInquiryDlg->Set_Text(cur, idx, cstr);
}




void ToolManager::findMostFrequentColor(const Mat& roi, int& maxR, int& maxG, int& maxB)
{

	int histSize = 256;
	float range[] = { 0, 256 };
	const float* histRange = { range };
	bool uniform = true, accumulate = false;

	Mat b_hist, g_hist, r_hist;

	vector<Mat> bgr_planes;
	split(roi, bgr_planes);

	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

	maxR = maxG = maxB = 0;
	int maxCountR = 0, maxCountG = 0, maxCountB = 0;
	for (int i = 0; i < histSize; ++i) {
		if (b_hist.at<float>(i) > maxCountB) {
			maxCountB = b_hist.at<float>(i);
			maxB = i;
		}
		if (g_hist.at<float>(i) > maxCountG) {
			maxCountG = g_hist.at<float>(i);
			maxG = i;
		}
		if (r_hist.at<float>(i) > maxCountR) {
			maxCountR = r_hist.at<float>(i);
			maxR = i;
		}
	}
}

void ToolManager::SendResult(RESULT res)
{
	time_t timer;
	struct tm* t;
	timer = time(NULL);
	t = localtime(&timer);
	string time_s = std::to_string(t->tm_year + 1900) + "/" +
		std::to_string(t->tm_mon + 1) + "/" +
		std::to_string(t->tm_mday) + "/" +
		std::to_string(t->tm_hour) + "/" +
		std::to_string(t->tm_min) + "/" +
		std::to_string(t->tm_sec);

	
	if (res == RESULT::YELLOW) {
		if (m_Serverform->GetServerSwitch() == STATUCOLOR::SERVERGREEN)
		{
			m_Serverform->SetAwsColor("yellow");
			m_Serverform->SetAwsFaulty("false");
			m_Serverform->SetDate(time_s);
			m_Serverform->SetAwsFilename("BOX.jpg");
			imwrite("BOX.jpg", frame);
		}
		else
		{
			TEMPINFO res;
			res.filename = "BOX" + to_string(tempboxcnt) + ".jpg";
			res.color = "yellow";
			res.faulty = "false";
			res.date = time_s;
			m_TempVec.emplace_back(res);
			imwrite("BOX" + to_string(tempboxcnt) + ".jpg", frame);
		}
		m_Serverform->SetList(_T(""), _T("PROC:YELLOW"));
		m_Serverform->ClientTCP(_T("ST/PROC:YELLOW/END"));
		Allcount(RESULT::YELLOW);
	}
	else if (res == RESULT::RED) {
		if (m_Serverform->GetServerSwitch() == STATUCOLOR::SERVERGREEN)
		{
			m_Serverform->SetAwsColor("red");
			m_Serverform->SetAwsFaulty("false");
			m_Serverform->SetDate(time_s);
			m_Serverform->SetAwsFilename("BOX.jpg");
			imwrite("BOX.jpg", frame);
		}
		else
		{
			TEMPINFO res;
			res.filename = "BOX" + to_string(tempboxcnt) + ".jpg";
			res.color = "red";
			res.faulty = "false";
			res.date = time_s;
			m_TempVec.emplace_back(res);
			imwrite("BOX" + to_string(tempboxcnt) + ".jpg", frame);
		}
		m_Serverform->SetList(_T(""), _T("PROC:RED"));
		m_Serverform->ClientTCP(_T("ST/PROC:RED/END"));
		Allcount(RESULT::RED);
	}
	else if (res == RESULT::GREEN) {
		if (m_Serverform->GetServerSwitch() == STATUCOLOR::SERVERGREEN)
		{
			m_Serverform->SetAwsColor("green");
			m_Serverform->SetAwsFaulty("false");
			m_Serverform->SetDate(time_s);
			m_Serverform->SetAwsFilename("BOX.jpg");
			imwrite("BOX.jpg", frame);
		}
		else
		{
			TEMPINFO res;
			res.filename = "BOX" + to_string(tempboxcnt) + ".jpg";
			res.color = "green";
			res.faulty = "false";
			res.date = time_s;
			m_TempVec.emplace_back(res);
			imwrite("BOX" + to_string(tempboxcnt) + ".jpg", frame);
		}
		m_Serverform->SetList(_T(""), _T("PROC:GREEN"));
		m_Serverform->ClientTCP(_T("ST/PROC:GREEN/END"));
		Allcount(RESULT::GREEN);
	}
	else if (res == RESULT::FAIL) {
		if (m_Serverform->GetServerSwitch() == STATUCOLOR::SERVERGREEN)
		{
			m_Serverform->SetAwsColor("fail");
			m_Serverform->SetAwsFaulty("true");
			m_Serverform->SetDate(time_s);
			m_Serverform->SetAwsFilename("BOX.jpg");
			imwrite("BOX.jpg", frame);
		}
		else
		{
			TEMPINFO res;
			res.filename = "BOX" + to_string(tempboxcnt) + ".jpg";
			res.color = "fail";
			res.faulty = "true";
			res.date = time_s;
			m_TempVec.emplace_back(res);
			imwrite("BOX" + to_string(tempboxcnt) + ".jpg", frame);
		}
		m_Serverform->SetList(_T(""), _T("PROC:FAIL"));
		m_Serverform->ClientTCP(_T("ST/PROC:FAIL/END"));
		Allcount(RESULT::FAIL);
	}


	++tempboxcnt;

}

void ToolManager::SetProcessState(PROCESSSTATE s)
{
	m_CurState = s;
}

void ToolManager::TempVecSendAll()
{
	if (m_TempVec.size() <= 0)
		return;
	m_Serverform->SetAwsInfo(AWSINFO::AWSTEMPLIST);
}

vector<TEMPINFO> ToolManager::GetVec()
{
	return m_TempVec;
}


void ToolManager::SetReady(bool bReady)
{
	m_bReadyState = bReady;
}

void ToolManager::SetInference()
{
	inf = new Inference("C:\\C\\github\\Vision_P\\MainTool\\block.onnx", cv::Size(640, 480), "C:\\C\\github\\Vision_P\\MainTool\\block.txt", true);
}

void ToolManager::Allcount(RESULT Color)
{
	switch (Color)
	{
	case RESULT::FAIL:
		if (f_count < 4)
		{
			f_count++;
		}
	
		break;
	case RESULT::RED:
		if (r_count < 4)
			r_count++;
	
		break;
	case RESULT::YELLOW:
		if (y_count < 4)
			y_count++;

		break;
	case RESULT::GREEN:
		if (g_count < 4)
		{
			g_count++;
		}
		break;
	default:
		break;
	}

	m_Resform->Invalidate(false);
}

bool ToolManager::SetCap(int set)
{
	return cap.open(set, cv::CAP_DSHOW);
}
