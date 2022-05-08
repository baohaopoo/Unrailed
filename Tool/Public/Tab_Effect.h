#pragma once


// CTab_Effect 대화 상자입니다.
#include "Tool_Defines.h"

BEGIN(Tool)

class CTab_Effect : public CDialogEx
{
	DECLARE_DYNAMIC(CTab_Effect)

public:
	CTab_Effect(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTab_Effect();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB_EFFECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};

END