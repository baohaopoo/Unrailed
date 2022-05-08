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

	// z�� ������ ����/���� ���
	if (pGameInstance->Get_DIKeyState(DIK_Z)) {
		CMainFrame* m_pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
		CMyForm* m_pFormView = dynamic_cast<CMyForm*>(m_pMain->m_MainSplitter.GetPane(0, 0));

		m_pFormView->GetDlg().GetTabObject()->RadioCtrl(IDC_RADIO2);
	}

	// x�� ������ ���� ���
	if (pGameInstance->Get_DIKeyState(DIK_X)) {
		CMainFrame* m_pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
		CMyForm* m_pFormView = dynamic_cast<CMyForm*>(m_pMain->m_MainSplitter.GetPane(0, 0));

		m_pFormView->GetDlg().GetTabObject()->RadioCtrl(IDC_RADIO3);
	}

	// c�� ������ ī�޶� ���
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

	// ���� ��尡 ���� ����� ��쿡�� ������Ʈ�� ����, ���� �Ѵ�.
	if (MODE_CREATE == m_eCurrMode) {
		// ���콺 ���� ��ư Ŭ��
		if (!m_bLButtonClick && (pGameInstance->Get_DIMButtonState(CInput_Device::DIMB_LBUTTON) & 0x80)) {
			// ���콺 ���� ��ư�� �����ٰ� ǥ���ϰ�, �����ٰ� ���� ������ �۵����� �ʰ��Ѵ�.
			m_bLButtonClick = true;
			CreateVIBuffer(VIB_CUBE);
		}
		if(!(pGameInstance->Get_DIMButtonState(CInput_Device::DIMB_LBUTTON) & 0x80)) {
			// ���콺 ���� ��ư�� �����ٰ� ���� ���, �ٽ� ���콺 Ŭ�� ��ư�� �۵��ϵ����Ѵ�.
			m_bLButtonClick = false;
		}

		// ���콺 ������ ��ư Ŭ��
		if (!m_bRButtonClick && (pGameInstance->Get_DIMButtonState(CInput_Device::DIMB_RBUTTON) & 0x80)) {
			m_bRButtonClick = true;
			DeleteVIBuffer();
		}
		if (!(pGameInstance->Get_DIMButtonState(CInput_Device::DIMB_RBUTTON) & 0x80)) {
			// ���콺 ������ ��ư�� �����ٰ� ���� ���, �ٽ� ���콺 Ŭ�� ��ư�� �۵��ϵ����Ѵ�.
			m_bRButtonClick = false;
		}
	}

	// ī�޶� ������ ����� ��쿡 ��� �ൿ�� ���� �ʴ´�. 
	if (MODE_CAMERA == m_eCurrMode) {
		;
	}

	// ������Ʈ ���� ����� ��� ������Ʈ�� ��� �ش��ϴ� ������Ʈ�� ������ �Ѱ��ش�.
	if (MODE_SELECT == m_eCurrMode) {

		if (!m_bLButtonClick && (pGameInstance->Get_DIMButtonState(CInput_Device::DIMB_LBUTTON) & 0x80)) {
			// ���콺 ���� ��ư�� �����ٰ� ǥ���ϰ�, �����ٰ� ���� ������ �۵����� �ʰ��Ѵ�.
			m_bLButtonClick = true;

			CMainFrame* m_pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
			CMyForm* m_pFormView = dynamic_cast<CMyForm*>(m_pMain->m_MainSplitter.GetPane(0, 0));

			m_pFormView->GetDlg().GetTabObject()->Set_SelectObject(SelectVIBuffer());

		}
		if (!(pGameInstance->Get_DIMButtonState(CInput_Device::DIMB_LBUTTON) & 0x80)) {
			// ���콺 ���� ��ư�� �����ٰ� ���� ���, �ٽ� ���콺 Ŭ�� ��ư�� �۵��ϵ����Ѵ�.
			m_bLButtonClick = false;
		}
	}

}

HRESULT CLevel_Object::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	DrawAxis();

	SetWindowText(g_hWnd, TEXT("�����÷��̷����Դϴ�."));

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

	D3DXVECTOR3 m_vAxisX[2];	// X���� ���� ����,
	D3DXVECTOR3 m_vAxisY[2];	// Y���� ���� ����,
	D3DXVECTOR3 m_vAxisZ[2];	// Z���� ���� ����,

	int iAxisSize = 10;

	// x��
	m_vAxisX[0] = D3DXVECTOR3(-iAxisSize, 0, 0);
	m_vAxisX[1] = D3DXVECTOR3(iAxisSize, 0, 0);

	// y��
	m_vAxisY[0] = D3DXVECTOR3(0, -iAxisSize, 0);
	m_vAxisY[1] = D3DXVECTOR3(0, iAxisSize, 0);

	// z��
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

	// ���� �ø� �������� �˾�����, �ش��ϴ� �������� �������͸� ���Ѵ�.
	// ������ ���͸� ���Ѵ�.
	_float3 vExLook = pPolygon->p2 - pPolygon->p1;
	_float3 vExUp = pPolygon->p3 - pPolygon->p1;


	// ���� �븻�������� ���� ���Ϳ� ���ϴ� �Ÿ��� ���Ѵ�.
	// ���⼭ �븻������ �� ������ ������� 1�̶�� �Ѵ�.
	_float3 vCross{};
	D3DXVec3Cross(&vCross, &vExLook, &vExUp);
	D3DXVec3Normalize(&vCross, &vCross);

	// ������ ���� ������ ���Ѵ�.
	_float3 vPos = TriBevelMiddle(*pPolygon);

	// ���� ������ ���� ������ Cube�� �� ���� ������ 1�� ������ ���� 0.5�� ���̸� ���� ���� ���͸� ���Ѵ�.
	vPos += (vCross * 0.5);

	// ���� vPos�� �������� �ϴ� �Ѻ��� ���̰� 1�� Cube�� �����Ͽ� ȭ�鿡 ���� �ȴ�.
	CTransform::TRANSFORMDESC TransDesc;

	CMainFrame* m_pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	CMyForm* m_pFormView = dynamic_cast<CMyForm*>(m_pMain->m_MainSplitter.GetPane(0, 0));
	
	TransDesc.m_iTextureNumber = m_pFormView->GetDlg().GetTabObject()->Get_TextureNumber();

	// ����� Rect�� Cube�ε� ��ǻ� Cude�� �ִ�. ���߿� ����
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
	// 1. ���ڷ� ���� �� 3���� ���� �ﰢ������ ���� ���̰� �� ���� ���Ѵ�.
	float fSide0 = 0.f;
	float fSide1 = pow((p1.x - p2.x), 2.f) + pow((p1.y - p2.y), 2.f) + pow((p1.z - p2.z), 2.f);	// �� 1,2
	float fSide2 = pow((p2.x - p3.x), 2.f) + pow((p2.y - p3.y), 2.f) + pow((p2.z - p3.z), 2.f);	// �� 2,3
	float fSide3 = pow((p3.x - p1.x), 2.f) + pow((p3.y - p1.y), 2.f) + pow((p3.z - p1.z), 2.f);	// �� 3,1
																						// �� 1,2�� ���� ���� ���� ��� 
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
//	// �� �ʿ� �ִ� ������Ʈ�� ������ �����ϴ� �Լ�
//	// �� ������Ʈ�� ������Ʈ�� Ÿ�԰� ��ķ� �̷���� �ִ�.
//	// ����� �� 3����, Transform ������Ʈ�� �ִ� ���ÿ����� ȸ��, ũ��, �̵� ��� 3���� ����� �����Ѵ�.
//	// �̸� ��� ������Ʈ�� ������ �� ���� �ݺ��Ѵ�. 
//
//	// 1. ������Ʈ�� �� ���� ����
//	// 2. ������Ʈ�� Ÿ�� ����
//	// 3. ������Ʈ�� �������
//	// 4. �ݺ�
//
//	CGameInstance* pGameInstance = CGameInstance::GetInstance();
//
//	// ������ ������ ��� ���̾ ã�´�.
//	CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_OBJECT, Layer_Object);
//
//	// ���̾ ���ٸ� return E_FAIL;
//	if (nullptr == pLayer)
//		return E_FAIL;
//
//	// ���̾ �ִ� ������Ʈ �������� ������ �´�.
//	list<CGameObject*>& ObjectList = pLayer->Get_ObjectList();
//	
//	// ������ �� ������ ����ִٸ�, ������ �� �����Ƿ� return E_FAIL;
//	if (ObjectList.empty())
//		return E_FAIL;
//
//	// ������ ��θ� �о�´�.
//	int len = wcslen(_strFilePath);
//	char* psz = new char[2 * len + 1];
//	wcstombs(psz, _strFilePath, 2 * len + 1);
//	std::string s = psz;
//
//	delete[] psz;
//
//	// ������ ����
//	ofstream out(s, ios::app | ios::binary);
//
//	// ������ ��� �ҷ��´�.
//	if (out) {
//
//		// ��ü ������Ʈ�� ������ �����Ѵ�.
//		out << ObjectList.size() << endl;
//
//		// ������Ʈ ������ŭ �ݺ��Ͽ� ���� ������ �����Ѵ�.
//		for (auto& Object : ObjectList) {
//			// ������Ʈ�� VIBuffer Ÿ�԰� ��� ������ ���´�.
//			CVIBuffer* pVIBuffer = dynamic_cast<CVIBuffer*>(Object->Get_Component(Component_VIBuffer));
//			CTransform* pTransform = dynamic_cast<CTransform*>(Object->Get_Component(Component_Transform));
//
//			// ���� ���� �ϳ��� Ÿ�������� ��������� ���ٸ� �ѱ��.
//			if (nullptr == pVIBuffer || nullptr == pTransform)
//				continue;
//
//			// Ÿ���� ������ �����Ѵ�.
//			out << (int)pVIBuffer->Get_Type();
//
//			// �ش� ��ü�� ��������� �����Ѵ�.
//			const CTransform::TRANSFORMDESC& TransDesc = pTransform->Get_TransformState();
//			
//			// ���ÿ����� ��ü ��� ������ �����Ѵ�.
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
