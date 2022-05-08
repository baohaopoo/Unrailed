#include "stdafx.h"
#include "..\Public\PlayBackground.h"
#include "GameInstance.h"

PlayBackground::PlayBackground(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

PlayBackground::PlayBackground(const PlayBackground & rhs)
	: CGameObject(rhs)
{
}

HRESULT PlayBackground::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT PlayBackground::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;


	m_pTransformCom->Scaled(_float3(1000.f, 1000.f, 1000.f));
}

void PlayBackground::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void PlayBackground::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	_float4x4		ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	_float3		vScale = m_pTransformCom->Get_Scale();


	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_ALPHABLEND, this);
}

HRESULT PlayBackground::Render()
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

HRESULT PlayBackground::SetUp_Components(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	//ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	if (nullptr == pArg)
		return E_FAIL;
	else {
		TransformDesc = *((CTransform::TRANSFORMDESC*)pArg);
	}
	_float3 vPos{};
	memcpy(&vPos, TransformDesc.m_WorldMatrix.m[3], sizeof(_float3));
	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	TransformDesc.fScalePerSec = 1.f;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom, nullptr)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_emptyLoading"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	return S_OK;
}

PlayBackground * PlayBackground::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	PlayBackground*	pInstance = new PlayBackground(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created PlayBackground"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * PlayBackground::Clone(void * pArg)
{
	PlayBackground*	pInstance = new PlayBackground(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created PlayBackground"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void PlayBackground::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
