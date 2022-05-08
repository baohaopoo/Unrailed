#include "stdafx.h"
#include "Unbreakable_Rock.h"
#include "GameInstance.h"

CUnbreakable_Rock::CUnbreakable_Rock(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
	m_eType = OBJECT_TYPE::OBJECT_UNBREAKABLE_ROCK1;
}

CUnbreakable_Rock::CUnbreakable_Rock(const CUnbreakable_Rock & rhs)
	: CGameObject(rhs)
{
	m_eType = rhs.m_eType;
}

HRESULT CUnbreakable_Rock::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))	
		return E_FAIL;

	return S_OK;
}

HRESULT CUnbreakable_Rock::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CUnbreakable_Rock::Tick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	
	__super::Tick(fTimeDelta);

	Safe_Release(pGameInstance);
}

void CUnbreakable_Rock::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CUnbreakable_Rock::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	// 라이프는 최대 3, 1이되고 다음 타이밍에 삭제된다.
	auto& ObjectList = m_pVIBufferCom->Get_lstVIBuffer();

	for (auto& Object : ObjectList) {

		if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
			return E_FAIL;

		if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(Object.second)))
			return E_FAIL;

		Object.first->Render();
	}

	return S_OK;
}

HRESULT CUnbreakable_Rock::SetUp_Components(void* pArg)
{
	/* For.Com_Transform */
	CUnbreakable_Rock::ROCKDESC		RockDesc;

	if (nullptr == pArg)
		return E_FAIL;
	else {
		RockDesc = *((CUnbreakable_Rock::ROCKDESC*)pArg);
	}

	RockDesc.TransDesc.fSpeedPerSec = 0.3f;
	RockDesc.TransDesc.fRotationPerSec = D3DXToRadian(10.0f);
	RockDesc.TransDesc.fScalePerSec = 1.f;

	// rock의 스케일 조정
	RockDesc.TransDesc.m_WorldMatrix._11 /= 30.f;
	RockDesc.TransDesc.m_WorldMatrix._12 /= 30.f;
	RockDesc.TransDesc.m_WorldMatrix._13 /= 30.f;
	RockDesc.TransDesc.m_WorldMatrix._21 /= 30.f;
	RockDesc.TransDesc.m_WorldMatrix._22 /= 30.f;
	RockDesc.TransDesc.m_WorldMatrix._23 /= 30.f;
	RockDesc.TransDesc.m_WorldMatrix._31 /= 30.f;
	RockDesc.TransDesc.m_WorldMatrix._32 /= 30.f;
	RockDesc.TransDesc.m_WorldMatrix._33 /= 30.f;

	if (FAILED(__super::SetUp_Components(Component_Transform, LEVEL_STATIC, Prototype_Compo_Transform, (CComponent**)&m_pTransformCom, &RockDesc.TransDesc)))
		return E_FAIL;
	
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(Component_Renderer, LEVEL_STATIC, Prototype_Compo_Renderer, (CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	switch (RockDesc.iRockNumber)
	{
	case 1:
		if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Unbreakeable_Rock1.dat"))))
			return E_FAIL;
		m_eType = OBJECT_TYPE::OBJECT_UNBREAKABLE_ROCK1;
		break;
	case 2:
		if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Unbreakeable_Rock2.dat"))))
			return E_FAIL;
		m_eType = OBJECT_TYPE::OBJECT_UNBREAKABLE_ROCK2;
		break;
	case 3:
		if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Unbreakeable_Rock3.dat"))))
			return E_FAIL;
		m_eType = OBJECT_TYPE::OBJECT_UNBREAKABLE_ROCK3;
		break;
	default:
		break;
	}

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(Component_Texture, LEVEL_STATIC, TEXT("Prototype_Component_Texture_HyunWoo"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;



	return S_OK;
}

CUnbreakable_Rock * CUnbreakable_Rock::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUnbreakable_Rock*	pInstance = new CUnbreakable_Rock(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created Rock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUnbreakable_Rock::Clone(void * pArg)
{
	CUnbreakable_Rock*	pInstance = new CUnbreakable_Rock(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created Rock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUnbreakable_Rock::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
