#include "stdafx.h"
#include "..\Public\Effect_Rain.h"
#include "GameInstance.h"


CEffect_Rain::CEffect_Rain(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CEffect_Rain::CEffect_Rain(const CEffect_Rain & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect_Rain::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Rain::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CEffect_Rain::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	fTime += fTimeDelta;

	//_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3		vUp = m_pTransformCom->Get_State(CTransform::STATE_UP);

	fPos -= *D3DXVec3Normalize(&vUp, &vUp) * fTimeDelta * 30.f;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, fPos);

}

void CEffect_Rain::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	fTime += fTimeDelta;
	if (fTime >= 1.f)
		m_bDead = true;
	else
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CEffect_Rain::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(1)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	return S_OK;
}



HRESULT CEffect_Rain::SetUp_Components(void * pArg)
{
	/* For. Com_Transform */
	CTransform::TRANSFORMDESC		TransDesc;
	if (nullptr == pArg)
		return E_FAIL;
	else {
		TransDesc = *((CTransform::TRANSFORMDESC*)pArg);
	}
	TransDesc.fSpeedPerSec = 1.f;
	TransDesc.fRotationPerSec = D3DXToRadian(90.f);
	////////////////* 초기 위치잡기 (랜덤하게 잡아줌) *///////////
	_float fX = TransDesc.m_WorldMatrix.m[3][0] + rand() % 40;
	_float fY = TransDesc.m_WorldMatrix.m[3][1] + 15.f;
	_float fZ = TransDesc.m_WorldMatrix.m[3][2] + rand() % 20-10;

	fPos = { fX,fY,fZ };
	//////////////////////////////////////////////////////////////
	///////////////////////* 크기 잡기 *////////////////////////
	_float fScale = 5.f;

	TransDesc.m_WorldMatrix._11 /= fScale;
	TransDesc.m_WorldMatrix._12 /= fScale;
	TransDesc.m_WorldMatrix._13 /= fScale;

	TransDesc.m_WorldMatrix._21 /= fScale;
	TransDesc.m_WorldMatrix._22 /= fScale;
	TransDesc.m_WorldMatrix._23 /= fScale;

	TransDesc.m_WorldMatrix._31 /= fScale;
	TransDesc.m_WorldMatrix._32 /= fScale;
	TransDesc.m_WorldMatrix._33 /= fScale;
	/////////////////////////////////////////////////////////////
	memcpy(TransDesc.m_WorldMatrix.m[3], &fPos, sizeof(_float3));

	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransDesc)))
		return E_FAIL;

	/* For. Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For. Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For. Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}



CEffect_Rain * CEffect_Rain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_Rain*	pInstance = new CEffect_Rain(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("비 이펙트 원형객체 생성실패"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEffect_Rain::Clone(void * pArg)
{
	CEffect_Rain*	pInstance = new CEffect_Rain(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("비 이펙트 복사객체 생성실패"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_Rain::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
