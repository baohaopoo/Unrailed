#include "stdafx.h"
#include "..\Public\Effect_Explo.h"
#include "GameInstance.h"


CEffect_Explo::CEffect_Explo(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CEffect_Explo::CEffect_Explo(const CEffect_Explo & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect_Explo::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Explo::NativeConstruct(void * pArg)
{
	// 추가 적인 값은 위치값으로 한다.

	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CEffect_Explo::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	fTime += fTimeDelta;

	m_vPosition.x = fPos.x + fv_X * fTime;
	m_vPosition.y = fPos.y + (fv_Y * fTime) - (0.5f * fG * fTime * fTime);
	m_vPosition.z = fPos.z + fv_Z * fTime;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPosition);

}

void CEffect_Explo::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	fTime += fTimeDelta;
	if (fTime >= 1.8f)
		m_bDead = true;
	else
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CEffect_Explo::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(m_iTextureNumber)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	return S_OK;
}



HRESULT CEffect_Explo::SetUp_Components(void * pArg)
{
	random_device(dre);
	EXPLODESC	ExploDesc;
	if (nullptr == pArg)
		return E_FAIL;
	else {
		ExploDesc = *((EXPLODESC*)pArg);
	}

	ExploDesc.TransDesc.fSpeedPerSec = 0.3f;
	ExploDesc.TransDesc.fRotationPerSec = D3DXToRadian(10.0f);
	ExploDesc.TransDesc.fScalePerSec = 1.f;

	/* 얘들은 아마도 다 같은형식으로 셋업될듯, 아닐 시 분기함.*/
	/////////////////////////////위치 받기///////////////////////////////////////
	_float fX = ExploDesc.TransDesc.m_WorldMatrix.m[3][0];
	_float fY = ExploDesc.TransDesc.m_WorldMatrix.m[3][1];
	_float fZ = ExploDesc.TransDesc.m_WorldMatrix.m[3][2];
	fPos = { fX,fY,fZ };
	m_iTextureNumber = ExploDesc.iTextureNumber;
	//////////////////////////////사이즈 조정//////////////////////////////////////
	if (ExploDesc.eExplo == ET_METEO)
	{
		ExploDesc.TransDesc.m_WorldMatrix._11 /= 3.f;
		ExploDesc.TransDesc.m_WorldMatrix._12 /= 3.f;
		ExploDesc.TransDesc.m_WorldMatrix._13 /= 3.f;
		ExploDesc.TransDesc.m_WorldMatrix._21 /= 3.f;
		ExploDesc.TransDesc.m_WorldMatrix._22 /= 3.f;
		ExploDesc.TransDesc.m_WorldMatrix._23 /= 3.f;
		ExploDesc.TransDesc.m_WorldMatrix._31 /= 3.f;
		ExploDesc.TransDesc.m_WorldMatrix._32 /= 3.f;
		ExploDesc.TransDesc.m_WorldMatrix._33 /= 3.f;
	}
	else
	{
		ExploDesc.TransDesc.m_WorldMatrix._11 /= 6.f;
		ExploDesc.TransDesc.m_WorldMatrix._12 /= 6.f;
		ExploDesc.TransDesc.m_WorldMatrix._13 /= 6.f;
		ExploDesc.TransDesc.m_WorldMatrix._21 /= 6.f;
		ExploDesc.TransDesc.m_WorldMatrix._22 /= 6.f;
		ExploDesc.TransDesc.m_WorldMatrix._23 /= 6.f;
		ExploDesc.TransDesc.m_WorldMatrix._31 /= 6.f;
		ExploDesc.TransDesc.m_WorldMatrix._32 /= 6.f;
		ExploDesc.TransDesc.m_WorldMatrix._33 /= 6.f;
	}
	///////////////////////////////튀는 방향/////////////////////////////////////

	fEndHeight = fPos.y - ExploDesc.TransDesc.m_WorldMatrix.m[3][1];
	fHeight = fMaxHeight - ExploDesc.TransDesc.m_WorldMatrix.m[3][1];

	fG = 2 * fHeight / (fMaxTime * fMaxTime);

	fv_Y = sqrtf(2 * fG * fHeight);

	float a = fG;
	float b = -2 * fv_Y;
	float c = 2 * fEndHeight;

	fEndTime = (-b + sqrtf(b * b - 4 * a * c)) / (2 * a);
	_float fEndX = ExploDesc.TransDesc.m_WorldMatrix.m[3][0] - fPos.x;
	_float fEndZ = ExploDesc.TransDesc.m_WorldMatrix.m[3][2] - fPos.z;

	if (ExploDesc.eExplo == ET_METEO)
	{
		fv_X = -(fEndX - (uid(dre) / 2.f)) / fEndTime;
		fv_Z = -(fEndZ - (uid(dre) / 2.f)) / fEndTime;
	}
	else
	{
		fv_X = -(fEndX - (uid(dre) / 10.f)) / fEndTime;
		fv_Z = -(fEndZ - (uid(dre) / 10.f)) / fEndTime;

	}


	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &ExploDesc.TransDesc)))
		return E_FAIL;

	/* For. Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For. Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For. Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_HyunWoo"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}



CEffect_Explo * CEffect_Explo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_Explo*	pInstance = new CEffect_Explo(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("폭발 이펙트 원형객체 생성실패"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEffect_Explo::Clone(void * pArg)
{
	CEffect_Explo*	pInstance = new CEffect_Explo(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("폭발 이펙트 복사객체 생성실패"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_Explo::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
