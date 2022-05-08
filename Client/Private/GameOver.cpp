#include "stdafx.h"
#include "..\Public\GameOver.h"
#include "GameInstance.h"
#include "..\Public\Level_Loading.h"
#include "Camera_Default.h"
#include "SoundMgr.h"
GameOver::GameOver(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

HRESULT GameOver::NativeConstruct(LEVEL eNextLevel)
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

	D3DXCreateFont(m_pGraphic_Device, 99, 49, 920, 1, TRUE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"오이체", &font);

	CSoundMgr::Get_Instance()->PlayBGM(TEXT("Lobby.mp3"), 1.0f);

	return S_OK;
}

void GameOver::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	CGameInstance*		pGameInstance = CGameInstance::GetInstance();


	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_GAMEPLAY))))
			return;
	}
}

HRESULT GameOver::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	char string1[100] = "게임 오버";
	RECT rt1 = { 100,100,200,200 };
	font->DrawTextA(NULL, string1, -1, &rt1, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));



	return S_OK;
}

HRESULT GameOver::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_LobbyTerain"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT GameOver::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	//if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_UIObject"))))

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_Player"))))
		return E_FAIL;




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

HRESULT GameOver::Ready_Layer_Btn(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_Btn"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT GameOver::Ready_Layer_ProgressBar(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_ProgressBar"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT GameOver::Ready_Layer_ProgressBarExit(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_ProgressBarExit"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT GameOver::Ready_Layer_Camera(const _tchar * pLayerTag)
{

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_LOBBY, TEXT("Layer_UIObject"), TEXT("Com_Transform"));
	_float3		vPosition = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));
	CameraDesc.vEye = _float3(vPosition.x - 5.f, 5.f, -4.0f);
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

HRESULT GameOver::Ready_Layer_BtnExit(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_ExitBtn"))))
		return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT GameOver::Ready_Layer_TalkBallon(const _tchar * pLayerTag)
{
	return E_NOTIMPL;
}

GameOver * GameOver::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	GameOver*	pInstance = new GameOver(pGraphic_Device);

	//if (FAILED(pInstance->NativeConstruct(eNextLevel)))
	//{
	//	MSG_BOX(TEXT("Failed to Created GameOver"));
	//	Safe_Release(pInstance);
	//}

	return pInstance;
}

void GameOver::Free()
{
}
