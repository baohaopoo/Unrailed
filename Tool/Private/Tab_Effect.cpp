// Tab_Effect.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "Tab_Effect.h"
#include "afxdialogex.h"


// CTab_Effect 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTab_Effect, CDialogEx)

CTab_Effect::CTab_Effect(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TAB_EFFECT, pParent)
{

}

CTab_Effect::~CTab_Effect()
{
}

void CTab_Effect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTab_Effect, CDialogEx)
END_MESSAGE_MAP()


// CTab_Effect 메시지 처리기입니다.
