#pragma once


// 로봇 상태 구조체
typedef struct RobotInfo {
	int x_limit, y_limit, pos, home;
}RINFO;


// 서버 스위치 
enum class AWSINFO :int {
	STAY = 0,
	SEVERSTART,
	AWSSEND,
	AWSCHEAK,
	AWSEXIT,
	AWSLIST,
	AWSTEMPLIST,
	AWSMODIFY,
	AWSDELETE
};


// 서버 리스트 저장 구조체
typedef struct {
	string id;
	string color;
	string faulty;
	string date;
	string url;

}AWSLIST;


// 통신 상태 스위치
enum class STATUCOLOR : int
{
	STAY = 0,
	SERVERRED,
	SERVERYELLOW,
	SERVERGREEN,
	SOCKETRED,
	SOCKETYELLOW,
	SOCKETGREEN,
};


// 쓰레드 자원 해제
enum class COLORTHREAD : int
{
	THREADRUN = 0,
	THREADEXIT
};


// 통신 시퀀스
enum class PROCESSSTATE : int
{
	STANDBY = 0,
	WAIT_CAM_GRAB,
	INSPECT,
	ABNORMAL,
	PROCESS_END
};


// 서버에 저장된 리스트 정보
typedef struct TempInfo{

	string filename;
	string color;
	string faulty;
	string date;

}TEMPINFO;