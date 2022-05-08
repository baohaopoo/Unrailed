
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

	// ������� Near�� �»��, ����, ���ϴ�, ���ϴ�, Far�� �»��, ����, ���ϴ�, ���ϴ��� ��ǥ�̴�.
}

CFrustum_Manager * CFrustum_Manager::Create(LPDIRECT3DDEVICE9 _pGraphic_Device)
{
	m_pInstance = new CFrustum_Manager(_pGraphic_Device);

	return m_pInstance;
}

void CFrustum_Manager::Tick(void)
{
	// ���� �������ҿ��� ���� �������������� ���� �����Ѵ�.

	// ���� �������ҿ� ī�޶� ��������� ������� ���ϰ�, ���� �� ��ȯ ����� ������� ���Ѵ�.
	_float4x4 matrixViewInverse, matrixProjInverse;

	m_pGraphic_Device->GetTransform(D3DTRANSFORMSTATETYPE::D3DTS_VIEW, &matrixViewInverse);
	m_pGraphic_Device->GetTransform(D3DTRANSFORMSTATETYPE::D3DTS_PROJECTION, &matrixProjInverse);

	D3DXMatrixInverse(&matrixViewInverse, nullptr, &matrixViewInverse);
	D3DXMatrixInverse(&matrixProjInverse, nullptr, &matrixProjInverse);

	// ���°��� ���忡���� �������� ��ǥ�̴�.
	for (int i = 0; i < 8; ++i) {
		D3DXVec3TransformCoord(&m_vfWorldFrustumPoints[i], &m_vfLocalFrustumPoints[i], &(matrixProjInverse * matrixViewInverse));
	}
}

bool CFrustum_Manager::Check_InOut(const _float4x4& Object_WorldMatrix)
{
	// ���ڷ� ���� ���� ����� ������ �������� ���̸� ���Ѵ�.
	_float3 vPos{};
	memcpy(&vPos, Object_WorldMatrix.m[3], sizeof(_float3));

	// �������� ���̴� �� ���� ������ ���� ������� �Ѵ�.
	_float fRadius = 0.f;
	_float fX, fY, fZ;
	_float3 vLength;

	memcpy(&vLength, Object_WorldMatrix.m[0], sizeof(_float3));
	fX = D3DXVec3Length(&vLength) * 0.5f;
	memcpy(&vLength, Object_WorldMatrix.m[1], sizeof(_float3));
	fY = D3DXVec3Length(&vLength) * 0.5f;
	memcpy(&vLength, Object_WorldMatrix.m[2], sizeof(_float3));
	fZ = D3DXVec3Length(&vLength) * 0.5f;

	// ���� ���� ū ���� ���� ���� fRadius�� ���� �ִ´�.
	(fX > fY) ? (fX > fZ ? fRadius = fX : fRadius = fZ) : (fY > fZ ? fRadius = fY : fRadius = fZ);

	// �������� ����Ͽ� ��� 6���� ������ش�.
	// ����� �� 3���� �� ��鿡�� �ƹ� ���̳� �����ͼ� ����Ѵ�. ��, �ð�������� ���� ������ �����´�.

	/* 
	�� 403
	�� 104
	�� 156 
	�� 326 
	�� 012 
	�� 546
	*/

	D3DXPLANE Plane[6];

	// ����� ������ : ax + by + cz + d = 0
	D3DXPlaneFromPoints(&Plane[0], &m_vfWorldFrustumPoints[4], &m_vfWorldFrustumPoints[0], &m_vfWorldFrustumPoints[3]);
	D3DXPlaneFromPoints(&Plane[1], &m_vfWorldFrustumPoints[1], &m_vfWorldFrustumPoints[0], &m_vfWorldFrustumPoints[4]);
	D3DXPlaneFromPoints(&Plane[2], &m_vfWorldFrustumPoints[1], &m_vfWorldFrustumPoints[5], &m_vfWorldFrustumPoints[6]);
	D3DXPlaneFromPoints(&Plane[3], &m_vfWorldFrustumPoints[3], &m_vfWorldFrustumPoints[2], &m_vfWorldFrustumPoints[6]);
	D3DXPlaneFromPoints(&Plane[4], &m_vfWorldFrustumPoints[0], &m_vfWorldFrustumPoints[1], &m_vfWorldFrustumPoints[2]);
	D3DXPlaneFromPoints(&Plane[5], &m_vfWorldFrustumPoints[5], &m_vfWorldFrustumPoints[4], &m_vfWorldFrustumPoints[6]);

	// ���� ����� �����ĵ鿡 ������Ʈ�� ���� ��ǥ�� ���� ���� �������� ���̺��� �ϳ��� ������ �� �������� �ȿ� �ִ�.
	for (int i = 0; i < 6; ++i) {
		if ((Plane[i].a * vPos.x + Plane[i].b * vPos.y + Plane[i].c * vPos.z + Plane[i].d) > fRadius)
			return false;
	}

	// ��� ����� �����Ŀ��� ������Ʈ�� �� �������� �ȿ� �������� �ʴ´�.
	return true;
}

void CFrustum_Manager::Free()
{

}