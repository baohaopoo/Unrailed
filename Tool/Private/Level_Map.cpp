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

	pGameInstance->Tick();// z를 누르면 생성/삭제 모드

	if (pGameInstance->Get_DIKeyState(DIK_Z)) {
		CMainFrame* m_pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
		CMyForm* m_pFormView = dynamic_cast<CMyForm*>(m_pMain->m_MainSplitter.GetPane(0, 0));

		m_pFormView->GetDlg().GetTabMap()->RadioCtrl(IDC_RADIO8);
	}

	// x를 누르면 선택 모드
	if (pGameInstance->Get_DIKeyState(DIK_X)) {
		CMainFrame* m_pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
		CMyForm* m_pFormView = dynamic_cast<CMyForm*>(m_pMain->m_MainSplitter.GetPane(0, 0));

		m_pFormView->GetDlg().GetTabMap()->RadioCtrl(IDC_RADIO9);
	}

	// c를 누르면 카메라 모드
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

	// 현재 모드가 생성 모드일 경우에만 오브젝트를 생성, 삭제 한다.
	if (MODE_CREATE == m_eCurrMode) {
		// 마우스 왼쪽 버튼 클릭
		if (!m_bLButtonClick && (pGameInstance->Get_DIMButtonState(CInput_Device::DIMB_LBUTTON) & 0x80)) {
			// 마우스 왼쪽 버튼을 눌렀다고 표시하고, 눌렀다가 때기 전까지 작동하지 않게한다.
			m_bLButtonClick = true;
			CreateMapObject();
		}
		if (!(pGameInstance->Get_DIMButtonState(CInput_Device::DIMB_LBUTTON) & 0x80)) {
			// 마우스 왼쪽 버튼을 눌렀다가 땠을 경우, 다시 마우스 클릭 버튼이 작동하도록한다.
			m_bLButtonClick = false;
		}

		// 마우스 오른쪽 버튼 클릭
		if (!m_bRButtonClick && (pGameInstance->Get_DIMButtonState(CInput_Device::DIMB_RBUTTON) & 0x80)) {
			m_bRButtonClick = true;
			DeleteMapObject();
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
			CMainFrame* m_pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
			CMyForm* m_pFormView = dynamic_cast<CMyForm*>(m_pMain->m_MainSplitter.GetPane(0, 0));

			// 색을 바꾸는것은 터레인 레이어에 있는 오브젝트만 가능하도록한다.
			// 맵 레이어에 있는것들은 이미 자체로 완성된 오브젝트 들이다.
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
			// 마우스 왼쪽 버튼을 눌렀다가 땠을 경우, 다시 마우스 클릭 버튼이 작동하도록한다.
		}
	}
}

HRESULT CLevel_Map::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));

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
	_float3 vPos = pGameInstance->TriBevelMiddle(*pPolygon);

	// 구한 빗변의 점의 중점에 Cube의 한 변의 길이인 1을 반으로 나눈 0.5의 길이를 가진 방향 벡터를 더한다.
	vPos += (vCross * 0.5);

	//// 이제 vPos를 중점으로 하는 한변의 길이가 1인 Object를 생성하여 화면에 띄우면 된다.
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

	// 찾은 오브젝트가 존재한다면 터레인이나 맵 레이어에 있을테니 둘줄 하나에서 삭제한다.
	pGameInstance->Delete_GameObject(LEVEL_MAP, Layer_Terrain, pObject);
	pGameInstance->Delete_GameObject(LEVEL_MAP, Layer_Map, pObject);

	return S_OK;
}
