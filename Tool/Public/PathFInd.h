#pragma once
#include "afxwin.h"

#include "Include.h"


// CPathFInd 대화 상자입니다.

class CPathFInd : public CDialog
{
	DECLARE_DYNAMIC(CPathFInd)

public:
	CPathFInd(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPathFInd();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHFIND };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
