
#include "..\Public\Frustum_Manager.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "Layer.h"

IMPLEMENT_SINGLETON(CFrustum_Manager)

CFrustum_Manager::CFrustum_Manager()
{
}

CFrustum_Manager::CFrustum_Manager(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: m_pGraphic_Device(_pGraphic_Device)
{
	m_vfLocalFrustumPoints[0] = _float3(-1.f, 1.f, 0.f);
	m_vfLocalFrustumPoints[1] = _float3(1.f, 1.f, 0.f);
	m_vfLocalFrustumPoints[2] = _float3(1.f, -1.f, 0.f);
	m_vfLocalFrustumPoints[3] = _float3(-1.f, -1.f, 0.f);
	m_vfLocalFrustumPoints[4] = _float3(-1.f, 1.f, 1.f);
	m_vfLocalFrustumPoints[5] = _float3(1.f, 1.f, 1.f);
	m_vfLocalFrustumPoints[6] = _float3(1.f, -1.f, 1.f);
	m_vfLocalFrustumPoints[7] = _float3(-1.f, -1.f, 1.f);

	// 순서대로 Near의 좌상단, 우상단, 우하단, 좌하단, Far의 좌상단, 우상단, 좌하단, 우하단의 좌표이다.
}

CFrustum_Manager * CFrustum_Manager::Create(LPDIRECT3DDEVICE9 _pGraphic_Device)
{
	m_pInstance = new CFrustum_Manager(_pGraphic_Device);

	return m_pInstance;
}

void CFrustum_Manager::Tick(void)
{
	// 로컬 프러스텀에서 월드 프러스텀으로의 값을 갱신한다.

	// 로컬 프러스텀에 카메라 투영행렬의 역행렬을 곱하고, 이후 뷰 변환 행렬의 역행렬을 곱한다.
	_float4x4 matrixViewInverse, matrixProjInverse;

	m_pGraphic_Device->GetTransform(D3DTRANSFORMSTATETYPE::D3DTS_VIEW, &matrixViewInverse);
	m_pGraphic_Device->GetTransform(D3DTRANSFORMSTATETYPE::D3DTS_PROJECTION, &matrixProjInverse);

	D3DXMatrixInverse(&matrixViewInverse, nullptr, &matrixViewInverse);
	D3DXMatrixInverse(&matrixProjInverse, nullptr, &matrixProjInverse);

	// 나온값이 월드에서의 프러스텀 좌표이다.
	for (int i = 0; i < 8; ++i) {
		D3DXVec3TransformCoord(&m_vfWorldFrustumPoints[i], &m_vfLocalFrustumPoints[i], &(matrixProjInverse * matrixViewInverse));
	}
}

bool CFrustum_Manager::Check_InOut(const _float4x4& Object_WorldMatrix)
{
	// 인자로 받은 월드 행렬의 중점과 반지름의 길이를 구한다.
	_float3 vPos{};
	memcpy(&vPos, Object_WorldMatrix.m[3], sizeof(_float3));

	// 반지름의 길이는 각 축의 길이중 가장 긴것으로 한다.
	_float fRadius = 0.f;
	_float fX, fY, fZ;
	_float3 vLength;

	memcpy(&vLength, Object_WorldMatrix.m[0], sizeof(_float3));
	fX = D3DXVec3Length(&vLength) * 0.5f;
	memcpy(&vLength, Object_WorldMatrix.m[1], sizeof(_float3));
	fY = D3DXVec3Length(&vLength) * 0.5f;
	memcpy(&vLength, Object_WorldMatrix.m[2], sizeof(_float3));
	fZ = D3DXVec3Length(&vLength) * 0.5f;

	// 셋중 가장 큰 값을 가진 값을 fRadius에 집어 넣는다.
	(fX > fY) ? (fX > fZ ? fRadius = fX : fRadius = fZ) : (fY > fZ ? fRadius = fY : fRadius = fZ);

	// 정점들을 사용하여 평면 6개를 만들어준다.
	// 사용할 점 3개는 각 평면에서 아무 점이나 가져와서 사용한다. 단, 시계방향으로 점을 돌리며 가져온다.

	/* 
	좌 403
	상 104
	우 156 
	하 326 
	앞 012 
	뒤 546
	*/

	D3DXPLANE Plane[6];

	// 평면의 방정식 : ax + by + cz + d = 0
	D3DXPlaneFromPoints(&Plane[0], &m_vfWorldFrustumPoints[4], &m_vfWorldFrustumPoints[0], &m_vfWorldFrustumPoints[3]);
	D3DXPlaneFromPoints(&Plane[1], &m_vfWorldFrustumPoints[1], &m_vfWorldFrustumPoints[0], &m_vfWorldFrustumPoints[4]);
	D3DXPlaneFromPoints(&Plane[2], &m_vfWorldFrustumPoints[1], &m_vfWorldFrustumPoints[5], &m_vfWorldFrustumPoints[6]);
	D3DXPlaneFromPoints(&Plane[3], &m_vfWorldFrustumPoints[3], &m_vfWorldFrustumPoints[2], &m_vfWorldFrustumPoints[6]);
	D3DXPlaneFromPoints(&Plane[4], &m_vfWorldFrustumPoints[0], &m_vfWorldFrustumPoints[1], &m_vfWorldFrustumPoints[2]);
	D3DXPlaneFromPoints(&Plane[5], &m_vfWorldFrustumPoints[5], &m_vfWorldFrustumPoints[4], &m_vfWorldFrustumPoints[6]);

	// 구한 평면의 방정식들에 오브젝트의 월드 좌표를 넣은 값이 반지름의 길이보다 하나라도 작으면 뷰 프러스텀 안에 있다.
	for (int i = 0; i < 6; ++i) {
		if ((Plane[i].a * vPos.x + Plane[i].b * vPos.y + Plane[i].c * vPos.z + Plane[i].d) > fRadius)
			return false;
	}

	// 모든 평면의 방정식에서 오브젝트가 뷰 프러스텀 안에 존재하지 않는다.
	return true;
}

void CFrustum_Manager::Free()
{

}