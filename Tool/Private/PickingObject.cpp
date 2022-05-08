#include "stdafx.h"
#include "PickingObject.h"
#include "GameInstance.h"

CPickingObject::CPickingObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CPickingObject::CPickingObject(const CPickingObject & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPickingObject::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))	
		return E_FAIL;

	return S_OK;
}

HRESULT CPickingObject::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CPickingObject::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	// Tick 관련 코드 작성

	Safe_Release(pGameInstance);
}

void CPickingObject::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CPickingObject::Render()
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

HRESULT CPickingObject::SetUp_Components(void * pArg)
{
	POINT pt{};

	if (nullptr == pArg)
		return E_FAIL;
	else {
		pt = *((POINT*)pArg);
	}

	CTransform::TRANSFORMDESC		TransformDesc;

	/* For.Com_Transform */
	if (FAILED(__super::SetUp_Components(Component_Transform, LEVEL_STATIC, Prototype_Compo_Transform, (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(Component_Renderer, LEVEL_STATIC, Prototype_Compo_Renderer, (CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Terrain, (CComponent**)&m_pVIBufferCom, &pt)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(Component_Texture, LEVEL_STATIC, Prototype_Compo_Texture_Terrain, (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	return S_OK;
}

CPickingObject * CPickingObject::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPickingObject*	pInstance = new CPickingObject(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created PickingObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPickingObject::Clone(void * pArg)
{
	CPickingObject*	pInstance = new CPickingObject(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Load PickingObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPickingObject::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
