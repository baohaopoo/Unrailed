#include "..\Public\Animation.h"
#include "GameObject.h"

CAnimation::CAnimation(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

HRESULT CAnimation::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CAnimation::NativeConstruct(void * pArg)
{
	InitAnimation();
	return S_OK;
}

void CAnimation::InitAnimation()
{
	m_AniPos[0] = D3DXVECTOR3(0, 0, 0);	// 위치 키
	m_AniPos[1] = D3DXVECTOR3(5, 5, 5); //	 ""

	_float Yaw = D3DX_PI * 90.f / 180.f; // y축 90도 회전
	_float Pitch = 0;
	_float Roll = 0;

	D3DXQuaternionRotationYawPitchRoll(&m_AniRot[0], Yaw, Pitch, Roll);	// 사원수 키 y축 90도

	Yaw = 0;
	Pitch = D3DX_PI * 90.f / 180.f;
	Roll = 0;

	D3DXQuaternionRotationYawPitchRoll(&m_AniRot[1], Yaw, Pitch, Roll);	// 사원수 키 x축 90도
}

_float CAnimation::Linear(_float v0, _float v1, _float t)
{
	return v0 + t * (v1 - v0);

}


HRESULT CAnimation::Animation()
{
	_float t = 0.f;
	_float x, y, z;
	D3DXQUATERNION	quat;


	if (t > 1.0f)
		t = 0.0f;

	_float4x4	matA;

	D3DXVECTOR3 v;
	D3DXVec3Lerp(&v, &m_AniPos[0], &m_AniPos[1], t);
	D3DXMatrixTranslation(&matA, v.x, v.y, v.z);	// 부모의 이동행렬...

													//D3DXQuaternionSlerp(&quat, &m_AniRot[0], &m_AniRot[1], t);
													//D3DXMatrixRotationQuaternion(,)
	
	return S_OK;
}

CAnimation * CAnimation::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CAnimation*	pInstance = new CAnimation(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CAnimation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CAnimation::Clone(void * pArg)
{
	AddRef();

	return this;
}

void CAnimation::Free()
{
	__super::Free();
}


