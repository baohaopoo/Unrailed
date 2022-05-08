#include "stdafx.h"
#include "..\Public\LobbyCamera.h"
#include "GameInstance.h"

LobbyCamera::LobbyCamera(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
{
}

LobbyCamera::LobbyCamera(const LobbyCamera & rhs)
	: CCamera(rhs)
{
}

HRESULT LobbyCamera::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT LobbyCamera::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

void LobbyCamera::Tick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);


	__super::Tick(fTimeDelta);

	Safe_Release(pGameInstance);
}

void LobbyCamera::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT LobbyCamera::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

LobbyCamera * LobbyCamera::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	LobbyCamera*	pInstance = new LobbyCamera(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created LobbyCamera"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * LobbyCamera::Clone(void * pArg)
{
	LobbyCamera*	pInstance = new LobbyCamera(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created LobbyCamera"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void LobbyCamera::Free()
{
}
