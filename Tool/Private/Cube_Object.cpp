#include "stdafx.h"
#include "Cube_Object.h"
#include "GameInstance.h"

CCube_Object::CCube_Object(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
	m_eType = OBJECT_TYPE::OBJECT_MAPCUBE;
}

CCube_Object::CCube_Object(const CCube_Object & rhs)
	: CGameObject(rhs)
{
	m_eType = rhs.m_eType;
}

HRESULT CCube_Object::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))	
		return E_FAIL;

	return S_OK;
}

HRESULT CCube_Object::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCube_Object::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	// Tick 관련 코드 작성

	Safe_Release(pGameInstance);
}

void CCube_Object::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CCube_Object::Render()
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

HRESULT CCube_Object::SetUp_Components(void * pArg)
{
	// 여기선 CTransform::TRANSFORMDESC 형 변수를 인자로 넣어준다.
	CTransform::TRANSFORMDESC		TransformDesc;

	if (nullptr == pArg)
		E_FAIL;
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

	return S_OK;
}

CCube_Object * CCube_Object::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCube_Object*	pInstance = new CCube_Object(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CCube_Object"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCube_Object::Clone(void * pArg)
{
	CCube_Object*	pInstance = new CCube_Object(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CCube_Object"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCube_Object::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
