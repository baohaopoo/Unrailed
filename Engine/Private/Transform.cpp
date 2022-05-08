#include "..\Public\Transform.h"


CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
	m_eDir = DIRECTION::DIR_FRONT;
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_WorldMatrix(rhs.m_WorldMatrix)
{
	m_eDir = rhs.m_eDir;
}

_float3 CTransform::Get_Scale()
{
	_float3		vRight = Get_State(CTransform::STATE_RIGHT);
	_float3		vUp = Get_State(CTransform::STATE_UP);
	_float3		vLook = Get_State(CTransform::STATE_LOOK);

	return _float3(D3DXVec3Length(&vRight), D3DXVec3Length(&vUp), D3DXVec3Length(&vLook));
}

HRESULT CTransform::NativeConstruct_Prototype()
{
	D3DXMatrixIdentity(&m_WorldMatrix);

	return S_OK;
}

HRESULT CTransform::NativeConstruct(void * pArg)
{
	if (nullptr != pArg) {
		memcpy(&m_TransformDesc, pArg, sizeof(TRANSFORMDESC));
		m_WorldMatrix = m_TransformDesc.m_WorldMatrix;
		return S_OK;
	}

	return E_FAIL;
}

HRESULT CTransform::Bind_OnGraphicDevice() const
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	// 객체의 로컬 변환 행렬, 로컬에서의 회전, 크기, 이동 행렬이 순서대로 곱해져 있다.
	//m_TransformDesc.m_LocalMatrix = m_TransformDesc.m_LocalRotate * m_TransformDesc.m_LocalScale * m_TransformDesc.m_LocalTransform;

	m_pGraphic_Device->SetTransform(D3DTS_WORLD,
		&(m_TransformDesc.m_LocalMatrix * m_WorldMatrix));

	return S_OK;
}

HRESULT CTransform::Go_Straight(_float fTimeDelta)
{
	/* 위치 += 노멀라이즈(룩) * 스피드 * FtIMNEdELTJ */
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);
	_float3		vLook = Get_State(CTransform::STATE_LOOK);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_BackWard(_float fTimeDelta)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);
	_float3		vLook = Get_State(CTransform::STATE_LOOK);

	vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Right(_float fTimeDelta)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);
	_float3		vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition += *D3DXVec3Normalize(&vRight, &vRight) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Left(_float fTimeDelta)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);
	_float3		vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition -= *D3DXVec3Normalize(&vRight, &vRight) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::LiftUp(_float fTimeDelta)		// 물건들어올리기
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);

	vPosition.y += 0.7f;


	Set_State(CTransform::STATE_POSITION, vPosition);
	return S_OK;
}

HRESULT CTransform::LiftDown(_float fTimeDelta)		// 물건내리기
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);

	vPosition.y -= 0.7f;


	Set_State(CTransform::STATE_POSITION, vPosition);
	return S_OK;
}

HRESULT CTransform::CameraShake(_float fTimeDelta)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);
	_float3		vRight = Get_State(CTransform::STATE_RIGHT);
	int i = rand() % 4;
	if (i % 2 == 0)			//오른쪽으로 간다
		vPosition.x -= 0.5f;

	else				// 왼쪽으로 간다
		vPosition.x += 0.5f;

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Scaled(_float3 vScale)
{
	_float3		vRight = Get_State(CTransform::STATE_RIGHT);
	_float3		vUp = Get_State(CTransform::STATE_UP);
	_float3		vLook = Get_State(CTransform::STATE_LOOK);

	Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScale.x);
	Set_State(CTransform::STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * vScale.y);
	Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * vScale.z);

	return S_OK;
}

HRESULT CTransform::Scaling(_float fTimeDelta)
{
	_float		fScale = 0.f;

	_float3		vRight = Get_State(CTransform::STATE_RIGHT);
	fScale = D3DXVec3Length(&vRight) + m_TransformDesc.fScalePerSec * fTimeDelta;

	Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * fScale);

	_float3		vUp = Get_State(CTransform::STATE_UP);
	fScale = D3DXVec3Length(&vUp) + m_TransformDesc.fScalePerSec * fTimeDelta;

	Set_State(CTransform::STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * fScale);

	_float3		vLook = Get_State(CTransform::STATE_LOOK);
	fScale = D3DXVec3Length(&vLook) + m_TransformDesc.fScalePerSec * fTimeDelta;

	Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * fScale);

	return S_OK;
}

HRESULT CTransform::LookAtplayer(const _float3 & vTargetPos)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);
	_float3		vScale = Get_Scale();

	_float3		vLook = vTargetPos - vPosition;
	_float3		vAxisY = _float3(0.f, 1.f, 0.f);
	_float3		vRight = *D3DXVec3Cross(&vRight, &vAxisY, &vLook);
	_float3		vUp = *D3DXVec3Cross(&vUp, &vLook, &vRight);

	vRight = *D3DXVec3Normalize(&vRight, &vRight) * vScale.x;
	vUp = *D3DXVec3Normalize(&vUp, &vUp) * vScale.y;
	vLook = *D3DXVec3Normalize(&vLook, &vLook) * vScale.z * (-1.f);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::Turn(const _float3& vAxis, _float fTimeDelta)
{
	_float4x4		RotationMatrix;

	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, fTimeDelta);

	_float3		vRight = Get_State(CTransform::STATE_RIGHT);
	_float3		vUp = Get_State(CTransform::STATE_UP);
	_float3		vLook = Get_State(CTransform::STATE_LOOK);

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::Turned(const _float3& vAxis, _float _fAngle)
{
	_float4x4		RotationMatrix;

	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, D3DXToRadian(_fAngle));

	_float3		vRight = Get_State(CTransform::STATE_RIGHT);
	_float3		vUp = Get_State(CTransform::STATE_UP);
	_float3		vLook = Get_State(CTransform::STATE_LOOK);

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::TurnAround(const _float3 & vAxis, DIRECTION _eDir)
{
	if (m_eDir != _eDir) {
		Turned(vAxis, -1 * (_float)m_eDir * 45.f);

		m_eDir = _eDir;

		Turned(vAxis, (_float)m_eDir * 45.f);
	}
	return S_OK;
}

HRESULT CTransform::TurnByOrigin(_float _fRadius, _float _phi, _float _theta, _float _fTimeDelta)
{
	_float3		vCurPos = Get_State(CTransform::STATE_POSITION);
	_float fRadius = sqrtf(vCurPos.x * vCurPos.x + vCurPos.y * vCurPos.y + vCurPos.z * vCurPos.z);

	_float3		vPos(0.f, 0.f, 0.f);
	//_phi *= _fTimeDelta;
	//_theta *= _fTimeDelta;

	D3DXMATRIX temp;
	D3DXMatrixIdentity(&temp);
	temp._41 = vCurPos.x;
	temp._42 = vCurPos.y;
	temp._43 = vCurPos.z;

	D3DXMatrixRotationX(&temp, D3DXToRadian(_phi));
	D3DXMatrixRotationY(&temp, D3DXToRadian(_theta));

	_float3		vLook = vPos - _float3(0.f, 0.f, 0.f);
	Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook));

	_float3		vRight = *D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vLook);
	Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight));

	_float3		vUp = *D3DXVec3Cross(&vUp, &vLook, &vRight);
	Set_State(CTransform::STATE_UP, *D3DXVec3Normalize(&vUp, &vUp));

	Set_State(CTransform::STATE_POSITION, vPos);
	return S_OK;
}

HRESULT CTransform::Chase(const _float3 & vTargetPos, _float fTimeDelta, _float fLimitDistance)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);

	_float3		vLook = vTargetPos - vPosition;	

	_float		fDistance = D3DXVec3Length(&vLook);

	if(fDistance > fLimitDistance)
		vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::LookAt(const _float3 & vTargetPos)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);
	_float3		vScale = Get_Scale();

	_float3		vLook = vTargetPos - vPosition;
	vLook.z = 30.f;				// 기차의 z값 고정
	_float3		vAxisY = _float3(0.f, 1.f, 0.f);
	_float3		vRight = *D3DXVec3Cross(&vRight, &vAxisY, &vLook);
	_float3		vUp = *D3DXVec3Cross(&vUp, &vLook, &vRight);

	vRight = *D3DXVec3Normalize(&vRight, &vRight) * vScale.x;
	vUp = *D3DXVec3Normalize(&vUp, &vUp) * vScale.y;
	vLook = *D3DXVec3Normalize(&vLook, &vLook) * vScale.z;

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::Go_Up(_float fTimeDelta)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);

	vPosition.y += m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
	return S_OK;
}

HRESULT CTransform::Go_Down(_float fTimeDelta)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);

	vPosition.y -= m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_JustRight(_float fTimeDelta)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);

	vPosition.x += m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
	return S_OK;
}

HRESULT CTransform::Go_JustLeft(_float fTimeDelta)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);

	vPosition.x -= m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
	return S_OK;
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTransform*	pInstance = new CTransform(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	CTransform*	pInstance = new CTransform(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();

}
