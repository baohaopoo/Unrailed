// ToolView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "ToolView.h"
#include "MainFrm.h"
#include "MyForm.h"
#include "Tool_Defines.h"

// ī�޶�
#include "Camera_Default.h"
#include "Camera_Observer.h"

// ������Ʈ
#include "Terrain.h"
#include "Player.h"
#include "Cube_Object.h"
#include "PickingObject.h"
#include "Unbreakable_Rock.h"

#include "Rock.h"
#include "Tree.h"

#include "Material_Iron.h"
#include "Material_Wood.h"

#include "Axe.h"
#include "PickAx.h"
#include "Bucket.h"

#include "Camel.h"

#include "TrainStation.h"
#include "Train_Head.h"
#include "Train_Material.h"
#include "Train_Rail.h"
#include "Train_Water.h"
#include "Train_Dynamite.h"

#include "Rail_Straight.h"
#include "Rail_Curve.h"
#include "Rail_Ending.h"


// ����
#include "Level_Map.h"
#include "Level_Object.h"
#include "Level_Player.h"
#include "Level_Effect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND	g_hWnd;
POINT	Resolution;
float	g_ToolViewAspect;

// CToolView
IMPLEMENT_DYNCREATE(CToolView, CFormView)

CToolView::CToolView()
	: CFormView(IDD_TOOLVIEW), m_pInstance(CGameInstance::GetInstance()), m_eCurrLevel(LEVEL_STATIC)
{
}

CToolView::~CToolView()
{
}

void CToolView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CToolView, CFormView)
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CToolView �����Դϴ�.

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CToolView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CToolView �޽��� ó�����Դϴ�.


void CToolView::OnInitialUpdate()	//ToolView��� â�� ������� �� �Ͼ�� �ϵ��� �ִ� �Լ�
{
	CFormView::OnInitialUpdate();
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	
	this->GetWindowRect(m_rectCurHist);
	g_ToolViewAspect = 1.f * m_rectCurHist.Width() / m_rectCurHist.Height();

	ViewInitialize();	// MFC ToolView �ʱ�ȭ

	// ���� �ʱ�ȭ
	CGraphic_Device::GRAPHICDESC graphicDesc{};
	graphicDesc.hWnd = g_hWnd;

	CMainFrame* m_pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	POINT pt = m_pMain->GetResolution();

	Resolution.x = pt.x;
	Resolution.y = pt.y;
	graphicDesc.iWinCX = pt.x;
	graphicDesc.iWinCY = pt.y;

	if (FAILED(m_pInstance->Initialize_Engine(AfxGetInstanceHandle(), LEVEL_END, graphicDesc, &m_pGraphic_Device)))
		return;

	if (FAILED(m_pInstance->InitPickManager(graphicDesc.hWnd, m_rectCurHist.Width(), m_rectCurHist.Height())))
		return;

	SetUp_DefaultRenderState();
	Ready_Prototype_Component();	// ����� component ��ü STATIC_LEVEL�� �߰�
	Ready_Prototype_GameObject();	// ����� Object ��ü �߰�

	OpenLevel();
}


void CToolView::SetUp_DefaultRenderState()
{
	if (nullptr == m_pGraphic_Device)
		return;

	/* ���� �� ���ӿ� �ʿ��� �⺻ ������ ���¸� �����Ѵ�. */
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	/* �ؽ��ķκ��� ���� �������� �Ӽ��� �����ϳ�.  */
	/*m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);*/

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
}


void CToolView::ViewInitialize()		// MFC ToolView �ʱ�ȭ
{
	//AfxGetMainWnd : ������ ���� �����츦 ��ȯ�ϴ� ���� �Լ�
	// �ڽ� Ÿ������ �� ��ȯ�Ͽ� ��ȯ��
	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();

	RECT	rcWnd{};

	// GetWindowRect : ���� ������ â�� ��Ʈ ������ ������ �Լ�
	pMainFrm->GetWindowRect(&rcWnd);

	// SetRect : ������ ���ڰ����� rect������ �����ϴ� �Լ�
	// ���� 0,0�������� â�� ��Ʈ�� �����ϰ� �ִ� ��Ȳ
	SetRect(&rcWnd, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);

	RECT	rcMainView{};
	//GetClientRect : ���� view â�� rect ������ ������ �Լ�
	GetClientRect(&rcMainView);

	// �����Ӱ� viewâ�� ����, ���� ���� ���Ѵ�.
	float	fRowFrm = float(rcWnd.right - rcMainView.right);
	float	fColFrm = float(rcWnd.bottom - rcMainView.bottom);

	// SetWindowPos: ���ڰ� ��� ���Ӱ� ������ ��ġ�� ũ�⸦ �����ϴ� �Լ�
	// 1���� : ��ġ�� �������� Z������ ���� ������
	// 2, 3���� : left, top ��ǥ
	// 4, 5���� : ����, ���� â ������
	// 6���� : SWP_NOZORDER : ���� ������ �����ϰڴٴ� �÷��� ��, ���� null�� ��� ���� ������ ���� �ʰڴ�.

	pMainFrm->SetWindowPos(nullptr, START_WIDTH, 100, int(WINCX + fRowFrm), int(WINCY + fColFrm), SWP_NOZORDER);

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	g_hWnd = m_hWnd;

}

void CToolView::Ready_Prototype_Component()
{
	if (FAILED(m_pInstance->Add_Prototype(LEVEL_STATIC, Prototype_Compo_Renderer, m_pRenderer = CRenderer::Create(m_pGraphic_Device))))
		return;


	/* For.Prototype_Component_Transform */
	if (FAILED(m_pInstance->Add_Prototype(LEVEL_STATIC, Prototype_Compo_Transform, CTransform::Create(m_pGraphic_Device))))
		return;


	// VIBuffer
	if (FAILED(m_pInstance->Add_Prototype(LEVEL_STATIC, Prototype_Compo_VIB_Rect, CVIBuffer_Rect::Create(m_pGraphic_Device))))
		return;
	if (FAILED(m_pInstance->Add_Prototype(LEVEL_STATIC, Prototype_Compo_VIB_Terrain, CVIBuffer_Terrain::Create(m_pGraphic_Device, 10, 10))))
		return;
	if (FAILED(m_pInstance->Add_Prototype(LEVEL_STATIC, Prototype_Compo_VIB_Cube, CVIBuffer_Cube::Create(m_pGraphic_Device))))
		return;
	if (FAILED(m_pInstance->Add_Prototype(LEVEL_STATIC, Prototype_Compo_VIB_Object, CVIBuffer_Object::Create(m_pGraphic_Device))))
		return;


	Ready_Prototype_Texture();
}

void CToolView::Ready_Prototype_Texture()
{
	// Texture
	if (FAILED(m_pInstance->Add_Prototype(LEVEL_STATIC, Prototype_Compo_Texture_Default, CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Temp/Default%d.jpg"), 2))))
		return;
	if (FAILED(m_pInstance->Add_Prototype(LEVEL_STATIC, Prototype_Compo_Texture_Terrain, CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Temp/Terrain/Grass_%d.tga"), 2))))
		return;

	int a = 0;
	if (FAILED(Ready_Prototype_Texture_SeongUk())) {
		//MSG_BOX(TEXT("Failed to Load SeongUk Texture"));
		a = 1;
		return;
	}
	if (FAILED(Ready_Prototype_Texture_MinHee())) {
		//MSG_BOX(TEXT("Failed to Load _MinHee Texture"));
		a = 1;
		return;
	}
	if (FAILED(Ready_Prototype_Texture_JuhYeon())) {
		//MSG_BOX(TEXT("Failed to Load JuhYeon Texture"));
		a = 1;
		return;
	}
	if (FAILED(Ready_Prototype_Texture_HyunWoo())) {
		//MSG_BOX(TEXT("Failed to Load HyunWoo Texture"));
		a = 1;
		return;
	}
}

HRESULT CToolView::Ready_Prototype_Texture_SeongUk()
{
	return S_OK;
}

HRESULT CToolView::Ready_Prototype_Texture_MinHee()
{
	// Ÿ�� 6��
	if (FAILED(m_pInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MinHee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/MinHee/Tile%d.dds"), 6))))
		return E_FAIL;

	return S_OK;
}

HRESULT CToolView::Ready_Prototype_Texture_JuhYeon()
{
	// Ÿ�� 12��
	if (FAILED(m_pInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_JuhYeon"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/JuhYeon/Tile%d.dds"), 12))))
		return E_FAIL;

	return S_OK;
}

HRESULT CToolView::Ready_Prototype_Texture_HyunWoo()
{
	// Ÿ�� 12��
	if (FAILED(m_pInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_HyunWoo"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/HyunWoo/Tile%d.dds"), 58))))
		return E_FAIL;

	// ����Ʈ 2��
	if (FAILED(m_pInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/HyunWoo/Effect/Texture%d.dds"), 2))))
		return E_FAIL;

	return S_OK;
}

void CToolView::Ready_Prototype_GameObject()
{
	if (FAILED(m_pInstance->Add_Prototype(Prototype_Object_Terrain, CTerrain::Create(m_pGraphic_Device))))
		return;

	if (FAILED(m_pInstance->Add_Prototype(TEXT("Prototype_GameObject_Unbreakable_Rock"), CUnbreakable_Rock::Create(m_pGraphic_Device))))
		return;

	if (FAILED(m_pInstance->Add_Prototype(TEXT("Prototype_GameObject_MapCube"), CCube_Object::Create(m_pGraphic_Device))))
		return;





	if (FAILED(m_pInstance->Add_Prototype(TEXT("Prototype_GameObject_Rock"), CRock::Create(m_pGraphic_Device))))
		return;

	if (FAILED(m_pInstance->Add_Prototype(TEXT("Prototype_GameObject_Tree"), CTree::Create(m_pGraphic_Device))))
		return;






	if (FAILED(m_pInstance->Add_Prototype(TEXT("Prototype_GameObject_Iron"), CMaterial_Iron::Create(m_pGraphic_Device))))
		return;

	if (FAILED(m_pInstance->Add_Prototype(TEXT("Prototype_GameObject_Wood"), CMaterial_Wood::Create(m_pGraphic_Device))))
		return;






	if (FAILED(m_pInstance->Add_Prototype(TEXT("Prototype_GameObject_Axe"), CAxe::Create(m_pGraphic_Device))))
		return;

	if (FAILED(m_pInstance->Add_Prototype(TEXT("Prototype_GameObject_PickAx"), CPickAx::Create(m_pGraphic_Device))))
		return;

	if (FAILED(m_pInstance->Add_Prototype(TEXT("Prototype_GameObject_Bucket"), CBucket::Create(m_pGraphic_Device))))
		return;





	if (FAILED(m_pInstance->Add_Prototype(TEXT("Prototype_GameObject_Camel"), CCamel::Create(m_pGraphic_Device))))
		return;





	if (FAILED(m_pInstance->Add_Prototype(TEXT("Prototype_GameObject_TrainStation"), CTrainStation::Create(m_pGraphic_Device))))
		return;





	if (FAILED(m_pInstance->Add_Prototype(TEXT("Prototype_GameObject_Train_Head"), CTrain_Head::Create(m_pGraphic_Device))))
		return;

	if (FAILED(m_pInstance->Add_Prototype(TEXT("Prototype_GameObject_Train_Material"), CTrain_Material::Create(m_pGraphic_Device))))
		return;

	if (FAILED(m_pInstance->Add_Prototype(TEXT("Prototype_GameObject_Train_Rail"), CTrain_Rail::Create(m_pGraphic_Device))))
		return;

	if (FAILED(m_pInstance->Add_Prototype(TEXT("Prototype_GameObject_Train_Water"), CTrain_Water::Create(m_pGraphic_Device))))
		return;

	if (FAILED(m_pInstance->Add_Prototype(TEXT("Prototype_GameObject_Train_Dynamite"), CTrain_Dynamite::Create(m_pGraphic_Device))))
		return;








	if (FAILED(m_pInstance->Add_Prototype(TEXT("Prototype_GameObject_Rail"), CRail::Create(m_pGraphic_Device))))
		return;
	if (FAILED(m_pInstance->Add_Prototype(TEXT("Prototype_GameObject_Rail_End"), CRail_Ending::Create(m_pGraphic_Device))))
		return;




	if (FAILED(m_pInstance->Add_Prototype(Prototype_Object_Player, CPlayer::Create(m_pGraphic_Device))))
		return;





	if (FAILED(m_pInstance->Add_Prototype(Prototype_Object_CubeObject, CCube_Object::Create(m_pGraphic_Device))))
		return;

	if (FAILED(m_pInstance->Add_Prototype(Prototype_Object_PickingObject, CPickingObject::Create(m_pGraphic_Device))))
		return;

	if (FAILED(m_pInstance->Add_Prototype(Prototype_Object_Camera_Default, CCamera_Default::Create(m_pGraphic_Device))))
		return;

	if (FAILED(m_pInstance->Add_Prototype(Prototype_Object_Camera_Observer, CCamera_Observer::Create(m_pGraphic_Device))))
		return;
}

void CToolView::OpenLevel()
{
	if (FAILED(m_pInstance->SetLevel(LEVEL_MAP, CLevel_Map::Create(m_pGraphic_Device))))
		return;
	if (FAILED(m_pInstance->SetLevel(LEVEL_OBJECT, CLevel_Object::Create(m_pGraphic_Device))))
		return;
	if (FAILED(m_pInstance->SetLevel(LEVEL_PLAYER, CLevel_Player::Create(m_pGraphic_Device))))
		return;
	if (FAILED(m_pInstance->SetLevel(LEVEL_EFFECT, CLevel_Effect::Create(m_pGraphic_Device))))
		return;

	if (FAILED(m_pInstance->ChangeLevel(LEVEL_MAP)))
		return;
}




int CToolView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

	SetTimer(0, 1000 / 60, NULL);	// 1�ʿ� 60��
	
	return 0;
}


void CToolView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CMainFrame* m_pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	CMyForm* m_pFormView = dynamic_cast<CMyForm*>(m_pMain->m_MainSplitter.GetPane(0, 0));

	LEVEL m_eSelecLevel = (LEVEL)(m_pFormView->GetDlg().GetTabCtrl().GetCurSel() + 1);
	if (m_eCurrLevel != m_eSelecLevel) {
		m_eCurrLevel = m_eSelecLevel;
		m_pInstance->ChangeLevel(m_eCurrLevel);
	}

	CGameInstance::GetInstance()->Tick_Engine(m_eCurrLevel, 60.f / 1000);

	CGameInstance::GetInstance()->Render_Begin();

	// ������ ������Ʈ�� �ִ� ��� ������Ʈ�� ���� ������ �°� ����
	if (FAILED(m_pRenderer->Render_GameObjects()))
		return;

	// ���� ������ �ִ� ������Ʈ�� ����, �׷��� �̰� ������ �Ⱦ�
	if (FAILED(m_pInstance->Render_Engine()))
		return;

	CGameInstance::GetInstance()->Render_End();

	CFormView::OnTimer(nIDEvent);
}
