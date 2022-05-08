#include "stdafx.h"
#include "..\Public\LobbyUI.h"
#include "GameInstance.h"
LobbyUI::LobbyUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

LobbyUI::LobbyUI(const LobbyUI & rhs)
	: CGameObject(rhs)
{
}

HRESULT LobbyUI::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT LobbyUI::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;


	m_pTransformCom->Scaled(_float3(2.f, 1.3f, 1.f)); 
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 0.88f, -9.1f));
	return S_OK;


}

void LobbyUI::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void LobbyUI::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_UI, this);
}

HRESULT LobbyUI::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;



	_float4x4			ViewMatrix, ProjMatrix;

	D3DXMatrixLookAtLH(&ViewMatrix, &_float3(0.f, 1.f, -10.f), &_float3(0.f, 0.f, 0.f), &_float3(0.f, 1.f, 0.f));
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);

	D3DXMatrixPerspectiveFovLH(&ProjMatrix, D3DXToRadian(60.0f), (_float)g_iWinCX / g_iWinCY, 0.2f, 300.f);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &ProjMatrix);

	m_pVIBufferCom->Render();

	return S_OK; 
}

HRESULT LobbyUI::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;

	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	TransformDesc.fScalePerSec = 1.f;

	/* For.Com_Tranform */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom, nullptr)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_LobbyUI"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	return S_OK;
}

LobbyUI * LobbyUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	LobbyUI*	pInstance = new LobbyUI(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created LobbyUI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * LobbyUI::Clone(void * pArg)
{
	LobbyUI*	pInstance = new LobbyUI(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created LobbyUI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void LobbyUI::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
