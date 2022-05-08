#include "stdafx.h"
#include "..\Public\LobbyTerrain.h"
#include "GameInstance.h"



LobbyTerrain::LobbyTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

LobbyTerrain::LobbyTerrain(const LobbyTerrain & rhs)
	: CGameObject(rhs)
{
}

HRESULT LobbyTerrain::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;




	return S_OK;
}

HRESULT LobbyTerrain::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

void LobbyTerrain::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void LobbyTerrain::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT LobbyTerrain::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT LobbyTerrain::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	//ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(60.0f);
	TransformDesc.fScalePerSec = 2.f;
	TransformDesc.m_WorldMatrix.m[0][0] = 3.f;
	TransformDesc.m_WorldMatrix.m[1][1] = 3.f;
	TransformDesc.m_WorldMatrix.m[2][2] = 3.f;




	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain"), (CComponent**)&m_pVIBufferCom, nullptr)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_LobbyUI"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	return S_OK;
}

LobbyTerrain * LobbyTerrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	LobbyTerrain*	pInstance = new LobbyTerrain(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created LobbyTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * LobbyTerrain::Clone(void * pArg)
{
	LobbyTerrain*	pInstance = new LobbyTerrain(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created LobbyTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void LobbyTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
