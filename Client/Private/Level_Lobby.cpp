#include "stdafx.h"
#include "..\Public\Level_Lobby.h"
#include "GameInstance.h"
#include "..\Public\Level_Loading.h"
#include "Camera_Default.h"
#include "SoundMgr.h"

CLevel_Lobby::CLevel_Lobby(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{

}

HRESULT CLevel_Lobby::NativeConstruct(LEVEL eNextLevel)
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	//// 로비UI랑 버튼 추가
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_UIObject"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_TalkBallon(TEXT("Layer_TalkBallon"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;


	if (FAILED(Ready_Layer_Btn(TEXT("Layer_LobbyBtn"))))	//시작버튼
		return E_FAIL;

	if (FAILED(Ready_Layer_BtnExit(TEXT("Layer_LobbyExitBtn"))))		//나가기버튼
		return E_FAIL;



	
	D3DXCreateFont(m_pGraphic_Device, 99, 49, 920, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"오이체", &font);


	D3DXCreateFont(m_pGraphic_Device, 99, 49, 920, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"오이체", &font2);


	CSoundMgr::Get_Instance()->PlayBGM(TEXT("Lobby.mp3"), 1.0f);

	return S_OK;
}

void CLevel_Lobby::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	
	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PROGRESS_BAR);
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_GAMEPLAY))))
			return;
	}


		
}

HRESULT CLevel_Lobby::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	char string1[100] = "로비";
	RECT rt1 = { 100,100,200,200 };
	font->DrawTextA(NULL, string1, -1, &rt1, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	


	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_LobbyTerain"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	//if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_UIObject"))))


	CTransform::TRANSFORMDESC tradesc;
	tradesc.m_WorldMatrix.m[0][0] = 0.001f;
	tradesc.m_WorldMatrix.m[1][1] = 0.001f;
	tradesc.m_WorldMatrix.m[2][2] = 0.001f;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_Player"),&tradesc)))
		return E_FAIL;



	//깍두기..
	/*if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_LeftFeet"),
		CPlayer_LeftFeet::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_LeftHand"),
		CPlayer_LeftHand::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_RightHand"),
		CPlayer_RightHand::Create(m_pGraphic_Device))))
		return E_FAIL;*/

	//if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_Player_Feet"))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Player_LeftFeet"))))
	//	return E_FAIL;






	CTransform*		terrain = (CTransform*)pGameInstance->Get_Component(LEVEL_LOBBY, TEXT("Layer_BackGround"), TEXT("Com_Transform"));
	_float3 terrainpos = terrain->Get_State(CTransform::STATE_POSITION);


	CTransform*		m_pTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_LOBBY, TEXT("Layer_UIObject"), TEXT("Com_Transform"));
	m_pTransform->Set_State(CTransform::STATE_POSITION, { terrainpos.x, terrainpos.y, terrainpos.z });



	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	//CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	//Safe_AddRef(pGameInstance);

	////CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_LOBBY, TEXT("Layer_UIObject"), TEXT("Com_Transform"));
	////_float3		vPosition = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	//CTransform*		pTerrainTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_LOBBY, TEXT("Layer_BackGround"), TEXT("Com_Transform"));
	//_float3 vPositionTerrain = pTerrainTransform->Get_State(CTransform::STATE_POSITION);

	//CCamera::CAMERADESC			CameraDesc;
	//ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	//CameraDesc.vEye = _float3(vPositionTerrain.x, 4.f, 2.f);
	//CameraDesc.vAt = _float3(vPositionTerrain.x, 0.f, vPositionTerrain.z);
	//CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);
	//CameraDesc.fNear = 0.1f;
	//CameraDesc.fFar = 300.0f;
	//CameraDesc.fFovy = D3DXToRadian(60.0f);
	//CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;

	//CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	//CameraDesc.TransformDesc.fRotationPerSec = 10.f;
	//CameraDesc.TransformDesc.fScalePerSec = 1.f;


	//if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_Camera_Default"), &CameraDesc)))
	//	return E_FAIL;

	//Safe_Release(pGameInstance);
	//return S_OK;


	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_LOBBY, TEXT("Layer_UIObject"), TEXT("Com_Transform"));
	_float3		vPosition = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));
	CameraDesc.vEye = _float3(vPosition.x-1.f, 21.f, -2.f);
	CameraDesc.vAt = _float3(vPosition.x, 0.f, vPosition.z);

	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 300.0f;
	CameraDesc.fFovy = D3DXToRadian(30.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;


	CameraDesc.TransformDesc.fSpeedPerSec = 2.f;
	CameraDesc.TransformDesc.fRotationPerSec = 10.f;
	CameraDesc.TransformDesc.fScalePerSec = 1.f;


	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_Camera_Default"), &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Layer_Btn(const _tchar * pLayerTag)
{

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_Btn"))))
		return E_FAIL;


	//if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_LifeUI"))))
	//	return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Layer_ProgressBar(const _tchar * pLayerTag)
{

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_ProgressBar"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Layer_ProgressBarExit(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_ProgressBarExit"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}



HRESULT CLevel_Lobby::Ready_Layer_BtnExit(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_ExitBtn"))))
		return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Layer_TalkBallon(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_TalkBallon"))))
		return E_FAIL;


	//CTransform*		object = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_UIObject"), TEXT("Com_Transform"));
	//_float3 objectpos = object->Get_State(CTransform::STATE_POSITION);



	//CTransform*		m_pTransform1 = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_TalkBallon"), TEXT("Com_Transform"));
	//m_pTransform1->Set_State(CTransform::STATE_POSITION, { objectpos.x+1, objectpos.y+1 , objectpos.z });

		return S_OK;
}


CLevel_Lobby * CLevel_Lobby::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel)
{
	CLevel_Lobby*	pInstance = new CLevel_Lobby(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(eNextLevel)))
	{
		MSG_BOX(TEXT("Failed to Created CLevel_Lobby"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Lobby::Free()
{
	__super::Free();


}
