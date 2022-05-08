#include "stdafx.h"
#include "..\Public\Giant_Head.h"
#include "GameInstance.h"

CGiant_Head::CGiant_Head(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
	m_eType = OBJECT_TYPE::OBJECT_GIANTHEAD;
}

CGiant_Head::CGiant_Head(const CGiant_Head & rhs)
	: CGameObject(rhs)
{

	m_eType = rhs.m_eType;
}

HRESULT CGiant_Head::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGiant_Head::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CGiant_Head::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	fTime += fTimeDelta;
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	CTransform*		pGiantBodyTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Giant"), TEXT("Com_Transform"));
	_float3		vLeftHandPosition = pGiantBodyTransform->Get_State(CTransform::STATE_POSITION);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { vLeftHandPosition.x, vLeftHandPosition.y, vLeftHandPosition.z });

}

void CGiant_Head::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	m_pRendererCom->Add_RenderGroup_NonCulling(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CGiant_Head::Render()
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

HRESULT CGiant_Head::SetUp_Components(void* pArg)
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransDesc;

	if (nullptr == pArg)
		return E_FAIL;
	else {
		TransDesc = *((CTransform::TRANSFORMDESC*)pArg);
	}
	_float3 vPos{};
	memcpy(&vPos, TransDesc.m_WorldMatrix.m[3], sizeof(_float3));

	TransDesc.fSpeedPerSec = 0.3f;
	TransDesc.fRotationPerSec = D3DXToRadian(10.0f);
	TransDesc.fScalePerSec = 1.f;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(Component_Renderer, LEVEL_STATIC, Prototype_Compo_Renderer, (CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	D3DXMatrixRotationY(&TransDesc.m_WorldMatrix, D3DXToRadian(180.f));
	// 배율
	/*_float fScale = 30.f;

	TransDesc.m_WorldMatrix._11 /= fScale;
	TransDesc.m_WorldMatrix._12 /= fScale;
	TransDesc.m_WorldMatrix._13 /= fScale;

	TransDesc.m_WorldMatrix._21 /= fScale;
	TransDesc.m_WorldMatrix._22 /= fScale;
	TransDesc.m_WorldMatrix._23 /= fScale;

	TransDesc.m_WorldMatrix._31 /= fScale;
	TransDesc.m_WorldMatrix._32 /= fScale;
	TransDesc.m_WorldMatrix._33 /= fScale;*/

	//memcpy(TransDesc.m_WorldMatrix.m[3], &vPos, sizeof(_float3));

	if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Giant_Head.dat"))))
		return E_FAIL;

	// Tree의 스케일 조정
	if (FAILED(__super::SetUp_Components(Component_Transform, LEVEL_STATIC, Prototype_Compo_Transform, (CComponent**)&m_pTransformCom, &TransDesc)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(Component_Texture, LEVEL_STATIC, TEXT("Prototype_Component_Texture_HyunWoo"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	CCollider::ColliDesc colDesc;

	colDesc.localMatrix;
	colDesc.pTransform = m_pTransformCom;
	//	m_pTransformCom->Scaled(_float3(0.1f,0.1f, 0.1f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	/* For.Com_Collider */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Collider"), LEVEL_STATIC, TEXT("Prototype_Component_Collider"), (CComponent**)&m_pColliderCom, &colDesc)))
		return E_FAIL;

	return S_OK;
}

CGiant_Head * CGiant_Head::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGiant_Head*	pInstance = new CGiant_Head(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created Giant_Head"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CGiant_Head::Clone(void * pArg)
{
	CGiant_Head*	pInstance = new CGiant_Head(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created Clone Giant_Head"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGiant_Head::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}

