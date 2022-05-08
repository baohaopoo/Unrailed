#pragma once

#include "afxcmn.h"
#include "Tool_Defines.h"
// CDlg 대화 상자입니다.

BEGIN(Tool)
class CTab_Map;
class CTab_Object;
class CTab_Player;
class CTab_Effect;
END

BEGIN(Tool)
class CDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg)

public:
	CDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG };
#endif

public:
	// Control 변수
	CTabCtrl m_Tab;

	// General 변수
	CTab_Map*		m_pTabMap;
	CTab_Object*	m_pTabObject;
	CTab_Player*	m_pTabPlayer;
	CTab_Effect*	m_pTabEffect;

public:
	const CTabCtrl&		GetTabCtrl(void) const { return m_Tab; }
	CTab_Map*		GetTabMap(void) { return m_pTabMap; }
	CTab_Object*	GetTabObject(void) { return m_pTabObject; }
	const CTab_Player*	GetTabPlayer(void) const { return m_pTabPlayer; }
	const CTab_Effect*	GetTabEffect(void) const { return m_pTabEffect; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void Change_Tab(NMHDR *pNMHDR, LRESULT *pResult);
};

END