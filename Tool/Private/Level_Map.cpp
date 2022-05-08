#include "stdafx.h"
#include "Level_Map.h"
#include "GameInstance.h"
#include "Camera_Default.h"

#include "MainFrm.h"
#include "MyForm.h"
#include "Tab_Map.h"

#include "Terrain.h"
#include "Rock.h"
#include "Tree.h"

#include "Tab_Map.h"
#include "resource.h"

CLevel_Map::CLevel_Map(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
	m_eCurrMode = MODE_CREATE;
}

HRESULT CLevel_Map::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(Layer_Camera_Map)))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Terrain(Layer_Terrain)))
	//	return E_FAIL;

	return S_OK;
}

void CLevel_Map::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	pGameInstance->Tick();// z�� ������ ����/���� ���

	if (pGameInstance->Get_DIKeyState(DIK_Z)) {
		CMainFrame* m_pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
		CMyForm* m_pFormView = dynamic_cast<CMyForm*>(m_pMain->m_MainSplitter.GetPane(0, 0));

		m_pFormView->GetDlg().GetTabMap()->RadioCtrl(IDC_RADIO8);
	}

	// x�� ������ ���� ���
	if (pGameInstance->Get_DIKeyState(DIK_X)) {
		CMainFrame* m_pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
		CMyForm* m_pFormView = dynamic_cast<CMyForm*>(m_pMain->m_MainSplitter.GetPane(0, 0));

		m_pFormView->GetDlg().GetTabMap()->RadioCtrl(IDC_RADIO9);
	}

	// c�� ������ ī�޶� ���
	if (pGameInstance->Get_DIKeyState(DIK_C)) {
		CMainFrame* m_pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
		CMyForm* m_pFormView = dynamic_cast<CMyForm*>(m_pMain->m_MainSplitter.GetPane(0, 0));

		m_pFormView->GetDlg().GetTabMap()->RadioCtrl(IDC_RADIO10);
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
			CreateMapObject();
		}
		if (!(pGameInstance->Get_DIMButtonState(CInput_Device::DIMB_LBUTTON) & 0x80)) {
			// ���콺 ���� ��ư�� �����ٰ� ���� ���, �ٽ� ���콺 Ŭ�� ��ư�� �۵��ϵ����Ѵ�.
			m_bLButtonClick = false;
		}

		// ���콺 ������ ��ư Ŭ��
		if (!m_bRButtonClick && (pGameInstance->Get_DIMButtonState(CInput_Device::DIMB_RBUTTON) & 0x80)) {
			m_bRButtonClick = true;
			DeleteMapObject();
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
			CMainFrame* m_pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
			CMyForm* m_pFormView = dynamic_cast<CMyForm*>(m_pMain->m_MainSplitter.GetPane(0, 0));

			// ���� �ٲٴ°��� �ͷ��� ���̾ �ִ� ������Ʈ�� �����ϵ����Ѵ�.
			// �� ���̾ �ִ°͵��� �̹� ��ü�� �ϼ��� ������Ʈ ���̴�.
			CGameObject* pObject = pGameInstance->Picking_Layer(LEVEL_MAP, Layer_Terrain);
			if (nullptr != pObject) {
				pObject->Set_TextureNumber(m_pFormView->GetDlg().GetTabMap()->Get_TextureNumber());
			}

			pObject = pGameInstance->Picking_Object(LEVEL_MAP);
			if (nullptr != pObject) {
				m_pFormView->GetDlg().GetTabMap()->Set_SelectObject(pObject);
			}
		}
		if (!(pGameInstance->Get_DIMButtonState(CInput_Device::DIMB_LBUTTON) & 0x80)) {
			// ���콺 ���� ��ư�� �����ٰ� ���� ���, �ٽ� ���콺 Ŭ�� ��ư�� �۵��ϵ����Ѵ�.
		}
	}
}

HRESULT CLevel_Map::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("�����÷��̷����Դϴ�."));

	return S_OK;
}

HRESULT CLevel_Map::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	
	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float3(0.f, 10.f, -15.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 300.0f;
	CameraDesc.fFovy = D3DXToRadian(60.0f);

	CameraDesc.fAspect = g_ToolViewAspect;

	CameraDesc.TransformDesc.fSpeedPerSec = 3.f;
	CameraDesc.TransformDesc.fRotationPerSec = 0.3f;
	CameraDesc.TransformDesc.fScalePerSec = 3.f;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, pLayerTag, Prototype_Object_Camera_Default, &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Map::Ready_Layer_Terrain(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CTerrain::MAPDESC MapDesc;
	MapDesc.iMapSizeX = 10;
	MapDesc.iMapSizeY = 10;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, pLayerTag, Prototype_Object_Terrain, &MapDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_Map * CLevel_Map::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Map*	pInstance = new CLevel_Map(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(TEXT("Failed to Created CLevel_Map"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Map::Free()
{
	__super::Free();


}

HRESULT CLevel_Map::CreateMapObject(void)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CPick_Manager::POLYGON* pPolygon = pGameInstance->Picking(LEVEL_MAP);

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
	_float3 vPos = pGameInstance->TriBevelMiddle(*pPolygon);

	// ���� ������ ���� ������ Cube�� �� ���� ������ 1�� ������ ���� 0.5�� ���̸� ���� ���� ���͸� ���Ѵ�.
	vPos += (vCross * 0.5);

	//// ���� vPos�� �������� �ϴ� �Ѻ��� ���̰� 1�� Object�� �����Ͽ� ȭ�鿡 ���� �ȴ�.
	CMainFrame* m_pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	CMyForm* m_pFormView = dynamic_cast<CMyForm*>(m_pMain->m_MainSplitter.GetPane(0, 0));

	CTransform::TRANSFORMDESC TransDesc = m_pFormView->GetDlg().GetTabMap()->Get_TransDesc();
	memcpy(&TransDesc.m_WorldMatrix.m[3], &vPos, sizeof(_float3));

	_float3 vScale = m_pFormView->GetDlg().GetTabMap()->m_vScale;
	if (vScale.x == 0)
		vScale.x = 1.f;
	if (vScale.y == 0)
		vScale.y = 1.f;
	if (vScale.z == 0)
		vScale.z = 1.f;

	TransDesc.m_WorldMatrix.m[0][0] *= vScale.x;
	TransDesc.m_WorldMatrix.m[0][1] *= vScale.x;
	TransDesc.m_WorldMatrix.m[0][2] *= vScale.x;
	TransDesc.m_WorldMatrix.m[1][0] *= vScale.y;
	TransDesc.m_WorldMatrix.m[1][1] *= vScale.y;
	TransDesc.m_WorldMatrix.m[1][2] *= vScale.y;
	TransDesc.m_WorldMatrix.m[2][0] *= vScale.z;
	TransDesc.m_WorldMatrix.m[2][1] *= vScale.z;
	TransDesc.m_WorldMatrix.m[2][2] *= vScale.z;

	_float fRotateY = m_pFormView->GetDlg().GetTabMap()->m_fRotateY;

	_float3		vRight, vUp, vLook;
	memcpy(&vRight, TransDesc.m_WorldMatrix.m[0], sizeof(_float3));
	memcpy(&vUp, TransDesc.m_WorldMatrix.m[1], sizeof(_float3));
	memcpy(&vLook, TransDesc.m_WorldMatrix.m[2], sizeof(_float3));

	_float4x4 RotateMatrix;
	D3DXMatrixIdentity(&RotateMatrix);
	D3DXMatrixRotationY(&RotateMatrix, D3DXToRadian(fRotateY));

	D3DXVec3TransformNormal(&vRight, &vRight, &RotateMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotateMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotateMatrix);

	memcpy(TransDesc.m_WorldMatrix.m[0], &vRight, sizeof(_float3));
	memcpy(TransDesc.m_WorldMatrix.m[1], &vUp, sizeof(_float3));
	memcpy(TransDesc.m_WorldMatrix.m[2], &vLook, sizeof(_float3));

	CRock::ROCKDESC RockDesc;
	RockDesc.TransDesc = TransDesc;

	CTree::TreeDESC TreeDesc;
	TreeDesc.TransDesc = TransDesc;

	switch (m_pFormView->GetDlg().GetTabMap()->GetSelectListBoxSel()) {

	case Tool::CTab_Map::OBJECT_ROCK1:
		RockDesc.iRockNumber = 1;
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Rock"), &RockDesc)))
			return E_FAIL;
		break;
	case Tool::CTab_Map::OBJECT_ROCK2:
		RockDesc.iRockNumber = 2;
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Rock"), &RockDesc)))
			return E_FAIL;
		break;
	case Tool::CTab_Map::OBJECT_ROCK3:
		RockDesc.iRockNumber = 3;
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Rock"), &RockDesc)))
			return E_FAIL;
		break;



	case Tool::CTab_Map::OBJECT_TREE_DARK1:
		TreeDesc.iTreeNumber = 1;
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Tree"), &TreeDesc)))
			return E_FAIL;
		break;
	case Tool::CTab_Map::OBJECT_TREE_DARK2:
		TreeDesc.iTreeNumber = 2;
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Tree"), &TreeDesc)))
			return E_FAIL;
		break;
	case Tool::CTab_Map::OBJECT_TREE_DARK3:
		TreeDesc.iTreeNumber = 3;
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Tree"), &TreeDesc)))
			return E_FAIL;
		break;
	case Tool::CTab_Map::OBJECT_TREE_DARK4:
		TreeDesc.iTreeNumber = 4;
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Tree"), &TreeDesc)))
			return E_FAIL;
		break;
	case Tool::CTab_Map::OBJECT_TREE_LIGHT:
		TreeDesc.iTreeNumber = 5;
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Tree"), &TreeDesc)))
			return E_FAIL;
		break;


	case Tool::CTab_Map::OBJECT_MATERIAL_IRON:
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Iron"), &TransDesc)))
			return E_FAIL;
		break;
	case Tool::CTab_Map::OBJECT_MATERIAL_WOOD:
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Wood"), &TransDesc)))
			return E_FAIL;
		break;


	case Tool::CTab_Map::OBJECT_PLATFORM_START:
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_TrainStation"), &TransDesc)))
			return E_FAIL;
		break;
	case Tool::CTab_Map::OBJECT_PLATFORM_FINISH:
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_TrainStation"), &TransDesc)))
			return E_FAIL;
		break;



	case Tool::CTab_Map::OBJECT_TRAIN_HEAD:
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Train_Head"), &TransDesc)))
			return E_FAIL;
		break;
	case Tool::CTab_Map::OBJECT_TRAIN_MATERIAL:
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Train_Material"), &TransDesc)))
			return E_FAIL;
		break;
	case Tool::CTab_Map::OBJECT_TRAIN_RAIL:
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Train_Rail"), &TransDesc)))
			return E_FAIL;
		break;
	case Tool::CTab_Map::OBJECT_TRAIN_WATER:
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Train_Water"), &TransDesc)))
			return E_FAIL;
		break;
	case Tool::CTab_Map::OBJECT_TRAIN_DYNAMITE:
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Train_Dynamite"), &TransDesc)))
			return E_FAIL;
		break;



	case Tool::CTab_Map::OBJECT_PICKAX:
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_PickAx"), &TransDesc)))
			return E_FAIL;
		break;
	case Tool::CTab_Map::OBJECT_AXE:
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Axe"), &TransDesc)))
			return E_FAIL;
		break;
	case Tool::CTab_Map::OBJECT_BUCKET:
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Bucket"), &TransDesc)))
			return E_FAIL;
		break;



	case Tool::CTab_Map::OBJECT_CAMEL:
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Camel"), &TransDesc)))
			return E_FAIL;
		break;



	case Tool::CTab_Map::OBJECT_RAIL_STRAIGHT:
	case Tool::CTab_Map::OBJECT_RAIL_CURVE:
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Rail"), &TransDesc)))
			return E_FAIL;
		break;
	case Tool::CTab_Map::OBJECT_RAIL_END:
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Rail_End"), &TransDesc)))
			return E_FAIL;
		break;



	case Tool::CTab_Map::OBJECT_UNBREAKABLE_ROCK1:
		RockDesc.iRockNumber = 1;
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, TEXT("Layer_Unbreakable"), TEXT("Prototype_GameObject_Unbreakable_Rock"), &RockDesc)))
			return E_FAIL;
		break;
	case Tool::CTab_Map::OBJECT_UNBREAKABLE_ROCK2:
		RockDesc.iRockNumber = 2;
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, TEXT("Layer_Unbreakable"), TEXT("Prototype_GameObject_Unbreakable_Rock"), &RockDesc)))
			return E_FAIL;
		break;
	case Tool::CTab_Map::OBJECT_UNBREAKABLE_ROCK3:
		RockDesc.iRockNumber = 3;
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, TEXT("Layer_Unbreakable"), TEXT("Prototype_GameObject_Unbreakable_Rock"), &RockDesc)))
			return E_FAIL;
		break;



	case Tool::CTab_Map::OBJECT_MAPCUBE:
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Terrain, TEXT("Prototype_GameObject_MapCube"), &TransDesc)))
			return E_FAIL;
		break;



	case Tool::CTab_Map::OBJECT_END:
		break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CLevel_Map::DeleteMapObject(void)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CGameObject* pObject = pGameInstance->Picking_Object(LEVEL_MAP);

	if (nullptr == pObject)
		return E_FAIL;

	// ã�� ������Ʈ�� �����Ѵٸ� �ͷ����̳� �� ���̾ �����״� ���� �ϳ����� �����Ѵ�.
	pGameInstance->Delete_GameObject(LEVEL_MAP, Layer_Terrain, pObject);
	pGameInstance->Delete_GameObject(LEVEL_MAP, Layer_Map, pObject);

	return S_OK;
}
