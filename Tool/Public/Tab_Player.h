#pragma once


// CTab_Player ��ȭ �����Դϴ�.
#include "Tool_Defines.h"

BEGIN(Tool)

class CTab_Player : public CDialogEx
{
	DECLARE_DYNAMIC(CTab_Player)

public:
	CTab_Player(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTab_Player();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB_PLAYER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

END