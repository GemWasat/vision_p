// RobotForm.cpp: 구현 파일
//

#include "pch.h"
#include "MainTool.h"
#include "afxdialogex.h"
#include "RobotForm.h"


// RobotForm 대화 상자

IMPLEMENT_DYNAMIC(RobotForm, CDialogEx)

RobotForm::RobotForm(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RobotFrom, pParent)
{

}

RobotForm::~RobotForm()
{
}

void RobotForm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(RobotForm, CDialogEx)
END_MESSAGE_MAP()


// RobotForm 메시지 처리기
