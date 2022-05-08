#include "stdafx.h"
#include "Rail.h"
#include "GameInstance.h"
#include "Transform.h"

CRail::CRail(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
	m_eType = OBJECT_TYPE::OBJECT_RAIL_STRAIGHT;
	m_eRailType = RAIL_TYPE::RAIL_END;
}

CRail::CRail(const CRail & rhs)
	: CGameObject(rhs)
{
	m_eType = rhs.m_eType;

	m_eRailType = rhs.m_eRailType;
}

HRESULT CRail::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRail::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CRail::Tick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);


	__super::Tick(fTimeDelta);

	Safe_Release(pGameInstance);
}

void CRail::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CRail::Render()
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

HRESULT CRail::SetUp_Components(void* pArg)
{
	/* For.Com_Transform */
	RAILDESC			RailDesc;

	if (nullptr == pArg)
		return E_FAIL;
	else {
		RailDesc = *((RAILDESC*)pArg);
	}

	_float3 vPos{};
	memcpy(&vPos, RailDesc.TransDesc.m_WorldMatrix.m[3], sizeof(_float3));
	vPos.y = 0.5f;


	RailDesc.TransDesc.fSpeedPerSec = 0.3f;
	RailDesc.TransDesc.fRotationPerSec = D3DXToRadian(10.0f);
	RailDesc.TransDesc.fScalePerSec = 1.f;

	// 배율
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
	m_eRailType = RailDesc.eType_Rail;
	switch (RailDesc.eType_Rail)
	{
	case Client::CRail::RAIL_STRAIGHT_UP:
	case Client::CRail::RAIL_STRAIGHT_DOWN:
	case Client::CRail::RAIL_STRAIGHT_LEFT:
	case Client::CRail::RAIL_STRAIGHT_RIGHT:
		if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Rail_Straight.dat"))))
			return E_FAIL;
		break;

	case Client::CRail::RAIL_ROTATE_LEFT_UP:
	case Client::CRail::RAIL_ROTATE_LEFT_DOWN:
	case Client::CRail::RAIL_ROTATE_RIGHT_UP:
	case Client::CRail::RAIL_ROTATE_RIGHT_DOWN:
	case Client::CRail::RAIL_ROTATE_UP_LEFT:
	case Client::CRail::RAIL_ROTATE_UP_RIGHT:
	case Client::CRail::RAIL_ROTATE_DOWN_LEFT:
	case Client::CRail::RAIL_ROTATE_DOWN_RIGHT:
		if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Rail_Turned.dat"))))
			return E_FAIL;
		break;

	case Client::CRail::RAIL_END:
		break;
	default:
		break;
	}


	_float4x4 RotateMatrix;
	switch (RailDesc.eType_Rail)
	{
	case Client::CRail::RAIL_STRAIGHT_UP:
	case Client::CRail::RAIL_STRAIGHT_DOWN:
		D3DXMatrixRotationY(&RotateMatrix, D3DXToRadian(90));
		break;
	case Client::CRail::RAIL_STRAIGHT_LEFT:
	case Client::CRail::RAIL_STRAIGHT_RIGHT:
		D3DXMatrixRotationY(&RotateMatrix, D3DXToRadian(0));
		break;



	case Client::CRail::RAIL_ROTATE_RIGHT_DOWN:
	case Client::CRail::RAIL_ROTATE_UP_LEFT:
		D3DXMatrixRotationY(&RotateMatrix, D3DXToRadian(0));
		break;
	case Client::CRail::RAIL_ROTATE_RIGHT_UP:
	case Client::CRail::RAIL_ROTATE_DOWN_LEFT:
		D3DXMatrixRotationY(&RotateMatrix, D3DXToRadian(90));
		break;
	case Client::CRail::RAIL_ROTATE_DOWN_RIGHT:
	case Client::CRail::RAIL_ROTATE_LEFT_UP:
		D3DXMatrixRotationY(&RotateMatrix, D3DXToRadian(180));
		break;
	case Client::CRail::RAIL_ROTATE_LEFT_DOWN:
	case Client::CRail::RAIL_ROTATE_UP_RIGHT:
		D3DXMatrixRotationY(&RotateMatrix, D3DXToRadian(270));
		break;

	case Client::CRail::RAIL_END:
		break;
	default:
		break;
	}

	// y축 회전 행렬에 월드 매트릭스를 곱했다. => 제자리 회전
	RailDesc.TransDesc.m_WorldMatrix = RotateMatrix * RailDesc.TransDesc.m_WorldMatrix;

	/* For.Com_Transform */
	if (FAILED(__super::SetUp_Components(Component_Transform, LEVEL_STATIC, Prototype_Compo_Transform, (CComponent**)&m_pTransformCom, &RailDesc.TransDesc)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(Component_Texture, LEVEL_STATIC, TEXT("Prototype_Component_Texture_HyunWoo"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	return S_OK;
}

CRail * CRail::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRail*	pInstance = new CRail(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created Rail_Straight"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRail::Clone(void * pArg)
{
	CRail*	pInstance = new CRail(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created Rail_Straight"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRail::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}

void CRail::Change_RailType(RAIL_TYPE eType)
{
	Delete_Component(Component_VIBuffer);

	switch (eType)
	{
	case Client::CRail::RAIL_STRAIGHT_UP:
	case Client::CRail::RAIL_STRAIGHT_DOWN:
	case Client::CRail::RAIL_STRAIGHT_LEFT:
	case Client::CRail::RAIL_STRAIGHT_RIGHT:
		if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Rail_Straight.dat"))))
			return;
		break;

	case Client::CRail::RAIL_ROTATE_LEFT_UP:
	case Client::CRail::RAIL_ROTATE_LEFT_DOWN:
	case Client::CRail::RAIL_ROTATE_RIGHT_UP:
	case Client::CRail::RAIL_ROTATE_RIGHT_DOWN:
	case Client::CRail::RAIL_ROTATE_UP_LEFT:
	case Client::CRail::RAIL_ROTATE_UP_RIGHT:
	case Client::CRail::RAIL_ROTATE_DOWN_LEFT:
	case Client::CRail::RAIL_ROTATE_DOWN_RIGHT:
		if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Rail_Turned.dat"))))
			return;
		break;

	case Client::CRail::RAIL_END:
		break;
	default:
		break;
	}


	RAILDESC			RailDesc;

	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	RailDesc.TransDesc.fSpeedPerSec = 0.3f;
	RailDesc.TransDesc.fRotationPerSec = D3DXToRadian(10.0f);
	RailDesc.TransDesc.fScalePerSec = 1.f;

	// 배율
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

	_float4x4 RotateMatrix;
	switch (eType)
	{
	case Client::CRail::RAIL_STRAIGHT_UP:
	case Client::CRail::RAIL_STRAIGHT_DOWN:
		D3DXMatrixRotationY(&RotateMatrix, D3DXToRadian(90));
		break;
	case Client::CRail::RAIL_STRAIGHT_LEFT:
	case Client::CRail::RAIL_STRAIGHT_RIGHT:
		D3DXMatrixRotationY(&RotateMatrix, D3DXToRadian(0));
		break;

	case Client::CRail::RAIL_ROTATE_RIGHT_DOWN:
	case Client::CRail::RAIL_ROTATE_UP_LEFT:
		D3DXMatrixRotationY(&RotateMatrix, D3DXToRadian(0));
		break;
	case Client::CRail::RAIL_ROTATE_RIGHT_UP:
	case Client::CRail::RAIL_ROTATE_DOWN_LEFT:
		D3DXMatrixRotationY(&RotateMatrix, D3DXToRadian(90));
		break;
	case Client::CRail::RAIL_ROTATE_DOWN_RIGHT:
	case Client::CRail::RAIL_ROTATE_LEFT_UP:
		D3DXMatrixRotationY(&RotateMatrix, D3DXToRadian(180));
		break;
	case Client::CRail::RAIL_ROTATE_LEFT_DOWN:
	case Client::CRail::RAIL_ROTATE_UP_RIGHT:
		D3DXMatrixRotationY(&RotateMatrix, D3DXToRadian(270));
		break;

	case Client::CRail::RAIL_END:
		break;
	default:
		break;
	}

	// y축 회전 행렬에 월드 매트릭스를 곱했다. => 제자리 회전
	RailDesc.TransDesc.m_WorldMatrix = RotateMatrix * RailDesc.TransDesc.m_WorldMatrix;

	Delete_Component(Component_Transform);

	/* For.Com_Transform */
	if (FAILED(__super::SetUp_Components(Component_Transform, LEVEL_STATIC, Prototype_Compo_Transform, (CComponent**)&m_pTransformCom, &RailDesc.TransDesc)))
		return;

	m_eRailType = eType;

}
