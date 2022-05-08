
#include "..\Public\Collider.h"
#include "GameInstance.h"

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

CCollider::CCollider(const CCollider & rhs)
	: CComponent(rhs)
{
}

HRESULT CCollider::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCollider::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCollider::Render()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	// 객체의 로컬 변환 행렬, 로컬에서의 회전, 크기, 이동 행렬이 순서대로 곱해져 있다.
	//m_TransformDesc.m_LocalMatrix = m_TransformDesc.m_LocalRotate * m_TransformDesc.m_LocalScale * m_TransformDesc.m_LocalTransform;

	_float4x4 ColliderMatrix = m_ColDesc.localMatrix;
	memcpy(ColliderMatrix.m[3], &m_pTransform->Get_State(CTransform::STATE_POSITION), sizeof(_float3));

	m_pGraphic_Device->SetTransform(D3DTS_WORLD,
		&ColliderMatrix);

	m_pVIBufferCom->Render();
}

CGameObject * CCollider::Get_CollisionObject(void)
{
	if (m_ColObjects.empty())
		return nullptr;

	return m_ColObjects.begin()->second;
}

void CCollider::Push_ColObject(CGameObject * _pObject)
{
	CTransform* pTransform = dynamic_cast<CTransform*>(_pObject->Get_Component(TEXT("Com_Transform")));

	if (nullptr == pTransform)
		return;

	const _float4x4& WorldMatrix_Parameter = pTransform->Get_WorldMatrix();
	const _float4x4& WorldMatrix_This = m_pTransform->Get_WorldMatrix();

	_float fDist =
		sqrtf(
			pow((WorldMatrix_Parameter.m[3][0] - WorldMatrix_This.m[3][0]), 2.f) +
			pow((WorldMatrix_Parameter.m[3][1] - WorldMatrix_This.m[3][1]), 2.f) +
			pow((WorldMatrix_Parameter.m[3][2] - WorldMatrix_This.m[3][2]), 2.f)
		);

	m_ColObjects[fDist] = _pObject;
}

HRESULT CCollider::SetUp_Components(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;
	else {
		m_ColDesc = *((ColliDesc*)pArg);
	}

	m_pTransform = m_ColDesc.pTransform;

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	// 큐브 오브젝트의 원형은 무조껀 LEVEL_STATIC, 즉 0레벨에 존재할 테니 인자로 0을 넘겨준다.
	m_pVIBufferCom = dynamic_cast<CVIBuffer_Cube*>(pGameInstance->Clone_Component(0, Prototype_Compo_VIB_Cube, &m_ColDesc.localMatrix));

	// 컴포넌트의 Clone에 실패했다면 E_FAIL반환
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	return S_OK;
}

CCollider * CCollider::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCollider*	pInstance = new CCollider(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created Collider component"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollider::Clone(void * pArg)
{
	CCollider*	pInstance = new CCollider(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone Collider Component"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider::Free()
{
	__super::Free();

}
