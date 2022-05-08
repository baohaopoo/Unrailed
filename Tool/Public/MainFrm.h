
// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once

#include "Include.h"
#include "GameInstance.h"

class CMainFrame : public CFrameWnd
{
	
protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

public:
	// 창 분할을 위한 클래스, CWnd 자식 클래스
	CSplitterWnd		m_MainSplitter;		// 메인(좌우 2갈래로 갈라지는 윈도우)
	CSplitterWnd		m_ToolSplitter;		// Tool(우를 2갈래로 가르는 윈도우
	CSplitterWnd		m_CameraSplitter;	// Camera(우의 위를 6갈래로 가르는 윈도우)

private:
	POINT m_WinResolution;	// 화면 해상도

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

public:
	const POINT& GetResolution (void) const { return m_WinResolution; }
};


