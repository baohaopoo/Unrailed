
// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "Tool.h"

#include "MainFrm.h"
#include "MyForm.h"
#include "ToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)

	ON_WM_CREATE()

	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	/*if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("���� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("���� ǥ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: ���� ������ ��ŷ�� �� ���� �Ϸ��� �� �� ���� �����Ͻʽÿ�.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);*/


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	// ���� ��ũ���� �ػ󵵸� ���´�.
	m_WinResolution.x = GetSystemMetrics(SM_CXSCREEN);
	m_WinResolution.y = GetSystemMetrics(SM_CYSCREEN);


	cs.x = 0;
	cs.y = 0;
	cs.cx = m_WinResolution.x;
	cs.cy = m_WinResolution.y;

	cs.style &= ~WS_THICKFRAME; //â ũ�� ���� ��� ��Ȱ��ȭ
	cs.style &= ~WS_MAXIMIZEBOX; //â �ִ�ȭ��ư ��Ȱ��ȭ


	return TRUE;
}

// CMainFrame ����

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame �޽��� ó����


//  �� ��ü�� �����ϴ� ������ ȣ��(�� ������ ���ų� �ڽŸ��� �並 ����)
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	// CreateStatic : ���ҵ� â�� ����� ���� �Լ�(�θ� ������ �ּ�, ���� ��, ���� ��, â ��Ÿ��, �ڽ�â ID)
#pragma region 2����
	m_MainSplitter.CreateStatic(this, 1, 2);
	// CreateView : ���� â�� ǥ���� view â�� �����ϴ� �Լ�(��, ��, ��ġ�� viewâ�� ������ ������, �ʱ� ũ��, pContext)
	m_MainSplitter.CreateView(0, 0, RUNTIME_CLASS(CMyForm), CSize(m_WinResolution.x / 4, m_WinResolution.y), pContext);
	m_MainSplitter.CreateView(0, 1, RUNTIME_CLASS(CToolView), CSize(m_WinResolution.x / 4 * 3, m_WinResolution.y), pContext);
#pragma endregion 2����

	//// �� ��° ���� ������ ����

	// WS_CHILD : �ڽ� â�̶�� �ǹ�
	// WS_VISIBLE : ���� �� �ٷ� ȭ�鿡 ǥ���ϰڴ� �ǹ�
	//m_ToolSplitter.CreateStatic(&m_MainSplitter,
	//								2,
	//								1, 
	//								WS_CHILD | WS_VISIBLE, 
	//								m_MainSplitter.IdFromRowCol(0, 1));




	//// ���� �������� ���� ȭ�鿡 MyFrom class�� �����.
	//m_MainSplitter.CreateView(0, 0, RUNTIME_CLASS(CMyForm), CSize(lpcs->cx / 4, lpcs->cy), pContext);
	//m_MainSplitter.SetColumnInfo(0, lpcs->cx / 4, 10);
	//m_ToolSplitter.SetRowInfo(0, lpcs->cy / 5 * 4 / 4, 10);




	//// ���� ������ �������� �Ʒ��� ToolView ����
	//m_ToolSplitter.CreateView(1, 0, RUNTIME_CLASS(CToolView), CSize(lpcs->cx / 4 * 3, lpcs->cy / 5 * 4), pContext);




	//// ī�޶� ���ø��͸� ���� ������ �������� ���� ���� ĭ 6���� ������ �����Ѵ�.
	//m_CameraSplitter.CreateStatic(&m_ToolSplitter, 1, 6, WS_CHILD | WS_VISIBLE, m_ToolSplitter.IdFromRowCol(0, 0));

	//// ī�޶� ���ø��Ϳ� ���ʴ��, ī�޶� ��,��,��,����,��,�Ʒ� �ʿ��� ������Ʈ ������ �ٶ󺸴� ī�޶� �����ϴ� viewâ ����ϱ�
	//m_CameraSplitter.CreateView(0, 0, RUNTIME_CLASS(CFrontView), CSize(lpcs->cx / 8, lpcs->cy / 5 * 4 / 6), pContext);
	//m_CameraSplitter.CreateView(0, 1, RUNTIME_CLASS(CBackView), CSize(lpcs->cx / 8, lpcs->cy / 5 * 4 / 6), pContext);
	//m_CameraSplitter.CreateView(0, 2, RUNTIME_CLASS(CLeftView), CSize(lpcs->cx / 8, lpcs->cy / 5 * 4 / 6), pContext);
	//m_CameraSplitter.CreateView(0, 3, RUNTIME_CLASS(CRightView), CSize(lpcs->cx / 8, lpcs->cy / 5 * 4 / 6), pContext);
	//m_CameraSplitter.CreateView(0, 4, RUNTIME_CLASS(CTopView), CSize(lpcs->cx / 8, lpcs->cy / 5 * 4 / 6), pContext);
	//m_CameraSplitter.CreateView(0, 5, RUNTIME_CLASS(CBottomView), CSize(lpcs->cx / 8, lpcs->cy / 5 * 4 / 6), pContext);

	//for (int i = 0; i < 6; ++i) {
	//	m_CameraSplitter.SetRowInfo(0, lpcs->cy / 5, 10);
	//	m_CameraSplitter.SetColumnInfo(i, 1.f * lpcs->cx / 8.3, 10);
	//}

	//SetColumnInfo(�� ��ȣ, ���� ũ��, ��� ������ �ּ� ũ��)
	return TRUE; //CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWnd::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// â�� ���̴� ������ ���α׷� ���� �� �������� �Ǻ��ϱ� ���� ����
	static bool once = true;

	// â�� ���̴� ������ ���α׷� ������ �������� �˻��ϴ� ����
	if (bShow && !IsWindowVisible() && once)
	{
		// ���α׷� ������ �̰��� �ѹ��� ����ǵ��� once�� false��...
		once = false;

		// â�� ��ġ�� ���� ������ ���� ���� min, max
		// min: â�� ���� ����� ��ǥ
		// max: â�� ���� �ϴ��� ��ǥ
		// �Ʒ��� ���� ���ڴ� ���Ƿ� ����� ������
		POINT min, max;
		min.x = 0;
		min.y = 0;
		max.x = m_WinResolution.x;
		max.y = m_WinResolution.y;

		WINDOWPLACEMENT lwp;
		lwp.length = sizeof(WINDOWPLACEMENT);
		lwp.flags = SW_SHOWMINIMIZED | WPF_SETMINPOSITION;
		lwp.showCmd = SW_SHOW;
		lwp.ptMinPosition = min;
		lwp.ptMaxPosition = max;
		lwp.rcNormalPosition = CRect(min, max);
		SetWindowPlacement(&lwp);
	}
}
