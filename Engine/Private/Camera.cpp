#include "..\Public\Camera.h"

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device, string("camera"))
{
}

CCamera::CCamera(const CCamera & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCamera::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CCamera::NativeConstruct(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	memcpy(&m_CameraDesc, pArg, sizeof(CAMERADESC));

	m_pTransform = CTransform::Create(m_pGraphic_Device);
	if (nullptr == m_pTransform)
		return E_FAIL;

	m_pTransform->Set_TransformState(m_CameraDesc.TransformDesc);
	
	_float3		vLook = m_CameraDesc.vAt - m_CameraDesc.vEye;	
	m_pTransform->Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook));

	_float3		vRight = *D3DXVec3Cross(&vRight, &m_CameraDesc.vAxisY, &vLook);
	m_pTransform->Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight));

	_float3		vUp = *D3DXVec3Cross(&vUp, &vLook, &vRight);
	m_pTransform->Set_State(CTransform::STATE_UP, *D3DXVec3Normalize(&vUp, &vUp));

	m_pTransform->Set_State(CTransform::STATE_POSITION, m_CameraDesc.vEye);

	return S_OK;
}

void CCamera::Tick(_float fTimeDelta)
{
	_float4x4 WorldMatrix = m_pTransform->Get_WorldMatrix();

	D3DXMatrixInverse(&WorldMatrix, nullptr, &WorldMatrix);

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &WorldMatrix);

	_float4x4 ProjMatrix;

	D3DXMatrixPerspectiveFovLH(&ProjMatrix, m_CameraDesc.fFovy, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &ProjMatrix);


}

void CCamera::LateTick(_float fTimeDelta)
{
}

HRESULT CCamera::Render()
{
	return S_OK;
}

void CCamera::Free()
{
	__super::Free();
	
	Safe_Release(m_pTransform);
}
