#pragma once


// CPopUp 대화 상자입니다.

class CPopUp : public CDialog
{
	DECLARE_DYNAMIC(CPopUp)

public:
	CPopUp(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPopUp();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POPUP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
