#include "stdafx.h"
#include "Material_Iron.h"
#include "GameInstance.h"

CMaterial_Iron::CMaterial_Iron(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
	m_eType = (OBJECT_TYPE)(int)OBJECT_TYPE::OBJECT_MATERIAL_IRON;
}

CMaterial_Iron::CMaterial_Iron(const CMaterial_Iron & rhs)
	: CGameObject(rhs)
{
	m_eType = rhs.m_eType;
}

HRESULT CMaterial_Iron::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMaterial_Iron::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CMaterial_Iron::Tick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	
	__super::Tick(fTimeDelta);

	Safe_Release(pGameInstance);
}

void CMaterial_Iron::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CMaterial_Iron::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	// 라이프는 최대 3, 1이되고 다음 타이밍에 삭제된다.
	auto& ObjectList = m_pVIBufferCom->Get_lstVIBuffer();

	for (auto& Object : ObjectList) {

		if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(Object.second)))
			return E_FAIL;

		Object.first->Render();
	}

	return S_OK;
}

HRESULT CMaterial_Iron::SetUp_Components(void* pArg)
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransDesc;

	if (nullptr == pArg)
		return E_FAIL;
	else {
		TransDesc = *((CTransform::TRANSFORMDESC*)pArg);
	}

	TransDesc.fSpeedPerSec = 0.3f;
	TransDesc.fRotationPerSec = D3DXToRadian(10.0f);
	TransDesc.fScalePerSec = 1.f;

	// Material_Iron의 스케일 조정
	TransDesc.m_WorldMatrix._11 /= 30.f;
	TransDesc.m_WorldMatrix._12 /= 30.f;
	TransDesc.m_WorldMatrix._13 /= 30.f;
	TransDesc.m_WorldMatrix._21 /= 30.f;
	TransDesc.m_WorldMatrix._22 /= 30.f;
	TransDesc.m_WorldMatrix._23 /= 30.f;
	TransDesc.m_WorldMatrix._31 /= 30.f;
	TransDesc.m_WorldMatrix._32 /= 30.f;
	TransDesc.m_WorldMatrix._33 /= 30.f;

	/* For.Com_Transform */
	if (FAILED(__super::SetUp_Components(Component_Transform, LEVEL_STATIC, Prototype_Compo_Transform, (CComponent**)&m_pTransformCom, &TransDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(Component_Renderer, LEVEL_STATIC, Prototype_Compo_Renderer, (CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Material_Iron.dat"))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(Component_Texture, LEVEL_STATIC, TEXT("Prototype_Component_Texture_HyunWoo"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	return S_OK;
}

CMaterial_Iron * CMaterial_Iron::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMaterial_Iron*	pInstance = new CMaterial_Iron(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created Material_Iron"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMaterial_Iron::Clone(void * pArg)
{
	CMaterial_Iron*	pInstance = new CMaterial_Iron(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone Material_Iron"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMaterial_Iron::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
