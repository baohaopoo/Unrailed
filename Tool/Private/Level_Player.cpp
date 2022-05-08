#include "stdafx.h"
#include "Level_Player.h"
#include "GameInstance.h"
#include "Camera_Default.h"

CLevel_Player::CLevel_Player(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{

}

HRESULT CLevel_Player::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	

	//if (FAILED(Ready_Layer_Camera(Layer_Camera_Player)))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_Player(Layer_Player)))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_Terrain(Layer_Terrain)))
	//	return E_FAIL;

	return S_OK;
}

void CLevel_Player::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
		
}

HRESULT CLevel_Player::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Player::Ready_Layer_Camera(const _tchar * pLayerTag)
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
	CameraDesc.fAspect = (_float)Resolution.x / Resolution.y;

	CameraDesc.TransformDesc.fSpeedPerSec = 1.f;
	CameraDesc.TransformDesc.fRotationPerSec = 1.f;
	CameraDesc.TransformDesc.fScalePerSec = 1.f;


	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_PLAYER, pLayerTag, Prototype_Object_Camera_Observer, &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Player::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_PLAYER, pLayerTag, Prototype_Object_Player)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Player::Ready_Layer_Terrain(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_PLAYER, pLayerTag,Prototype_Object_Terrain)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_Player * CLevel_Player::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Player*	pInstance = new CLevel_Player(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(TEXT("Failed to Created CLevel_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Player::Free()
{
	__super::Free();


}
