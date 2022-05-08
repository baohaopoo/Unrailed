#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "GameInstance.h"
#include "Camera_Default.h"

#include "Rock.h"
#include "Tree.h"
#include "Rail.h"

#include "SoundMgr.h"
#include "Level_Lobby.h"
#include "Level_Loading.h"
#include "GameOver.h"
CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{

}

HRESULT CLevel_GamePlay::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	if (FAILED(Ready_Layer_MapObject(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Bird(TEXT("Layer_Bird"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_RightHand(TEXT("Layer_RightHand"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_LeftHand(TEXT("Layer_LeftHand"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Giant(TEXT("Layer_Giant"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_MenuBtnUI(TEXT("Layer_Btn"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_MissionBtn(TEXT("Layer_MissionBtn"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_MSignUI(TEXT("Layer_MSign"))))
		return E_FAIL;


	if (FAILED(Ready_Layer_Hammer(TEXT("Layer_Hammer"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_PickEx(TEXT("Layer_PickEx"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Bucket(TEXT("Layer_Bucket"))))
		return E_FAIL;


	if (FAILED(Ready_Layer_Material_Wood(TEXT("Layer_Wood"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Material_Iron(TEXT("Layer_Iron"))))
		return E_FAIL;


	if (FAILED(Ready_Layer_Rail_Invisible(TEXT("Layer_Rail_Invisible"))))
		return E_FAIL;

	// �浹ó�� - �浹 ���� ó���� �� ���̾� �־��ֱ�
	CGameInstance* pGameInstance = CGameInstance::GetInstance();



	// ��������
	pGameInstance->AddCollisionLayers(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Layer_Hammer"));
	pGameInstance->AddCollisionLayers(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Layer_PickEx"));
	pGameInstance->AddCollisionLayers(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Layer_Bucket"));

	// �÷��̾ �������� ���� ���
	pGameInstance->AddCollisionLayers(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Layer_Wood"));

	// �÷��̾ �������� ö���� ���
	pGameInstance->AddCollisionLayers(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Layer_Iron"));

	// �÷��̾�� Train
	pGameInstance->AddCollisionLayers(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Layer_Train"));

	// ���� �浹 (�ڿ�(��, ����), ������, ����, �߸�������Ʈ)
	pGameInstance->AddCollisionLayers(LEVEL_GAMEPLAY, TEXT("Layer_Player"), Layer_Map);


	// �÷��̾�� ���̳ʸ���Ʈ
	pGameInstance->AddCollisionLayers(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Layer_Dynamite"));
	// ���̳ʸ���Ʈ�� ��
	pGameInstance->AddCollisionLayers(LEVEL_GAMEPLAY, TEXT("Layer_Dynamite"), Layer_Map);


	D3DXCreateFont(m_pGraphic_Device, 10, 9, 920, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"����ü", &font);

	// �ΰ��� ���� Ȱ��ȭ
	//CSoundMgr::Get_Instance()->PlaySound(TEXT("Rockabye.mp3"), CSoundMgr::CHANNELID::BGM, SOUND_DEFAULT);
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
	CSoundMgr::Get_Instance()->PlayBGM(TEXT("GamePlay_Basic4.mp3"), 0.4f);




	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{


	__super::Tick(fTimeDelta);
	CGameInstance* pGameInstance = CGameInstance::GetInstance();


	for (int i = 0; i < 7; ++i)
	{
		CTransform* pBirdBody = dynamic_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Bird"), Component_Transform, i));
		CTransform* pBirdLeftWing = dynamic_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_LeftWing"), Component_Transform, i));
		CTransform* pBirdRightWing = dynamic_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_RightWing"), Component_Transform, i));

		_float3 vPos = pBirdBody->Get_State(CTransform::STATE_POSITION);

		pBirdLeftWing->Set_State(CTransform::STATE_POSITION, vPos);
		pBirdRightWing->Set_State(CTransform::STATE_POSITION, vPos);
	}
}

HRESULT CLevel_GamePlay::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	//SetWindowText(g_hWnd, TEXT("�����÷��̷����Դϴ�."));


	char string1[100] = "�޴�";
	RECT rt1 = { 600,300, -1,-1 };
	font->DrawTextA(NULL, string1, -1, &rt1, DT_NOCLIP, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));
	_float3		vPosition = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));
	CameraDesc.vEye = _float3(vPosition.x-3.f, 5.f, -4.0f);
	CameraDesc.vAt = _float3(vPosition.x, 0.f, vPosition.z);

	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 300.0f;
	CameraDesc.fFovy = D3DXToRadian(30.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;


	CameraDesc.TransformDesc.fSpeedPerSec = 2.f;
	CameraDesc.TransformDesc.fRotationPerSec = 10.f;
	CameraDesc.TransformDesc.fScalePerSec = 1.f;


	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Camera_Default"), &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	//level_static -> ���蹮��..

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Player"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Player_Feet"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Player_LeftFeet"))))
		return E_FAIL;



	//if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Player_LeftHand"))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Player_RightHand"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_Hammer(TEXT("Layer_Hammer"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_PickEx(TEXT("Layer_PickEx"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_Rock(TEXT("Layer_Rock"))))
	//	return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_MapObject(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	// ���ϴ� ��.dat ������ �̸��� ���ڷ� �޾� �����͸� �о� ���� ���� Ŭ���̾�Ʈ�� �ε��Ѵ�.
	//Load_MapData(TEXT("Map04.dat"));

	//Load_MapToolData(TEXT("snowmap4.dat"));
	Load_MapToolData(TEXT("EasyMap.dat"));


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_MenuBtnUI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	//if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_MenuBtn")))) //�̼� UI
	//	return E_FAIL;


	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_BoltUI")))) //��Ʈ����/���ñ�
		return E_FAIL;


	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_LifeUI")))) //�ӷ�
		return E_FAIL;
	
	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_MSignUI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	CTransform*		targetTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));
	_float3	targetpos = targetTransform->Get_State(CTransform::STATE_POSITION);



	CTransform::TRANSFORMDESC tradesc;
	tradesc.m_WorldMatrix.m[3][0] = targetpos.x + 8.f;	//msign ��ġ ����
	tradesc.m_WorldMatrix.m[3][1] = targetpos.y - 5.f;
	tradesc.m_WorldMatrix.m[3][2] = targetpos.z - 14.f;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_mSign"), &tradesc))) //���� ǥ�� UI
		return E_FAIL;


	CTransform::TRANSFORMDESC tradesc2;
	tradesc2.m_WorldMatrix.m[3][0] = targetpos.x;
	tradesc2.m_WorldMatrix.m[3][1] = targetpos.y - 7.f;
	tradesc2.m_WorldMatrix.m[3][2] = targetpos.z + 70.f;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_PlayGround"), &tradesc2))) //���� ǥ�� UI
		return E_FAIL;

	return S_OK;
}

//HRESULT CLevel_GamePlay::Ready_Layer_Giant(const _tchar * pLayerTag)
//{
//	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
//
//
//	CTransform::TRANSFORMDESC tradesc;
//	tradesc.m_WorldMatrix.m[3][0] = 3.f;
//	tradesc.m_WorldMatrix.m[3][1] = -5.f;
//	tradesc.m_WorldMatrix.m[3][2] = 45.f;
//
//	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Giant"), &tradesc)))
//		return E_FAIL;
//
//	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_GiantHead"), &tradesc)))
//		return E_FAIL;
//
//	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_GiantLeftHand"), &tradesc)))
//		return E_FAIL;
//
//	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_GiantRightHand"), &tradesc)))
//		return E_FAIL;
//
//	return S_OK;
//}

HRESULT CLevel_GamePlay::Ready_Layer_LeftHand(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Player_LeftHand"))))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_RightHand(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Player_RightHand"))))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Hammer(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();


	CTransform::TRANSFORMDESC tradesc;
	//CPlayer* player;
	tradesc.m_WorldMatrix.m[3][0] = -10.f;
	tradesc.m_WorldMatrix.m[3][1] = 0.f;
	tradesc.m_WorldMatrix.m[3][2] = -1.f;
	//tradesc.parent = player;
	//�������..


	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Axe"), &tradesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_PickEx(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();


	CTransform::TRANSFORMDESC tradesc;
	tradesc.m_WorldMatrix.m[3][0] = 0.f;
	tradesc.m_WorldMatrix.m[3][1] = 0.f;
	tradesc.m_WorldMatrix.m[3][2] = 2.f;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_PickAx"), &tradesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Bird(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	for (int i = 0; i < 7; ++i)
	{
		CTransform::TRANSFORMDESC tradesc;
		tradesc.m_WorldMatrix.m[3][0] = -20.f;
		tradesc.m_WorldMatrix.m[3][1] = 1.f;
		tradesc.m_WorldMatrix.m[3][2] = 2.f;

		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Bird"), &tradesc)))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_LeftWing"), TEXT("Prototype_GameObject_BirdLeft_Wing"), &tradesc)))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_RightWing"), TEXT("Prototype_GameObject_BirdRight_Wing"), &tradesc)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Bucket(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();


	CTransform::TRANSFORMDESC tradesc;
	tradesc.m_WorldMatrix.m[3][0] = -13.f;
	tradesc.m_WorldMatrix.m[3][1] = 0.f;
	tradesc.m_WorldMatrix.m[3][2] = 4.f;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Bucket"), &tradesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Rock(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();


	CRock::ROCKDESC rockdesc;
	rockdesc.iRockNumber = 1;
	rockdesc.TransDesc.m_WorldMatrix.m[3][0] = 12.f;
	rockdesc.TransDesc.m_WorldMatrix.m[3][1] = 1.f;
	rockdesc.TransDesc.m_WorldMatrix.m[3][2] = 1.f;


	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Rock"), &rockdesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Tree(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();


	CTree::TreeDESC treedesc;
	treedesc.iTreeNumber = 1;
	treedesc.TransDesc.m_WorldMatrix.m[3][0] = 17.f;
	treedesc.TransDesc.m_WorldMatrix.m[3][1] = 1.f;
	treedesc.TransDesc.m_WorldMatrix.m[3][2] = 1.f;


	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tree"), &treedesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Material_Wood(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();



	CTransform::TRANSFORMDESC tradesc;
	tradesc.m_WorldMatrix.m[3][0] = -13.f;
	tradesc.m_WorldMatrix.m[3][1] = 0.5f;
	tradesc.m_WorldMatrix.m[3][2] = 2.f;


	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Wood"), &tradesc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Material_Iron(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();



	CTransform::TRANSFORMDESC tradesc;
	tradesc.m_WorldMatrix.m[3][0] = -15.f;
	tradesc.m_WorldMatrix.m[3][1] = 0.5f;
	tradesc.m_WorldMatrix.m[3][2] = 2.f;


	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Iron"), &tradesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Material_Rail_Straight(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();



	/*CTransform::TRANSFORMDESC tradesc;
	tradesc.m_WorldMatrix.m[3][0] = 10.f;
	tradesc.m_WorldMatrix.m[3][1] = 1.f;
	tradesc.m_WorldMatrix.m[3][2] = 2.f;


	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Iron"), &tradesc)))
		return E_FAIL;
*/

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Dynamite(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();


	CTransform::TRANSFORMDESC tradesc;
	//CPlayer* player;
	tradesc.m_WorldMatrix.m[3][0] = 12.f;
	tradesc.m_WorldMatrix.m[3][1] = 1.5f;
	tradesc.m_WorldMatrix.m[3][2] = -5.f;
	//tradesc.parent = player;
	//�������..


	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Dynamite"), &tradesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_MissionBtn(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();


	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_MenuBtn")))) //�̼� UI
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Rail_Invisible(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	CRail::RAILDESC			RailDesc;
	RailDesc.eType_Rail = CRail::RAIL_INVISIBLE;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Rail_Invisible"), &RailDesc)))
		return E_FAIL;
}

HRESULT CLevel_GamePlay::Load_MapToolData(const _tchar * _pMapFileName)
{
	if (nullptr == _pMapFileName)
		return E_FAIL;

	TCHAR	szPath[MAX_PATH] = L"";

	// ���� ������Ʈ�� �ִ� ���丮 ��θ� ������ �Լ�
	GetCurrentDirectory(MAX_PATH, szPath);

	// ��ü ��ο��� �� ������ ��θ� �߶󳻴� �Լ�
	// ��� �� �� ������ ��ΰ� ���ϸ��̸� �� ���ϸ��� ������ ��θ� ����, ���� ���ϸ��� ������ ������ �������� �߶󳽴�.
	PathRemoveFileSpec(szPath);
	PathRemoveFileSpec(szPath);

	// data���� ��θ� �̾����
	lstrcat(szPath, L"\\Data\\Map\\");

	// ���ڷ� ���� ������ �̸��� ����
	lstrcat(szPath, _pMapFileName);

	HANDLE		hFile = CreateFile(szPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD		dwByte = 0;
	DWORD		dwStrByte = 0;

	while (true) {

		// Layer_Terrain
		// ============================================================================================================
		// ������Ʈ ������ �д´�.
		SIZE_T iSize = 0;
		ReadFile(hFile, &iSize, sizeof(SIZE_T), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CGameInstance* pGameInstance = CGameInstance::GetInstance();

		// �о�� ������Ʈ ������ŭ �ݺ��� ����
		for (int i = 0; i < iSize; ++i) {

			// ������Ʈ�� Ÿ���� �����´�.
			SIZE_T iType = 0;
			ReadFile(hFile, &iType, sizeof(SIZE_T), &dwByte, nullptr);

			// �ش� ��ü�� ��������� ��� �����Ѵ�.
			CTransform::TRANSFORMDESC TransDesc;
			ReadFile(hFile, &TransDesc, sizeof(CTransform::TRANSFORMDESC), &dwByte, NULL);

			// Ÿ�Կ� ���� �ٸ� VIBuffer ������Ʈ�� ����Ͽ� VIBuffer�� �����.
			switch (VIBuffer(iType))
			{
			case Engine::VIB_RECT:
				// �ϴ��� ť�길 �ٸ��� ���� �߰�
				break;
			case Engine::VIB_CUBE:
				// �ؽ��� �ѹ��� ��Ÿ���� ���
				if (TransDesc.m_iTextureNumber == 21) {
					TransDesc.m_WorldMatrix._42 -= 0.5;
					pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, Layer_Map, TEXT("Prototype_GameObject_WaterCubeObject"), &TransDesc);
				}
				else
					pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, Layer_Terrain, Prototype_Object_CubeObject, &TransDesc);
				break;
			default:
				break;
			}
		}
		// ============================================================================================================


		// Layer Map
		// ============================================================================================================
		ReadFile(hFile, &iSize, sizeof(SIZE_T), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		// �о�� ������Ʈ ������ŭ �ݺ��� ����
		for (int i = 0; i < iSize; ++i) {

			// ������Ʈ�� Ÿ���� �����´�.
			int iType = 0;
			ReadFile(hFile, &iType, sizeof(int), &dwByte, nullptr);

			// �ش� ��ü�� ��������� ��� �����Ѵ�.
			CTransform::TRANSFORMDESC TransDesc;
			ReadFile(hFile, &TransDesc, sizeof(CTransform::TRANSFORMDESC), &dwByte, NULL);
			TransDesc.m_WorldMatrix._11 *= 30.f;
			TransDesc.m_WorldMatrix._12 *= 30.f;
			TransDesc.m_WorldMatrix._13 *= 30.f;
			TransDesc.m_WorldMatrix._21 *= 30.f;
			TransDesc.m_WorldMatrix._22 *= 30.f;
			TransDesc.m_WorldMatrix._23 *= 30.f;
			TransDesc.m_WorldMatrix._31 *= 30.f;
			TransDesc.m_WorldMatrix._32 *= 30.f;
			TransDesc.m_WorldMatrix._33 *= 30.f;

			CRock::ROCKDESC RockDesc;
			RockDesc.TransDesc = TransDesc;
			RockDesc.TransDesc.m_WorldMatrix._42 -= 0.2f;

			CTree::TreeDESC TreeDesc;
			TreeDesc.TransDesc = TransDesc;
			TreeDesc.TransDesc.m_WorldMatrix._42 -= 0.5f;

			CRail::RAILDESC RailDesc;
			RailDesc.TransDesc = TransDesc;

			// Ÿ�Կ� ���� �ٸ� class�� ����Ͽ� ������Ʈ�� �����.
			switch ((OBJECT_TYPE)iType)
			{
				// �ڿ� - ��
				// ========================================================================================================================
			case Engine::OBJECT_ROCK1:
				RockDesc.iRockNumber = 1;
				RockDesc.TransDesc.m_WorldMatrix._42 += 0.2f;
				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, Layer_Map, TEXT("Prototype_GameObject_Rock"), &RockDesc);
				break;
			case Engine::OBJECT_ROCK2:
				RockDesc.iRockNumber = 2;
				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, Layer_Map, TEXT("Prototype_GameObject_Rock"), &RockDesc);
				break;
			case Engine::OBJECT_ROCK3:
				RockDesc.iRockNumber = 3;
				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, Layer_Map, TEXT("Prototype_GameObject_Rock"), &RockDesc);
				break;
				// ========================================================================================================================



				// �ڿ� - ����
				// ========================================================================================================================
			case Engine::OBJECT_TREE_DARK1:
				TreeDesc.iTreeNumber = 1;
				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, Layer_Map, TEXT("Prototype_GameObject_Tree"), &TreeDesc);
				break;
			case Engine::OBJECT_TREE_DARK2:
				TreeDesc.iTreeNumber = 2;
				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, Layer_Map, TEXT("Prototype_GameObject_Tree"), &TreeDesc);
				break;
			case Engine::OBJECT_TREE_DARK3:
				TreeDesc.iTreeNumber = 3;
				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, Layer_Map, TEXT("Prototype_GameObject_Tree"), &TreeDesc);
				break;
			case Engine::OBJECT_TREE_DARK4:
				TreeDesc.iTreeNumber = 4;
				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, Layer_Map, TEXT("Prototype_GameObject_Tree"), &TreeDesc);
				break;
			case Engine::OBJECT_TREE_LIGHT:
				TreeDesc.iTreeNumber = 5;
				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, Layer_Map, TEXT("Prototype_GameObject_Tree"), &TreeDesc);
				break;
				// ========================================================================================================================



				// �ǹ� - ������
				// ========================================================================================================================
			case Engine::OBJECT_PLATFORM_START:
				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, Layer_Map, TEXT("Prototype_GameObject_TrainStation"), &TransDesc);
				break;
			case Engine::OBJECT_PLATFORM_FINISH:
				break;
				// ========================================================================================================================



				// ������Ʈ - ����
				// ========================================================================================================================
			case Engine::OBJECT_TRAIN_HEAD:
				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Train"), TEXT("Prototype_GameObject_Train_Head"), &TransDesc);
				break;
			case Engine::OBJECT_TRAIN_MATERIAL:
				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Train"), TEXT("Prototype_GameObject_Train_Material"), &TransDesc);
				break;
			case Engine::OBJECT_TRAIN_RAIL:
				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Train"), TEXT("Prototype_GameObject_Train_Rail"), &TransDesc);
				break;
			case Engine::OBJECT_TRAIN_WATER:
				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Train"), TEXT("Prototype_GameObject_Train_Water"), &TransDesc);
				break;
			case Engine::OBJECT_TRAIN_DYNAMITE:
				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Train"), TEXT("Prototype_GameObject_Train_Dynamite"), &TransDesc);
				break;
				// ========================================================================================================================



				// ������Ʈ - ����
				// ========================================================================================================================
			case Engine::OBJECT_PICKAX:
				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, Layer_Map, TEXT("Prototype_GameObject_PickAx"), &TransDesc);
				break;
			case Engine::OBJECT_AXE:
				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, Layer_Map, TEXT("Prototype_GameObject_Axe"), &TransDesc);
				break;
			case Engine::OBJECT_BUCKET:
				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, Layer_Map, TEXT("Prototype_GameObject_Bucket"), &TransDesc);
				break;
				// ========================================================================================================================



				// ������Ʈ - �߸�
				// ========================================================================================================================
			case Engine::OBJECT_CAMEL:
				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, Layer_Map, TEXT("Prototype_GameObject_Camel"), &TransDesc);
				break;
				// ========================================================================================================================



				// ������Ʈ - ����
				// ========================================================================================================================
			case Engine::OBJECT_RAIL_STRAIGHT:
			case Engine::OBJECT_RAIL_CURVE:
				RailDesc.eType_Rail = CRail::RAIL_STRAIGHT_RIGHT;
				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Rail"), TEXT("Prototype_GameObject_Rail"), &RailDesc);
				break;
			case Engine::OBJECT_RAIL_END:
				RailDesc.eType_Rail = CRail::RAIL_ENDING;
				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Rail_End"), TEXT("Prototype_GameObject_Rail_End"), &RailDesc);
				break;
				// ========================================================================================================================



				// �� �ͷ��� ������Ʈ - �� Ÿ�� ť��
				// ========================================================================================================================
			case Engine::OBJECT_MAPCUBE:
				TransDesc.m_WorldMatrix._11 /= 30.f;
				TransDesc.m_WorldMatrix._12 /= 30.f;
				TransDesc.m_WorldMatrix._13 /= 30.f;
				TransDesc.m_WorldMatrix._21 /= 30.f;
				TransDesc.m_WorldMatrix._22 /= 30.f;
				TransDesc.m_WorldMatrix._23 /= 30.f;
				TransDesc.m_WorldMatrix._31 /= 30.f;
				TransDesc.m_WorldMatrix._32 /= 30.f;
				TransDesc.m_WorldMatrix._33 /= 30.f;
				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, Layer_Terrain, TEXT("Prototype_GameObject_MapCube"), &TransDesc);
				break;
				// ========================================================================================================================

			case Engine::OBJECT_END:
				break;
			default:
				break;
			}
		}







		// Layer Unbreakable
		// ============================================================================================================
		ReadFile(hFile, &iSize, sizeof(SIZE_T), &dwByte, NULL);

		// �о�� ������Ʈ ������ŭ �ݺ��� ����
		for (int i = 0; i < iSize; ++i) {

			// ������Ʈ�� Ÿ���� �����´�.
			int iType = 0;
			ReadFile(hFile, &iType, sizeof(int), &dwByte, nullptr);

			// �ش� ��ü�� ��������� ��� �����Ѵ�.
			CTransform::TRANSFORMDESC TransDesc;
			ReadFile(hFile, &TransDesc, sizeof(CTransform::TRANSFORMDESC), &dwByte, NULL);
			TransDesc.m_WorldMatrix._11 *= 30.f;
			TransDesc.m_WorldMatrix._12 *= 30.f;
			TransDesc.m_WorldMatrix._13 *= 30.f;
			TransDesc.m_WorldMatrix._21 *= 30.f;
			TransDesc.m_WorldMatrix._22 *= 30.f;
			TransDesc.m_WorldMatrix._23 *= 30.f;
			TransDesc.m_WorldMatrix._31 *= 30.f;
			TransDesc.m_WorldMatrix._32 *= 30.f;
			TransDesc.m_WorldMatrix._33 *= 30.f;

			CRock::ROCKDESC RockDesc;
			RockDesc.TransDesc = TransDesc;
			RockDesc.TransDesc.m_WorldMatrix._42 -= 0.2f;

			// Ÿ�Կ� ���� �ٸ� class�� ����Ͽ� ������Ʈ�� �����.
			switch ((OBJECT_TYPE)iType)
			{
				// �� �ͷ��� ������Ʈ - �μ����� �ʴ� ��
				// ========================================================================================================================
			case Engine::OBJECT_UNBREAKABLE_ROCK1:
				RockDesc.iRockNumber = 1;
				RockDesc.TransDesc.m_WorldMatrix._42 += 0.2f;
				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, Layer_Map, TEXT("Prototype_GameObject_Unbreakable_Rock"), &RockDesc);
				break;
			case Engine::OBJECT_UNBREAKABLE_ROCK2:
				RockDesc.iRockNumber = 2;
				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, Layer_Map, TEXT("Prototype_GameObject_Unbreakable_Rock"), &RockDesc);
				break;
			case Engine::OBJECT_UNBREAKABLE_ROCK3:
				RockDesc.iRockNumber = 3;
				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, Layer_Map, TEXT("Prototype_GameObject_Unbreakable_Rock"), &RockDesc);
				break;
				// ========================================================================================================================
			}
		}
		// ============================================================================================================
	}
	CloseHandle(hFile);

	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_GamePlay*	pInstance = new CLevel_GamePlay(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(TEXT("Failed to Created CLevel_GamePlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();


}
