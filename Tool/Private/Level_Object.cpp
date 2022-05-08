#include "stdafx.h"
#include "Level_Object.h"
#include "GameInstance.h"
#include "Camera_Default.h"

#include "MainFrm.h"
#include "MyForm.h"
#include "Tab_Object.h"
#include "resource.h"

CLevel_Object::CLevel_Object(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
	m_eCurrMode = MODE_CREATE;
}

HRESULT CLevel_Object::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;


	if (FAILED(Ready_Layer_Camera(Layer_Camera_Object)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Cube(Layer_Object)))
		return E_FAIL;

	return S_OK;
}

void CLevel_Object::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	pGameInstance->Tick();

	SetRenderState();

	// z를 누르면 생성/삭제 모드
	if (pGameInstance->Get_DIKeyState(DIK_Z)) {
		CMainFrame* m_pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
		CMyForm* m_pFormView = dynamic_cast<CMyForm*>(m_pMain->m_MainSplitter.GetPane(0, 0));

		m_pFormView->GetDlg().GetTabObject()->RadioCtrl(IDC_RADIO2);
	}

	// x를 누르면 선택 모드
	if (pGameInstance->Get_DIKeyState(DIK_X)) {
		CMainFrame* m_pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
		CMyForm* m_pFormView = dynamic_cast<CMyForm*>(m_pMain->m_MainSplitter.GetPane(0, 0));

		m_pFormView->GetDlg().GetTabObject()->RadioCtrl(IDC_RADIO3);
	}

	// c를 누르면 카메라 모드
	if (pGameInstance->Get_DIKeyState(DIK_C)) {
		CMainFrame* m_pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
		CMyForm* m_pFormView = dynamic_cast<CMyForm*>(m_pMain->m_MainSplitter.GetPane(0, 0));

		m_pFormView->GetDlg().GetTabObject()->RadioCtrl(IDC_RADIO4);
	}


	POINT ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (ptMouse.x < 0 || ptMouse.y < 0)
		return;

	// 현재 모드가 생성 모드일 경우에만 오브젝트를 생성, 삭제 한다.
	if (MODE_CREATE == m_eCurrMode) {
		// 마우스 왼쪽 버튼 클릭
		if (!m_bLButtonClick && (pGameInstance->Get_DIMButtonState(CInput_Device::DIMB_LBUTTON) & 0x80)) {
			// 마우스 왼쪽 버튼을 눌렀다고 표시하고, 눌렀다가 때기 전까지 작동하지 않게한다.
			m_bLButtonClick = true;
			CreateVIBuffer(VIB_CUBE);
		}
		if(!(pGameInstance->Get_DIMButtonState(CInput_Device::DIMB_LBUTTON) & 0x80)) {
			// 마우스 왼쪽 버튼을 눌렀다가 땠을 경우, 다시 마우스 클릭 버튼이 작동하도록한다.
			m_bLButtonClick = false;
		}

		// 마우스 오른쪽 버튼 클릭
		if (!m_bRButtonClick && (pGameInstance->Get_DIMButtonState(CInput_Device::DIMB_RBUTTON) & 0x80)) {
			m_bRButtonClick = true;
			DeleteVIBuffer();
		}
		if (!(pGameInstance->Get_DIMButtonState(CInput_Device::DIMB_RBUTTON) & 0x80)) {
			// 마우스 오른쪽 버튼을 눌렀다가 땠을 경우, 다시 마우스 클릭 버튼이 작동하도록한다.
			m_bRButtonClick = false;
		}
	}

	// 카메라 움직임 모드일 경우에 어떠한 행동도 하지 않는다. 
	if (MODE_CAMERA == m_eCurrMode) {
		;
	}

	// 오브젝트 선택 모드일 경우 오브젝트를 골라 해당하는 오브젝트의 정보를 넘겨준다.
	if (MODE_SELECT == m_eCurrMode) {

		if (!m_bLButtonClick && (pGameInstance->Get_DIMButtonState(CInput_Device::DIMB_LBUTTON) & 0x80)) {
			// 마우스 왼쪽 버튼을 눌렀다고 표시하고, 눌렀다가 때기 전까지 작동하지 않게한다.
			m_bLButtonClick = true;

			CMainFrame* m_pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
			CMyForm* m_pFormView = dynamic_cast<CMyForm*>(m_pMain->m_MainSplitter.GetPane(0, 0));

			m_pFormView->GetDlg().GetTabObject()->Set_SelectObject(SelectVIBuffer());

		}
		if (!(pGameInstance->Get_DIMButtonState(CInput_Device::DIMB_LBUTTON) & 0x80)) {
			// 마우스 왼쪽 버튼을 눌렀다가 땠을 경우, 다시 마우스 클릭 버튼이 작동하도록한다.
			m_bLButtonClick = false;
		}
	}

}

HRESULT CLevel_Object::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	DrawAxis();

	SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Object::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	

	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float3(10.f, 10.f, -10.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);
	CameraDesc.fNear = 0.001f;
	CameraDesc.fFar = 200.0f;
	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fAspect = (_float)Resolution.x / Resolution.y;

	CameraDesc.TransformDesc.fSpeedPerSec = 5.f;
	CameraDesc.TransformDesc.fRotationPerSec = 0.3f;
	CameraDesc.TransformDesc.fScalePerSec = 1.f;


	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_OBJECT, pLayerTag, Prototype_Object_Camera_Default, &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Object::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_OBJECT, pLayerTag, Prototype_Object_Player)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Object::Ready_Layer_Terrain(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_OBJECT, pLayerTag, Prototype_Object_Terrain)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Object::Ready_Layer_Cube(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_OBJECT, pLayerTag, Prototype_Object_CubeObject)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

void CLevel_Object::SetRenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void CLevel_Object::DrawAxis()
{
	_float4x4			WorldMatrix, ViewMatrix, ProjectionMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjectionMatrix);
	WorldMatrix = ViewMatrix * ProjectionMatrix;

	LPD3DXLINE	m_pAxis;

	D3DXCreateLine(m_pGraphic_Device, &m_pAxis);
	m_pAxis->SetWidth(5);
	m_pAxis->SetGLLines(FALSE);

	m_pAxis->Begin();

	D3DXVECTOR3 m_vAxisX[2];	// X축의 정점 정보,
	D3DXVECTOR3 m_vAxisY[2];	// Y축의 정점 정보,
	D3DXVECTOR3 m_vAxisZ[2];	// Z축의 정점 정보,

	int iAxisSize = 10;

	// x축
	m_vAxisX[0] = D3DXVECTOR3(-iAxisSize, 0, 0);
	m_vAxisX[1] = D3DXVECTOR3(iAxisSize, 0, 0);

	// y축
	m_vAxisY[0] = D3DXVECTOR3(0, -iAxisSize, 0);
	m_vAxisY[1] = D3DXVECTOR3(0, iAxisSize, 0);

	// z축
	m_vAxisZ[0] = D3DXVECTOR3(0, 0, -iAxisSize);
	m_vAxisZ[1] = D3DXVECTOR3(0, 0, iAxisSize);

	m_pAxis->DrawTransform(m_vAxisX, 2, &WorldMatrix, D3DCOLOR_XRGB(255, 0, 0));
	m_pAxis->DrawTransform(m_vAxisY, 2, &WorldMatrix, D3DCOLOR_XRGB(0, 255, 0));
	m_pAxis->DrawTransform(m_vAxisZ, 2, &WorldMatrix, D3DCOLOR_XRGB(0, 0, 255));

	m_pAxis->End();
	m_pAxis->Release();
}

HRESULT CLevel_Object::CreateVIBuffer(VIBuffer _eType)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CPick_Manager::POLYGON* pPolygon = pGameInstance->Picking(LEVEL_OBJECT);

	if (nullptr == pPolygon)
		return E_FAIL;

	// 이제 올릴 폴리곤을 알았으니, 해당하는 폴리곤의 법선벡터를 구한다.
	// 임의의 벡터를 구한다.
	_float3 vExLook = pPolygon->p2 - pPolygon->p1;
	_float3 vExUp = pPolygon->p3 - pPolygon->p1;


	// 나온 노말라이즈한 방향 벡터에 원하는 거리를 곱한다.
	// 여기서 노말라이즈 한 벡터의 사이즈는 1이라고 한다.
	_float3 vCross{};
	D3DXVec3Cross(&vCross, &vExLook, &vExUp);
	D3DXVec3Normalize(&vCross, &vCross);

	// 빗변의 점의 중점을 구한다.
	_float3 vPos = TriBevelMiddle(*pPolygon);

	// 구한 빗변의 점의 중점에 Cube의 한 변의 길이인 1을 반으로 나눈 0.5의 길이를 가진 방향 벡터를 더한다.
	vPos += (vCross * 0.5);

	// 이제 vPos를 중점으로 하는 한변의 길이가 1인 Cube를 생성하여 화면에 띄우면 된다.
	CTransform::TRANSFORMDESC TransDesc;

	CMainFrame* m_pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	CMyForm* m_pFormView = dynamic_cast<CMyForm*>(m_pMain->m_MainSplitter.GetPane(0, 0));
	
	TransDesc.m_iTextureNumber = m_pFormView->GetDlg().GetTabObject()->Get_TextureNumber();

	// 현재는 Rect와 Cube인데 사실상 Cude만 있다. 나중에 수정
	switch (_eType)
	{
	case Engine::VIB_RECT:
		break;
	case Engine::VIB_CUBE:
		memcpy(&TransDesc.m_LocalMatrix.m[3], &vPos, sizeof(_float3));
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_OBJECT, Layer_Object, Prototype_Object_CubeObject, &TransDesc)))
			return E_FAIL;
		break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CLevel_Object::DeleteVIBuffer()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CGameObject* pObject = pGameInstance->Picking_Layer(LEVEL_OBJECT, Layer_Object);

	if (nullptr == pObject)
		return E_FAIL;

	return pGameInstance->Delete_GameObject(LEVEL_OBJECT, Layer_Object, pObject);
}

CGameObject * CLevel_Object::SelectVIBuffer(void)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CGameObject* pObject = pGameInstance->Picking_Layer(LEVEL_OBJECT, Layer_Object);

	if (nullptr == pObject)
		return nullptr;

	return pObject;
}

void CLevel_Object::UpgradePrecision(CPick_Manager::POLYGON & polygon)
{
	_float a = polygon.p1.x * 2;
	_float b = polygon.p1.y * 2;
	_float c = polygon.p1.z * 2;

	//if(a)

	//if (fmodf(polygon.p1.x, 0.5) >= 0)

}

CLevel_Object * CLevel_Object::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Object*	pInstance = new CLevel_Object(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(TEXT("Failed to Created CLevel_Object"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Object::Free()
{
	__super::Free();


}

_float3 CLevel_Object::TriBevelMiddle(_float3 & p1, const _float3 & p2, const _float3 & p3)
{
	// 1. 인자로 받은 점 3개로 만든 삼각형에서 가장 길이가 긴 변을 구한다.
	float fSide0 = 0.f;
	float fSide1 = pow((p1.x - p2.x), 2.f) + pow((p1.y - p2.y), 2.f) + pow((p1.z - p2.z), 2.f);	// 점 1,2
	float fSide2 = pow((p2.x - p3.x), 2.f) + pow((p2.y - p3.y), 2.f) + pow((p2.z - p3.z), 2.f);	// 점 2,3
	float fSide3 = pow((p3.x - p1.x), 2.f) + pow((p3.y - p1.y), 2.f) + pow((p3.z - p1.z), 2.f);	// 점 3,1
																						// 점 1,2로 만든 변이 가장 길면 
	(fSide1 > fSide2) ? ((fSide1 > fSide3) ? fSide0 = fSide1 : fSide0 = fSide3) : ((fSide2 > fSide3) ? fSide0 = fSide2 : fSide0 = fSide3);
	_float3 fPoint{};

	if (fSide0 == fSide1)
		fPoint = (p1 + p2) / 2.f;
	else if (fSide0 == fSide2)
		fPoint = (p2 + p3) / 2.f;
	else if (fSide0 == fSide3)
		fPoint = (p3 + p1) / 2.f;

	return fPoint;
}

_float3 CLevel_Object::TriBevelMiddle(CPick_Manager::POLYGON & polygon)
{
	return TriBevelMiddle(polygon.p1, polygon.p2, polygon.p3);
}
//
//HRESULT CLevel_Object::Save(const TCHAR * _strFilePath)
//{
//	// 현 맵에 있는 오브젝트의 정보를 저장하는 함수
//	// 각 오브젝트는 오브젝트의 타입과 행렬로 이루어져 있다.
//	// 행렬은 총 3개로, Transform 컴포넌트에 있는 로컬에서의 회전, 크기, 이동 행렬 3개의 행렬을 저장한다.
//	// 이를 모든 오브젝트를 저장할 때 까지 반복한다. 
//
//	// 1. 오브젝트의 총 갯수 저장
//	// 2. 오브젝트의 타입 저장
//	// 3. 오브젝트의 행렬저장
//	// 4. 반복
//
//	CGameInstance* pGameInstance = CGameInstance::GetInstance();
//
//	// 저장할 정보가 담긴 레이어를 찾는다.
//	CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_OBJECT, Layer_Object);
//
//	// 레이어가 없다면 return E_FAIL;
//	if (nullptr == pLayer)
//		return E_FAIL;
//
//	// 레이어에 있는 오브젝트 정보들을 가지고 온다.
//	list<CGameObject*>& ObjectList = pLayer->Get_ObjectList();
//	
//	// 가지고 온 정보가 비어있다면, 저장할 수 없으므로 return E_FAIL;
//	if (ObjectList.empty())
//		return E_FAIL;
//
//	// 파일의 경로를 읽어온다.
//	int len = wcslen(_strFilePath);
//	char* psz = new char[2 * len + 1];
//	wcstombs(psz, _strFilePath, 2 * len + 1);
//	std::string s = psz;
//
//	delete[] psz;
//
//	// 파일을 연다
//	ofstream out(s, ios::app | ios::binary);
//
//	// 파일을 열어서 불러온다.
//	if (out) {
//
//		// 전체 오브젝트의 갯수를 저장한다.
//		out << ObjectList.size() << endl;
//
//		// 오브젝트 갯수만큼 반복하여 내부 내용을 저장한다.
//		for (auto& Object : ObjectList) {
//			// 오브젝트의 VIBuffer 타입과 행렬 정보를 들고온다.
//			CVIBuffer* pVIBuffer = dynamic_cast<CVIBuffer*>(Object->Get_Component(Component_VIBuffer));
//			CTransform* pTransform = dynamic_cast<CTransform*>(Object->Get_Component(Component_Transform));
//
//			// 만약 둘중 하나라도 타입정보나 행렬정보가 없다면 넘긴다.
//			if (nullptr == pVIBuffer || nullptr == pTransform)
//				continue;
//
//			// 타입의 정보를 저장한다.
//			out << (int)pVIBuffer->Get_Type();
//
//			// 해당 객체의 행렬정보를 저장한다.
//			const CTransform::TRANSFORMDESC& TransDesc = pTransform->Get_TransformState();
//			
//			// 로컬에서의 전체 행렬 정보만 저장한다.
//			_float4x4 LocalMatrix = TransDesc.m_LocalRotate * TransDesc.m_LocalScale * TransDesc.m_LocalTransform;
//			//out << sizeof()
//			for (int i = 0; i < 4; ++i) {
//				for (int j = 0; j < 4; ++j) {
//					out << LocalMatrix.m[i][j];
//				}
//			}
//		}
//	}
//}
//
//HRESULT CLevel_Object::Load(const TCHAR * _strFilePath)
//{
//
//	return S_OK;
//}
