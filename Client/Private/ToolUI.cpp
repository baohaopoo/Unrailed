#include "stdafx.h"
#include "..\Public\ToolUI.h"
#include "GameInstance.h"

ToolUI::ToolUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

ToolUI::ToolUI(const ToolUI & rhs)
	: CGameObject(rhs)
{
}

HRESULT ToolUI::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT ToolUI::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void ToolUI::Tick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();



	__super::Tick(fTimeDelta);
}

void ToolUI::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this); //일단 UI레벨에서 안함. 큐브오브젝트로 띄울거라서.
}

HRESULT ToolUI::Render()
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

HRESULT ToolUI::SetUp_Components(void * pArg)
{
	ToolUI::TOOLDES	ToolDesc;


	///* For.Com_Transform */
	//CTransform::TRANSFORMDESC		TransDesc;

	if (nullptr == pArg)
		return E_FAIL;
	else {
		ToolDesc = *((ToolUI::TOOLDES*)pArg);
	}

	// 배율
	_float fScale = 30.f;
	// Material_Iron의 스케일 조정
	ToolDesc.tradesc.m_WorldMatrix._11 /= 30.f;
	ToolDesc.tradesc.m_WorldMatrix._12 /= 30.f;
	ToolDesc.tradesc.m_WorldMatrix._13 /= 30.f;
	ToolDesc.tradesc.m_WorldMatrix._21 /= 30.f;
	ToolDesc.tradesc.m_WorldMatrix._22 /= 30.f;
	ToolDesc.tradesc.m_WorldMatrix._23 /= 30.f;
	ToolDesc.tradesc.m_WorldMatrix._31 /= 30.f;
	ToolDesc.tradesc.m_WorldMatrix._32 /= 30.f;
	ToolDesc.tradesc.m_WorldMatrix._33 /= 30.f;
	//memcpy(ToolDesc.tradesc.m_WorldMatrix.m[3], &vPos, sizeof(_float3));
	/* For.Com_Transform */
	if (FAILED(__super::SetUp_Components(Component_Transform, LEVEL_STATIC, Prototype_Compo_Transform, (CComponent**)&m_pTransformCom, &ToolDesc.tradesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(Component_Renderer, LEVEL_STATIC, Prototype_Compo_Renderer, (CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;
	


	switch (ToolDesc.tk)
	{
	case  TOOLKIND::AXE:
		/* For.Com_VIBuffer */
		if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Axe_UI.dat"))))
			return E_FAIL;
		break;
	case TOOLKIND::PICKAXE:
		if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("PickAxe_UI.dat"))))
			return E_FAIL;
		break;
	case TOOLKIND::BUCKET:
		if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Bucket_UI.dat"))))
			return E_FAIL;
		break;

	case TOOLKIND::WOOD:
		if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Wood_Iron_UI.dat"))))
			return E_FAIL;
		break;
	case TOOLKIND::IRON:
		if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Wood_Iron_UI.dat"))))
			return E_FAIL;
		break;

	case TOOLKIND::WATER:

		if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Bucket_Full_UI.dat"))))
			return E_FAIL;
		break;

	case TOOLKIND::WARNING:
		if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Warning_UI1.dat"))))
			return E_FAIL;
		break;

	default:
		break;
	}


	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(Component_Texture, LEVEL_STATIC, TEXT("Prototype_Component_Texture_HyunWoo"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	CCollider::ColliDesc colDesc;
	colDesc.pTransform = m_pTransformCom;



	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	/* For.Com_Collider */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Collider"), LEVEL_STATIC, TEXT("Prototype_Component_Collider"), (CComponent**)&m_pColliderCom, &colDesc)))
		return E_FAIL;

	return S_OK;
}

ToolUI * ToolUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	ToolUI*	pInstance = new ToolUI(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created ToolUI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * ToolUI::Clone(void * pArg)
{
	ToolUI*	pInstance = new ToolUI(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone ToolUI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void ToolUI::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
