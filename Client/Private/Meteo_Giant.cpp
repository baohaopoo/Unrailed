#include "stdafx.h"
#include "..\Public\Meteo_Giant.h"
#include "GameInstance.h"
#include "Player.h"
#include "Effect_TrainSmoke.h"
#include "Effect_Explo.h"
#include "SoundMgr.h"

CMeteo_Giant::CMeteo_Giant(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
	m_eType = OBJECT_TYPE::OBJECT_METEOGIANT;
}

CMeteo_Giant::CMeteo_Giant(const CMeteo_Giant & rhs)
	: CGameObject(rhs)
{

	m_eType = rhs.m_eType;
}

HRESULT CMeteo_Giant::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMeteo_Giant::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CMeteo_Giant::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	fTime += fTimeDelta;
	//fPos -= *D3DXVec3Normalize(&vUp, &vUp) * fTimeDelta * 2.f;
	m_pTransformCom->Turn(_float3(0.f, 1.f, 1.f), fTimeDelta * 2.f);
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, fPos);


	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	CTransform*		pGiantBodyTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Giant"), TEXT("Com_Transform"));
	_float3		vBodyPosition = pGiantBodyTransform->Get_State(CTransform::STATE_POSITION);
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, vBodyPosition);
	_float3 vMeteoPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	if (fTime < 23.f && fTime > 17.5f)
	{
		fSize += 0.006f*fTimeDelta*50.f;
		if (fSize <= 1.1f)
		{
			m_pTransformCom->Scaled(_float3(fSize, fSize, fSize));
		}
	}


	if (fTime > 23.f)
	{
		if (vMeteoPosition.y < vBodyPosition.y + 29.f)
		{
			vMeteoPosition.y += 0.06f*fTimeDelta*55.f;
			vMeteoPosition.z += 0.03f*fTimeDelta*55.f;
		}

		if (vMeteoPosition.y >= vBodyPosition.y + 27.f)
		{
			fTime = 0.f;
			m_bDead = true;
		}
	}
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vMeteoPosition);



}

void CMeteo_Giant::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup_NonCulling(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CMeteo_Giant::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	auto& ObjectList = m_pVIBufferCom->Get_lstVIBuffer();

	for (auto& Object : ObjectList) {

		if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(Object.second)))
			return E_FAIL;

		Object.first->Render();
	}

	return S_OK;
}

HRESULT CMeteo_Giant::SetUp_Components(void* pArg)
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransDesc;

	if (nullptr == pArg)
		return E_FAIL;
	else {
		TransDesc = *((CTransform::TRANSFORMDESC*)pArg);
	}
	/*_float3 vPos{};
	memcpy(&vPos, TransDesc.m_WorldMatrix.m[3], sizeof(_float3));*/
	////////////////* 초기 위치잡기 (랜덤하게 잡아줌) *///////////


	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	CTransform*		pGiantBodyTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Giant"), TEXT("Com_Transform"));
	_float3		vMeteoGiantPos = pGiantBodyTransform->Get_State(CTransform::STATE_POSITION);




	_float fX = vMeteoGiantPos.x;
	_float fY = vMeteoGiantPos.y;
	_float fZ = vMeteoGiantPos.z - 20.f;

	fPos = { fX,fY,fZ };

	///////////////////////* 크기 잡기 *////////////////////////
	_float fScale = 2.f;

	TransDesc.m_WorldMatrix._11 /= fScale;
	TransDesc.m_WorldMatrix._12 /= fScale;
	TransDesc.m_WorldMatrix._13 /= fScale;

	TransDesc.m_WorldMatrix._21 /= fScale;
	TransDesc.m_WorldMatrix._22 /= fScale;
	TransDesc.m_WorldMatrix._23 /= fScale;

	TransDesc.m_WorldMatrix._31 /= fScale;
	TransDesc.m_WorldMatrix._32 /= fScale;
	TransDesc.m_WorldMatrix._33 /= fScale;
	////////////////////////*축 비틀기*////////////////////////
	_float Ux = TransDesc.m_WorldMatrix.m[1][0];
	_float Uy = TransDesc.m_WorldMatrix.m[1][1];
	_float Uz = TransDesc.m_WorldMatrix.m[1][2];

	vUp = { Ux,Uy,Uz };
	_float4x4 RotationMatrix;
	int iAngle = rand() % 49 + 1.f;
	D3DXMatrixRotationAxis(&RotationMatrix, &_float3(1.f, 0.f, 0.f), D3DXToRadian(iAngle));
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	memcpy(TransDesc.m_WorldMatrix.m[3], &fPos, sizeof(_float3));
	//////////////////////////////////////////////////////////////
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(Component_Renderer, LEVEL_STATIC, Prototype_Compo_Renderer, (CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Meteo.dat"))))
		return E_FAIL;


	if (FAILED(__super::SetUp_Components(Component_Transform, LEVEL_STATIC, Prototype_Compo_Transform, (CComponent**)&m_pTransformCom, &TransDesc)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(Component_Texture, LEVEL_STATIC, TEXT("Prototype_Component_Texture_HyunWoo"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	TransDesc.fSpeedPerSec = 0.3f;
	TransDesc.fRotationPerSec = D3DXToRadian(30.0f);
	TransDesc.fScalePerSec = 1.f;

	return S_OK;
}

CMeteo_Giant * CMeteo_Giant::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMeteo_Giant*	pInstance = new CMeteo_Giant(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created Meteo_Giant"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMeteo_Giant::Clone(void * pArg)
{
	CMeteo_Giant*	pInstance = new CMeteo_Giant(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created clone Meteo_Giant"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMeteo_Giant::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}