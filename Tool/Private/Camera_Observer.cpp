#include "stdafx.h"
#include "Camera_Observer.h"
#include "GameInstance.h"

CCamera_Observer::CCamera_Observer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
{
}

CCamera_Observer::CCamera_Observer(const CCamera_Observer & rhs)
	: CCamera(rhs)
{
	m_fRadius = rhs.m_fRadius;
}

HRESULT CCamera_Observer::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_fRadius = 20.f;

	return S_OK;
}

HRESULT CCamera_Observer::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Observer::Tick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	if (pGameInstance->Get_DIMButtonState(CInput_Device::MOUSEBUTTONSTATE::DIMB_LBUTTON) & 0x80) {
		_long		MouseMove;

		if (MouseMove = pGameInstance->Get_DIMMoveState(CInput_Device::DIMM_X))
		{
			m_pTransform->TurnByOrigin(m_fRadius, 10.f, 0.f, fTimeDelta * MouseMove * 0.1f);
		}

		if (MouseMove = pGameInstance->Get_DIMMoveState(CInput_Device::DIMM_Y))
		{
			m_pTransform->TurnByOrigin(m_fRadius, 0.f, 10.f, fTimeDelta * MouseMove * 0.1f);
		}	

		//if (MouseMove = pGameInstance->Get_DIMMoveState(CInput_Device::DIMM_X))
		//{
		//	m_pTransform->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * MouseMove * 0.1f);
		//}

		//if (MouseMove = pGameInstance->Get_DIMMoveState(CInput_Device::DIMM_Y))
		//{
		//	m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove* 0.1f);
		//}
	}

	__super::Tick(fTimeDelta);
}

void CCamera_Observer::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_Observer::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

CCamera_Observer * CCamera_Observer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Observer*	pInstance = new CCamera_Observer(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CCamera_Default"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Observer::Clone(void * pArg)
{
	CCamera_Observer*	pInstance = new CCamera_Observer(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CCamera_Default"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Observer::Free()
{
	__super::Free();

}
