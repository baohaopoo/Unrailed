// Tab_Player.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "Tab_Player.h"
#include "afxdialogex.h"


// CTab_Player 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTab_Player, CDialogEx)

CTab_Player::CTab_Player(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TAB_PLAYER, pParent)
{

}

CTab_Player::~CTab_Player()
{
}

void CTab_Player::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTab_Player, CDialogEx)
END_MESSAGE_MAP()


// CTab_Player 메시지 처리기입니다.
