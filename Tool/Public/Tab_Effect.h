#pragma once


// CTab_Effect ��ȭ �����Դϴ�.
#include "Tool_Defines.h"

BEGIN(Tool)

class CTab_Effect : public CDialogEx
{
	DECLARE_DYNAMIC(CTab_Effect)

public:
	CTab_Effect(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTab_Effect();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB_EFFECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

END