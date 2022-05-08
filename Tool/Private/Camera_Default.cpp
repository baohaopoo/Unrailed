#include "stdafx.h"
#include "Camera_Default.h"
#include "GameInstance.h"

CCamera_Default::CCamera_Default(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device), m_bObserving(true)
{
}

CCamera_Default::CCamera_Default(const CCamera_Default & rhs)
	: CCamera(rhs), m_bObserving(true)
{
}

HRESULT CCamera_Default::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Default::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Default::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	// 현재 관찰하는 중이 아니라면, 카메라를 움직이지 않는다.
	if (!m_bObserving)
		return;

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	POINT ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (pGameInstance->Get_DIMButtonState(CInput_Device::MOUSEBUTTONSTATE::DIMB_LBUTTON) & 0x80) {

		if (!(ptMouse.x < 0 || ptMouse.y < 0)) {
			_long		MouseMove;

			if (MouseMove = pGameInstance->Get_DIMMoveState(CInput_Device::DIMM_X))
			{
				m_pTransform->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * MouseMove * 0.1f);
			}

			if (MouseMove = pGameInstance->Get_DIMMoveState(CInput_Device::DIMM_Y))
			{
				m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove* 0.1f);
			}
		}
	}


	if (!(ptMouse.x < 0 || ptMouse.y < 0)){
		if (pGameInstance->Get_DIKeyState(DIK_W))
		{
			m_pTransform->Go_Straight(fTimeDelta);
		}
		if (pGameInstance->Get_DIKeyState(DIK_S))
		{
			m_pTransform->Go_BackWard(fTimeDelta);
		}
		if (pGameInstance->Get_DIKeyState(DIK_A))
		{
			m_pTransform->Go_Left(fTimeDelta);
		}
		if (pGameInstance->Get_DIKeyState(DIK_D))
		{
			m_pTransform->Go_Right(fTimeDelta);
		}
	}
}

void CCamera_Default::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_Default::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

CCamera_Default * CCamera_Default::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Default*	pInstance = new CCamera_Default(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CCamera_Default"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Default::Clone(void * pArg)
{
	CCamera_Default*	pInstance = new CCamera_Default(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CCamera_Default"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Default::Free()
{
	__super::Free();

}
