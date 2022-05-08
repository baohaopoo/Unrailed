// PopUp.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "PopUp.h"
#include "afxdialogex.h"


// CPopUp 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPopUp, CDialog)

CPopUp::CPopUp(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_POPUP, pParent)
{

}

CPopUp::~CPopUp()
{
}

void CPopUp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPopUp, CDialog)
END_MESSAGE_MAP()


// CPopUp 메시지 처리기입니다.
