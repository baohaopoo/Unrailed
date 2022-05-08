#include "stdafx.h"
#include "..\Public\Level_Logo.h"
#include "..\Public\Level_GamePlay.h"
#include "GameInstance.h"
#include "Level_Lobby.h"
#include "Camera_Default.h"

CLevel_Logo::CLevel_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{

}

HRESULT CLevel_Logo::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	//로고 UI 추가
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;
	//if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
	//	return E_FAIL;
	return S_OK;
}

void CLevel_Logo::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

 	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOBBY, CLevel_Lobby::Create(m_pGraphic_Device, LEVEL_LOBBY))))
			return;
	}

	//if (GetKeyState(VK_SPACE) & 0x8000)
	//{
	///*	if (FAILED(pGameInstance->Open_Level(LEVEL_GAMEPLAY, CLevel_GamePlay::Create(m_pGraphic_Device))))
	//		return;*/


	//}
}

HRESULT CLevel_Logo::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("로고레벨입니다."));

	return S_OK;
}



HRESULT CLevel_Logo::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	// 로고 오브젝트 넣는곳 
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_LogoUI"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float3(0.f, 10.f, -15.f);
	CameraDesc.vAt = _float3(0.f, 1.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 300.0f;
	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;

	CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc.TransformDesc.fRotationPerSec = 10.f;
	CameraDesc.TransformDesc.fScalePerSec = 1.f;


	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_Camera_Default"), &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Logo*	pInstance = new CLevel_Logo(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(TEXT("Failed to Created CLevel_Logo"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();

	//Safe_Delete(pGameInstance);
	/*Safe_Release(pGameInstance);*/
}
