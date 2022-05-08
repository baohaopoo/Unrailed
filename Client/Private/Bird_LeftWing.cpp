#include "stdafx.h"
#include "Bird_LeftWing.h"
#include "GameInstance.h"

CBird_LeftWing::CBird_LeftWing(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
	m_eType = OBJECT_TYPE::OBJECT_BIRDLEFTWING;
}

CBird_LeftWing::CBird_LeftWing(const CBird_LeftWing & rhs)
	: CGameObject(rhs)
{

	m_eType = rhs.m_eType;
}

HRESULT CBird_LeftWing::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBird_LeftWing::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CBird_LeftWing::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	CTransform*		pBirdBodyTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Bird"), TEXT("Com_Transform"));
	_float3		vLeftWingPosition = pBirdBodyTransform->Get_State(CTransform::STATE_POSITION);

	//m_pTransformCom->Set_State(CTransform::STATE_RIGHT, pBirdBodyTransform->Get_State(CTransform::STATE_RIGHT));
	//m_pTransformCom->Set_State(CTransform::STATE_UP, pBirdBodyTransform->Get_State(CTransform::STATE_UP));
	//m_pTransformCom->Set_State(CTransform::STATE_LOOK, pBirdBodyTransform->Get_State(CTransform::STATE_LOOK));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { vLeftWingPosition.x, vLeftWingPosition.y, vLeftWingPosition.z });

	if (ObjectRotate == false)
	{
		ObjectTurn += 1.f;
		if (ObjectTurn >= 0.f)
		{
			ObjectRotate = true;
		}
	}

	if (ObjectRotate)
	{
		ObjectTurn -= 1.f;
		if (ObjectTurn <= -15.f)
		{
			ObjectRotate = false;
		}
	}

	_float4x4 BirdMatrix = pBirdBodyTransform->Get_WorldMatrix();
	_float4x4 worldMatrix = m_pTransformCom->Get_WorldMatrix();
	_float3 vPos{};
	memcpy(&vPos, worldMatrix.m[3], sizeof(_float3));
	D3DXMatrixRotationZ(&worldMatrix, D3DXToRadian(ObjectTurn));			// 45.f, float fNum = 0.1f;
																			//D3DXMatrixRotationZ(&worldMatrix, D3DXToRadian(ObjectTurn));
	memcpy(worldMatrix.m[3], &vPos, sizeof(_float3));
	m_pTransformCom->Set_WorldMatrix(worldMatrix*BirdMatrix);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vLeftWingPosition);

	//if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).x > 17)
	//	m_bDead = true;

}

void CBird_LeftWing::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CBird_LeftWing::Render()
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

HRESULT CBird_LeftWing::SetUp_Components(void* pArg)
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
	vPos.y = 1.5f;

	TransDesc.fSpeedPerSec = 0.3f;
	TransDesc.fRotationPerSec = D3DXToRadian(10.0f);
	TransDesc.fScalePerSec = 1.f;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(Component_Renderer, LEVEL_STATIC, Prototype_Compo_Renderer, (CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	D3DXMatrixRotationY(&TransDesc.m_WorldMatrix, D3DXToRadian(-90.f));
	// 배율
	_float fScale = 25.f;

	TransDesc.m_WorldMatrix._11 /= fScale;
	TransDesc.m_WorldMatrix._12 /= fScale;
	TransDesc.m_WorldMatrix._13 /= fScale;

	TransDesc.m_WorldMatrix._21 /= fScale;
	TransDesc.m_WorldMatrix._22 /= fScale;
	TransDesc.m_WorldMatrix._23 /= fScale;

	TransDesc.m_WorldMatrix._31 /= fScale;
	TransDesc.m_WorldMatrix._32 /= fScale;
	TransDesc.m_WorldMatrix._33 /= fScale;

	memcpy(TransDesc.m_WorldMatrix.m[3], &vPos, sizeof(_float3));

	if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Bird_LeftWing.dat"))))
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

CBird_LeftWing * CBird_LeftWing::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBird_LeftWing*	pInstance = new CBird_LeftWing(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created Bird_LeftWing"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBird_LeftWing::Clone(void * pArg)
{
	CBird_LeftWing*	pInstance = new CBird_LeftWing(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created Clone Bird_LeftWing"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBird_LeftWing::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
