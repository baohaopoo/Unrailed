#include "stdafx.h"
#include "Rail_Ending.h"
#include "GameInstance.h"
#include "Transform.h"

CRail_Ending::CRail_Ending(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
	m_eType = OBJECT_TYPE::OBJECT_RAIL_END;
	m_eRailType = CRail::RAIL_TYPE::RAIL_ENDING;
}

CRail_Ending::CRail_Ending(const CRail_Ending & rhs)
	: CGameObject(rhs)
{
	m_eType = rhs.m_eType;

	m_eRailType = rhs.m_eRailType;
}

HRESULT CRail_Ending::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRail_Ending::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CRail_Ending::Tick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);


	__super::Tick(fTimeDelta);

	Safe_Release(pGameInstance);
}

void CRail_Ending::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CRail_Ending::Render()
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

HRESULT CRail_Ending::SetUp_Components(void* pArg)
{
	/* For.Com_Transform */
	CRail::RAILDESC			RailDesc;

	if (nullptr == pArg)
		return E_FAIL;
	else {
		RailDesc = *((CRail::RAILDESC*)pArg);
	}

	_float3 vPos{};
	memcpy(&vPos, RailDesc.TransDesc.m_WorldMatrix.m[3], sizeof(_float3));
	vPos.y = 0.5f;

	RailDesc.TransDesc.fSpeedPerSec = 0.3f;
	RailDesc.TransDesc.fRotationPerSec = D3DXToRadian(10.0f);
	RailDesc.TransDesc.fScalePerSec = 1.f;

	// ¹èÀ²
	_float fScale = 30.f;

	RailDesc.TransDesc.m_WorldMatrix._11 /= fScale;
	RailDesc.TransDesc.m_WorldMatrix._12 /= fScale;
	RailDesc.TransDesc.m_WorldMatrix._13 /= fScale;

	RailDesc.TransDesc.m_WorldMatrix._21 /= fScale;
	RailDesc.TransDesc.m_WorldMatrix._22 /= fScale;
	RailDesc.TransDesc.m_WorldMatrix._23 /= fScale;

	RailDesc.TransDesc.m_WorldMatrix._31 /= fScale;
	RailDesc.TransDesc.m_WorldMatrix._32 /= fScale;
	RailDesc.TransDesc.m_WorldMatrix._33 /= fScale;

	memcpy(RailDesc.TransDesc.m_WorldMatrix.m[3], &vPos, sizeof(_float3));

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(Component_Renderer, LEVEL_STATIC, Prototype_Compo_Renderer, (CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Rail_Straight.dat"))))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::SetUp_Components(Component_Transform, LEVEL_STATIC, Prototype_Compo_Transform, (CComponent**)&m_pTransformCom, &RailDesc.TransDesc)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(Component_Texture, LEVEL_STATIC, TEXT("Prototype_Component_Texture_HyunWoo"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	return S_OK;
}

CRail_Ending * CRail_Ending::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRail_Ending*	pInstance = new CRail_Ending(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created Rail_Ending"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRail_Ending::Clone(void * pArg)
{
	CRail_Ending*	pInstance = new CRail_Ending(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created Rail_Ending"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRail_Ending::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}