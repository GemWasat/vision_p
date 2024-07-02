#pragma once

class Infernece;
class ServerForm;
class ResultForm;
class CntrlForm;
class DataInquiryDlg;
class ListForm;


enum class PROCESSSTATE;

enum RESULT
{
	FAIL, RED, YELLOW, GREEN, RES_NONE, RES_END
};


class ToolManager
{
private:
	ToolManager(ToolManager&);

public:
	ToolManager();
	~ToolManager();


private:
	static ToolManager* m_pInstance;

public:
	static ToolManager* GetInstance();
	void DestroyInstance();

public:
	void Initialize();
	bool Update(double t);
	void LateUpdate();
	void Render();

public:
	RESULT Detect();   //검출코드
	void Save();  //현재 프레임 이미지 저장
	void ShowPic(string Filename);    //파일열기한 이미지 보여줄려는용
	void RenderImg(CStatic* p, CString filepath);  //파일패스에있는 이미지 렌더링

	RESULT Detecttest();   //검출코드

public:
	void Setserverform(ServerForm* s);
	void SetCntrlForm(CntrlForm* c);
	void SetMainHndl(HWND mhd);
	void SetToolviewhdl(HWND tvhd);
	void SetListFormHndle(HWND tvhd);
	void SetDataDlg(DataInquiryDlg* d);
	void SetForceQuit(bool set);
	void SetKillFrm(bool set);
	void SetSpecialOn(bool set);
	void SetProcessState(PROCESSSTATE s);  //상태변경

public:
	CTabCtrl* GetTabctrl();
	bool  GetFramekill();

public:
	void Mod_Txt(int cur, int idx, CString cstr);
	void findMostFrequentColor(const Mat& roi, int& maxR, int& maxG, int& maxB); //색검출
	void SendResult(RESULT res);     //검출결과전송

public:
	void TempVecSendAll();     //임시벡터 내부 전송 
	vector<TEMPINFO>  GetVec(); //임시벡터 겟


public:
	HWND MainHndle;
	HWND Toolviewhandle;
	HWND ListFormHndle;
	Mat frame;  //메인프레임
	Mat roi;    //검사후 추출되는 roi
	VideoCapture cap;
	Inference* inf; //for onnx

public:
	bool ForceQuit = false; //update내부 펄스 만들기 위한 임시변수
	bool FrmKilled = false; // 캠 on off 여부
	bool SpecialOn = false; // 파일 열어서 사진 볼때 켜지는 변수
	bool bGrap = false;     // 그랩 신호 여부 
public:
	double dfps;            //fps
	double fpscnt;			//fps카운트 용
	double dtime;           //델타타임
	double updatetime;      //임시 델타타임 저장용
	bool onlycam = false;   //검사 안하는 캠키는 변수                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
	bool cameraR = true;   //      
	bool ServerSwitch = true;


	bool bDone = false;


public:
	CString m_strPickinLst;  //사진이름 리스트탭에 쓰임
public:
	ServerForm* m_Serverform;
	CTabCtrl* m_tab;
	ResultForm* m_Resform;
	CntrlForm* m_CntrlForm;
	DataInquiryDlg* m_DataInquiryDlg=nullptr;
public:
	int maxR, maxG, maxB;      //검사된 rgb값
	RESULT m_Res;				//검사 결과


public:
	string specFileName;         //파일 열어서 사진 볼때 쓰이는 파일이름 변수

public:
	int testcnt = 0;               //사진 저장할때 뒤에 번호 붙이는 용으로 쓰이는 변수
	int tempboxcnt = 0;          //서버 켜지기전 박스결과 이미지 저장할때 뒤에 번호 붙이는 용으로 쓰는 변수 

	//새로 만들것들 
public:
	PROCESSSTATE m_OldState;       //이전 상태
	PROCESSSTATE m_CurState;       //현재 상태
public:
	vector<TEMPINFO> m_TempVec;    //서버 켜지기전 박스검출 결과 임시 넣어놓을 백터
public:
	bool m_bReadyState = false;    //처음 스탠바이상태에서 쓰이는 불변수
	void SetReady(bool bReady);
	bool SetCap(int set);
	void SetInference();




public:

	//0 red / 1 yellow / 2 green
	//0~1 r 2~3 g 4~5 b
	int forcolor[3][6];


	int g_count = 0;
	int y_count = 0;
	int r_count = 0;
	int f_count = 0;

	void Allcount(RESULT Color); // 탐지한 객체 갯수


	




};
