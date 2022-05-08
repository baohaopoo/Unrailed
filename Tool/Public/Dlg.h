#pragma once

#include "afxcmn.h"
#include "Tool_Defines.h"
// CDlg ��ȭ �����Դϴ�.

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
	CDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG };
#endif

public:
	// Control ����
	CTabCtrl m_Tab;

	// General ����
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void Change_Tab(NMHDR *pNMHDR, LRESULT *pResult);
};

END