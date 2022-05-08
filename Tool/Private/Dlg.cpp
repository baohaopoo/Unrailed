// Dlg.cpp : ���� �����Դϴ�.
//


#include "Tool_Defines.h"

#include "stdafx.h"
#include "Tool.h"
#include "Dlg.h"
#include "afxdialogex.h"

#include "Tab_Map.h"
#include "Tab_Object.h"
#include "Tab_Player.h"
#include "Tab_Effect.h"

// CDlg ��ȭ �����Դϴ�.

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CDlg, CDialogEx)

CDlg::CDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG, pParent)
{

}

CDlg::~CDlg()
{
}

void CDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}


BEGIN_MESSAGE_MAP(CDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDlg::Change_Tab)
END_MESSAGE_MAP()


// CDlg �޽��� ó�����Դϴ�.


BOOL CDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_Tab.InsertItem(0, _T("Map"));
	m_Tab.InsertItem(1, _T("Object"));
	m_Tab.InsertItem(2, _T("Player"));
	m_Tab.InsertItem(3, _T("Effect"));


	m_Tab.SetCurSel(0);
	CRect rect;
	m_Tab.GetWindowRect(&rect);

	// Map
	m_pTabMap = new CTab_Map;
	m_pTabMap->Create(IDD_TAB_MAP, &m_Tab);
	m_pTabMap->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pTabMap->ShowWindow(SW_SHOW);

	// Object
	m_pTabObject = new CTab_Object;
	m_pTabObject->Create(IDD_TAB_OBJECT, &m_Tab);
	m_pTabObject->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pTabObject->ShowWindow(SW_HIDE);

	// Player
	m_pTabPlayer = new CTab_Player;
	m_pTabPlayer->Create(IDD_TAB_PLAYER, &m_Tab);
	m_pTabPlayer->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pTabPlayer->ShowWindow(SW_HIDE);

	// Effect
	m_pTabEffect = new CTab_Effect;
	m_pTabEffect->Create(IDD_TAB_EFFECT, &m_Tab);
	m_pTabEffect->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pTabEffect->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CDlg::Change_Tab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//CGameInstance* m_pGameInstance = CGameInstance::GetInstance();

	int sel = m_Tab.GetCurSel();

	switch (sel)
	{
	case 0:
		m_pTabMap->ShowWindow(SW_SHOW);
		m_pTabObject->ShowWindow(SW_HIDE);
		m_pTabPlayer->ShowWindow(SW_HIDE);
		m_pTabEffect->ShowWindow(SW_HIDE);
		break;

	case 1:
		m_pTabMap->ShowWindow(SW_HIDE);
		m_pTabObject->ShowWindow(SW_SHOW);
		m_pTabPlayer->ShowWindow(SW_HIDE);
		m_pTabEffect->ShowWindow(SW_HIDE);
		break;

	case 2:
		m_pTabMap->ShowWindow(SW_HIDE);
		m_pTabObject->ShowWindow(SW_HIDE);
		m_pTabPlayer->ShowWindow(SW_SHOW);
		m_pTabEffect->ShowWindow(SW_HIDE);
		break;

	case 3:
		m_pTabMap->ShowWindow(SW_HIDE);
		m_pTabObject->ShowWindow(SW_HIDE);
		m_pTabPlayer->ShowWindow(SW_HIDE);
		m_pTabEffect->ShowWindow(SW_SHOW);
		break;

	}

	*pResult = 0;
}