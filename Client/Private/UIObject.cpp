#include "stdafx.h"
#include "..\Public\UIObject.h"
#include "GameInstance.h"

UIObject::UIObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

UIObject::UIObject(const UIObject & rhs)
	: CGameObject(rhs)
{
}

HRESULT UIObject::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT UIObject::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pTransformCom->Scaled(_float3(0.2f, 0.3f, 0.2f));
	//ÀÌ°Å ¹Ù²ãÁÖ¸é ¾à°£ ·¹ÀÌÀú ºö ½îµíÀÌ µÊ.
	//m_pTransformCom->Set_State(CTransform::STATE_UP, _float3(1.f, 1.f, 1.f));


	return S_OK;
}

void UIObject::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();




	if (GetKeyState(VK_UP) & 0x8000)
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}

	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		m_pTransformCom->Go_BackWard(fTimeDelta);
	}

	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta);
	}

	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * -1.f);
	}

}

void UIObject::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT UIObject::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;


	m_pVIBufferCom->Render();

	//D3DXCreateSphere(m_pGraphic_Device, 1.0f, 15, 15, &mesh, 0);
	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//mesh->DrawSubset(0);
	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);



	return S_OK;
}

HRESULT UIObject::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	//ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	TransformDesc.fSpeedPerSec = 3.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	TransformDesc.fScalePerSec = 1.f;


	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), (CComponent**)&m_pVIBufferCom, nullptr)))
		return E_FAIL;

	///* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sky"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	return S_OK;
}


bool UIObject::CheckIntersect(_float3 center1, float radius1, _float3 center2, float radius2)
{
	float fDist;
	_float3 vSub;

	vSub = center2 - center1;
	fDist = D3DXVec3Length(&vSub);
	if (fDist <= (radius1 + radius2))
		return true;

	return false;
}

UIObject * UIObject::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	UIObject*	pInstance = new UIObject(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created UIObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * UIObject::Clone(void * pArg)
{
	UIObject*	pInstance = new UIObject(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created UIObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void UIObject::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
