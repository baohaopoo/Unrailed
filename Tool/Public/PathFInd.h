#pragma once
#include "afxwin.h"

#include "Include.h"


// CPathFInd ��ȭ �����Դϴ�.

class CPathFInd : public CDialog
{
	DECLARE_DYNAMIC(CPathFInd)

public:
	CPathFInd(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPathFInd();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHFIND };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnListBox();
	afx_msg void OnSaveData();
	afx_msg void OnLoadData();

	afx_msg void OnDropFiles(HDROP hDropInfo);

public:
	// control
	CListBox m_ListBox;

	// value

	// general
	list<IMGPATH*>		m_PathList;

};
