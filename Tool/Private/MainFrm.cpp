
// MainFrm.cpp : CMainFrame 클래스의 구현
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
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
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
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);*/


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	// 현재 스크린의 해상도를 얻어온다.
	m_WinResolution.x = GetSystemMetrics(SM_CXSCREEN);
	m_WinResolution.y = GetSystemMetrics(SM_CYSCREEN);


	cs.x = 0;
	cs.y = 0;
	cs.cx = m_WinResolution.x;
	cs.cy = m_WinResolution.y;

	cs.style &= ~WS_THICKFRAME; //창 크기 조절 기능 비활성화
	cs.style &= ~WS_MAXIMIZEBOX; //창 최대화버튼 비활성화


	return TRUE;
}

// CMainFrame 진단

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


// CMainFrame 메시지 처리기


//  뷰 객체를 생성하는 시점에 호출(뷰 생성을 막거나 자신만의 뷰를 생성)
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	// CreateStatic : 분할된 창을 만들기 위한 함수(부모 윈도우 주소, 행의 수, 열의 수, 창 스타일, 자식창 ID)
#pragma region 2분할
	m_MainSplitter.CreateStatic(this, 1, 2);
	// CreateView : 분할 창에 표시할 view 창을 생성하는 함수(행, 열, 배치할 view창을 생성한 포인터, 초기 크기, pContext)
	m_MainSplitter.CreateView(0, 0, RUNTIME_CLASS(CMyForm), CSize(m_WinResolution.x / 4, m_WinResolution.y), pContext);
	m_MainSplitter.CreateView(0, 1, RUNTIME_CLASS(CToolView), CSize(m_WinResolution.x / 4 * 3, m_WinResolution.y), pContext);
#pragma endregion 2분할

	//// 두 번째 분할 윈도우 생성

	// WS_CHILD : 자식 창이라는 의미
	// WS_VISIBLE : 생성 후 바로 화면에 표시하겠는 의미
	//m_ToolSplitter.CreateStatic(&m_MainSplitter,
	//								2,
	//								1, 
	//								WS_CHILD | WS_VISIBLE, 
	//								m_MainSplitter.IdFromRowCol(0, 1));




	//// 메인 윈도우의 왼쪽 화면에 MyFrom class를 만든다.
	//m_MainSplitter.CreateView(0, 0, RUNTIME_CLASS(CMyForm), CSize(lpcs->cx / 4, lpcs->cy), pContext);
	//m_MainSplitter.SetColumnInfo(0, lpcs->cx / 4, 10);
	//m_ToolSplitter.SetRowInfo(0, lpcs->cy / 5 * 4 / 4, 10);




	//// 메인 윈도우 오른쪽의 아래에 ToolView 생성
	//m_ToolSplitter.CreateView(1, 0, RUNTIME_CLASS(CToolView), CSize(lpcs->cx / 4 * 3, lpcs->cy / 5 * 4), pContext);




	//// 카메라 스플리터를 메인 윈도우 오른쪽의 위에 가로 칸 6개를 가지고 생성한다.
	//m_CameraSplitter.CreateStatic(&m_ToolSplitter, 1, 6, WS_CHILD | WS_VISIBLE, m_ToolSplitter.IdFromRowCol(0, 0));

	//// 카메라 스플리터에 차례대로, 카메라 앞,뒤,왼,오른,위,아래 쪽에서 오브젝트 원점을 바라보는 카메라 적용하는 view창 사용하기
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

	//SetColumnInfo(열 번호, 열의 크기, 허용 가능한 최소 크기)
	return TRUE; //CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWnd::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 창이 보이는 시점이 프로그램 실행 후 최초인지 판별하기 위한 변수
	static bool once = true;

	// 창이 보이는 시점이 프로그램 실행후 최초인지 검사하는 구문
	if (bShow && !IsWindowVisible() && once)
	{
		// 프로그램 실행후 이곳은 한번만 실행되도록 once를 false로...
		once = false;

		// 창의 위치에 대한 정보를 담을 변수 min, max
		// min: 창의 좌측 상단의 좌표
		// max: 창의 우측 하단의 좌표
		// 아래에 사용된 숫자는 임의로 사용한 숫자임
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
