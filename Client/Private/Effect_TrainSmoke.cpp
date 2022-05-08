#include "stdafx.h"
#include "..\Public\Effect_TrainSmoke.h"
#include "GameInstance.h"

CEffect_TrainSmoke::CEffect_TrainSmoke(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CEffect_TrainSmoke::CEffect_TrainSmoke(const CEffect_TrainSmoke & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect_TrainSmoke::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_TrainSmoke::NativeConstruct(void * pArg)
{

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CEffect_TrainSmoke::Tick(_float fTimeDelta)
{

	fPos += *D3DXVec3Normalize(&vUp, &vUp) * fTimeDelta;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, fPos);

	__super::Tick(fTimeDelta);
}

void CEffect_TrainSmoke::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	fTime += fTimeDelta;

	if (fTime >= 1.8f)
		m_bDead = true;

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CEffect_TrainSmoke::Render()
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

void CEffect_TrainSmoke::ChangeColor(_uint iTextureNumber)
{
	m_iTextureNumber = iTextureNumber;
}

HRESULT CEffect_TrainSmoke::SetUp_Components(void * pArg)
{

	TRAINSMOKEDESC					TrainSmokeDesc;
	if (nullptr == pArg)
		return E_FAIL;
	else {
		TrainSmokeDesc = *((TRAINSMOKEDESC*)pArg);
	}

	TrainSmokeDesc.TransDesc.fSpeedPerSec = 0.3f;
	TrainSmokeDesc.TransDesc.fRotationPerSec = D3DXToRadian(10.0f);
	TrainSmokeDesc.TransDesc.fScalePerSec = 1.f;
	m_iTextureNumber = TrainSmokeDesc.iTextureNumber;
	/////////////////////////////위치 받기///////////////////////////////////////
	if (TrainSmokeDesc.eSmoke == ST_DYNAMITE)
	{
		_float fX = TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][0];
		_float fY = TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][1] + 0.8f;
		_float fZ = TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][2];

		fPos = { fX,fY,fZ };
	}
	if (TrainSmokeDesc.eSmoke == ST_TRAIN)
	{
		_float fX = TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][0];
		_float fY = TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][1];
		_float fZ = TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][2];

		fPos = { fX,fY,fZ };
	}
	if (TrainSmokeDesc.eSmoke == ST_METEO)
	{
		_float fX = TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][0] + rand() % 3 - 1.5f;
		_float fY = TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][1] + 2.f;
		_float fZ = TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][2] + rand() % 3 - 1.5f;
		fPos = { fX,fY,fZ };
	}
	//////////////////////////////사이즈 조정//////////////////////////////////////
	if (TrainSmokeDesc.eSmoke == ST_DYNAMITE || TrainSmokeDesc.eSmoke == ST_TRAIN)
	{
		_float	fScale = 5.f;
		TrainSmokeDesc.TransDesc.m_WorldMatrix._11 /= fScale;
		TrainSmokeDesc.TransDesc.m_WorldMatrix._12 /= fScale;
		TrainSmokeDesc.TransDesc.m_WorldMatrix._13 /= fScale;
		TrainSmokeDesc.TransDesc.m_WorldMatrix._21 /= fScale;
		TrainSmokeDesc.TransDesc.m_WorldMatrix._22 /= fScale;
		TrainSmokeDesc.TransDesc.m_WorldMatrix._23 /= fScale;
		TrainSmokeDesc.TransDesc.m_WorldMatrix._31 /= fScale;
		TrainSmokeDesc.TransDesc.m_WorldMatrix._32 /= fScale;
		TrainSmokeDesc.TransDesc.m_WorldMatrix._33 /= fScale;
	}
	if (TrainSmokeDesc.eSmoke == ST_METEO)
	{
		_float	fScale = 3.f;
		TrainSmokeDesc.TransDesc.m_WorldMatrix._11 /= fScale;
		TrainSmokeDesc.TransDesc.m_WorldMatrix._12 /= fScale;
		TrainSmokeDesc.TransDesc.m_WorldMatrix._13 /= fScale;
		TrainSmokeDesc.TransDesc.m_WorldMatrix._21 /= fScale;
		TrainSmokeDesc.TransDesc.m_WorldMatrix._22 /= fScale;
		TrainSmokeDesc.TransDesc.m_WorldMatrix._23 /= fScale;
		TrainSmokeDesc.TransDesc.m_WorldMatrix._31 /= fScale;
		TrainSmokeDesc.TransDesc.m_WorldMatrix._32 /= fScale;
		TrainSmokeDesc.TransDesc.m_WorldMatrix._33 /= fScale;
	}
	////////////////////////////////생성 타입////////////////////////////////
	//if (TrainSmokeDesc.eSmoke == ST_METEO)
	//	{
	_float Ux = TrainSmokeDesc.TransDesc.m_WorldMatrix.m[1][0];
	_float Uy = TrainSmokeDesc.TransDesc.m_WorldMatrix.m[1][1];
	_float Uz = TrainSmokeDesc.TransDesc.m_WorldMatrix.m[1][2];

	vUp = { Ux,Uy,Uz };
	_float4x4 RotationMatrix;
	int iAngle = rand() % 60 - 30.f;
	D3DXMatrixRotationAxis(&RotationMatrix, &_float3(1.f, 0.f, 1.f), D3DXToRadian(iAngle));
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	//}

	//if (TrainSmokeDesc.eSmoke == ST_DASH)
	//{
	//	fY = - 0.5f;
	//}
	////////////////////////////////////////////////////////////////
	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TrainSmokeDesc.TransDesc)))
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

CEffect_TrainSmoke * CEffect_TrainSmoke::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_TrainSmoke*	pInstance = new CEffect_TrainSmoke(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("CEffect_TrainSmoke 복사객체 생성실패"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEffect_TrainSmoke::Clone(void * pArg)
{
	CEffect_TrainSmoke*	pInstance = new CEffect_TrainSmoke(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("CEffect_TrainSmoke 복사객체 생성실패"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_TrainSmoke::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
