#pragma once


// CTab_Player 대화 상자입니다.
#include "Tool_Defines.h"

BEGIN(Tool)

class CTab_Player : public CDialogEx
{
	DECLARE_DYNAMIC(CTab_Player)

public:
	CTab_Player(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTab_Player();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB_PLAYER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};

END