#include "stdafx.h"
#include "Water_Cube_Object.h"
#include "GameInstance.h"

CCube_Water::CCube_Water(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
	m_eType = OBJECT_TYPE::OBJECT_WATER;
}

CCube_Water::CCube_Water(const CCube_Water & rhs)
	: CGameObject(rhs)
{
	m_eType = rhs.m_eType;
}

HRESULT CCube_Water::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))	
		return E_FAIL;

	return S_OK;
}

HRESULT CCube_Water::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCube_Water::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	// Tick 관련 코드 작성

	Safe_Release(pGameInstance);
}

void CCube_Water::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CCube_Water::Render()
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

HRESULT CCube_Water::SetUp_Components(void * pArg)
{
	// 여기선 CTransform::TRANSFORMDESC 형 변수를 인자로 넣어준다.
	CTransform::TRANSFORMDESC		TransformDesc;

	if (nullptr == pArg)
		return E_FAIL;
	else
		TransformDesc = *((CTransform::TRANSFORMDESC*)pArg);

	TransformDesc.fSpeedPerSec = 0.3f;
	TransformDesc.fRotationPerSec = D3DXToRadian(10.0f);
	TransformDesc.fScalePerSec = 1.f;

	/* For.Com_Transform */
	if (FAILED(__super::SetUp_Components(Component_Transform, LEVEL_STATIC, Prototype_Compo_Transform, (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(Component_Renderer, LEVEL_STATIC, Prototype_Compo_Renderer, (CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Cube, (CComponent**)&m_pVIBufferCom, nullptr)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(Component_Texture, LEVEL_STATIC, TEXT("Prototype_Component_Texture_HyunWoo"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	m_iTextureNumber = TransformDesc.m_iTextureNumber;

	CCollider::ColliDesc colDesc;

	colDesc.pTransform = m_pTransformCom;

	/* For.Com_Collider */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Collider"), LEVEL_STATIC, TEXT("Prototype_Component_Collider"), (CComponent**)&m_pColliderCom, &colDesc)))
		return E_FAIL;

	return S_OK;
}

CCube_Water * CCube_Water::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCube_Water*	pInstance = new CCube_Water(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CCube_Water"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCube_Water::Clone(void * pArg)
{
	CCube_Water*	pInstance = new CCube_Water(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CCube_Water"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCube_Water::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
