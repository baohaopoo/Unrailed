
// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#pragma once

#include "Include.h"
#include "GameInstance.h"

class CMainFrame : public CFrameWnd
{
	
protected: // serialization������ ��������ϴ�.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// �����Դϴ�.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

public:
	// â ������ ���� Ŭ����, CWnd �ڽ� Ŭ����
	CSplitterWnd		m_MainSplitter;		// ����(�¿� 2������ �������� ������)
	CSplitterWnd		m_ToolSplitter;		// Tool(�츦 2������ ������ ������
	CSplitterWnd		m_CameraSplitter;	// Camera(���� ���� 6������ ������ ������)

private:
	POINT m_WinResolution;	// ȭ�� �ػ�

// ������ �޽��� �� �Լ�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

public:
	const POINT& GetResolution (void) const { return m_WinResolution; }
};


