
#include "..\Public\Collision_Manager.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "Layer.h"

IMPLEMENT_SINGLETON(CCollision_Manager)

CCollision_Manager::CCollision_Manager()
{
}

CCollision_Manager::CCollision_Manager(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: m_pGraphic_Device(_pGraphic_Device)
{
}

CCollision_Manager * CCollision_Manager::Create(LPDIRECT3DDEVICE9 _pGraphic_Device)
{
	m_pInstance = new CCollision_Manager(_pGraphic_Device);

	return m_pInstance;
}

HRESULT CCollision_Manager::AddCollisionLayers(_uint _iNumLevel, const TCHAR * _pLayerTag1, const TCHAR * _pLayerTag2)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CLayer* pLayer1 = pGameInstance->Find_Layer(_iNumLevel, _pLayerTag1);
	CLayer* pLayer2 = pGameInstance->Find_Layer(_iNumLevel, _pLayerTag2);

	if (nullptr == pLayer1 || nullptr == pLayer2)
		return E_FAIL;

	m_vCollision.push_back(make_pair(pLayer1, pLayer2));

	return S_OK;
}

bool CCollision_Manager::CheckIntersect(_float3 & center1, float & radius1, _float3 & center2, float & radius2)
{
	return D3DXVec3Length(&(center1 - center2)) <= (radius1 + radius2);
}

void CCollision_Manager::Tick(void)
{
	if (m_vCollision.empty())
		return;

	// �켱 �浹 ó���� �ϰ�, ó���� ������Ʈ�� �׸� ������Ʈ ����Ʈ�� �ִ´�.
	for (auto& layers : m_vCollision) {

		// ���̾���� ������ �ִ� ������Ʈ ����Ʈ�� �����´�.
		const list<CGameObject*>& Layer1_ObjectList = layers.first->Get_ObjectList();
		const list<CGameObject*>& Layer2_ObjectList = layers.second->Get_ObjectList();

		// ������Ʈ ����Ʈ�� �ִ� ������Ʈ ���� �浹 ������ �Ѵ�.
		for (auto& Layer1_Object : Layer1_ObjectList) {
			for (auto& Layer2_Object : Layer2_ObjectList) {

				CCollider* Collider_Component_Object_Layer1 = dynamic_cast<CCollider*>(Layer1_Object->Get_Component(TEXT("Com_Collider")));
				CCollider* Collider_Component_Object_Layer2 = dynamic_cast<CCollider*>(Layer2_Object->Get_Component(TEXT("Com_Collider")));

				if (nullptr == Collider_Component_Object_Layer1 || nullptr == Collider_Component_Object_Layer2)
					continue;
				else {
					// aabb ������ �浹 ó���� �Ѵ�.
					const _float4x4& worldMatrixObject1 = Collider_Component_Object_Layer1->Get_WorldMatrix();
					const _float4x4& worldMatrixObject2 = Collider_Component_Object_Layer2->Get_WorldMatrix();
					const _float4x4& localMatrixObject1 = Collider_Component_Object_Layer1->Get_LocalMatrix();
					const _float4x4& localMatrixObject2 = Collider_Component_Object_Layer2->Get_LocalMatrix();

					// �� ������Ʈ�� ������ ���� ��ġ�� ������ ��� ������ ���� ���� ��ġ�� ��������� ����� ���̸� ���Ѵ�.
					_float3 vPosObject1{}, vPosObject2;
					memcpy(&vPosObject1, localMatrixObject1.m[3], sizeof(_float3));
					memcpy(&vPosObject2, localMatrixObject2.m[3], sizeof(_float3));

					// ���忡���� ���� ��ġ�� �����.
					D3DXVec3TransformCoord(&vPosObject1, &vPosObject1, &worldMatrixObject1);
					D3DXVec3TransformCoord(&vPosObject2, &vPosObject2, &worldMatrixObject2);

					// �� ���� ������ ������ ������ ���Ѵ�.
					_float abs_fDistX, abs_fDistZ;
					abs_fDistX = abs(vPosObject1.x - vPosObject2.x);
					abs_fDistZ = abs(vPosObject1.z - vPosObject2.z);

					_float fLocalX1, fLocalZ1;
					_float fLocalX2, fLocalZ2;

					_float3 fLocalX, fLocalZ;
					memcpy(&fLocalX, Collider_Component_Object_Layer1->Get_ColDesc().localMatrix.m[0], sizeof(_float3));
					memcpy(&fLocalZ, Collider_Component_Object_Layer1->Get_ColDesc().localMatrix.m[2], sizeof(_float3));

					if (D3DXVec3Length(&fLocalX) <= 1.5f)
						fLocalX1 = (D3DXVec3Length(&fLocalX) - 1) * 2.f + 1;
					else
						fLocalX1 = (D3DXVec3Length(&fLocalX) - 1) * 4.f;

					if (D3DXVec3Length(&fLocalZ) <= 1.5f)
						fLocalZ1 = (D3DXVec3Length(&fLocalZ) - 1) * 2.f + 1;
					else
						fLocalZ1 = (D3DXVec3Length(&fLocalZ) - 1) * 4.f;



					memcpy(&fLocalX, Collider_Component_Object_Layer2->Get_ColDesc().localMatrix.m[0], sizeof(_float3));
					memcpy(&fLocalZ, Collider_Component_Object_Layer2->Get_ColDesc().localMatrix.m[2], sizeof(_float3));
					if (D3DXVec3Length(&fLocalX) <= 1.5f)
						fLocalX2 = (D3DXVec3Length(&fLocalX) - 1) * 2.f + 1;
					else
						fLocalX2 = (D3DXVec3Length(&fLocalX) - 1) * 4.f;

					if (D3DXVec3Length(&fLocalZ) <= 1.5f)
						fLocalZ2 = (D3DXVec3Length(&fLocalZ) - 1) * 2.f + 1;
					else
						fLocalZ2 = (D3DXVec3Length(&fLocalZ) - 1) * 4.f;


					// �� ���� ���� ������ ���̺��� ��������� ����� ���̸� ���� ���� ũ�ٸ�, �� �����δ� ��ģ�ٴ� �Ҹ���.
					// ��� ���� ���� ��ģ�ٸ� �ش� ������Ʈ�� ���� ���ƴٴ� �Ҹ���.
					if (
						(abs_fDistX < ((fLocalX1 + fLocalX2) * 0.5f)) &&
						(abs_fDistZ < ((fLocalZ1 + fLocalZ2) * 0.5f))
						)
					{
						// �浹�� �Ͼ�ٸ�, ������ �浹 üũ ������Ʈ ����Ʈ�� �浹�� ������Ʈ ������ �ִ´�.
						Collider_Component_Object_Layer1->Push_ColObject(Layer2_Object);
						Collider_Component_Object_Layer2->Push_ColObject(Layer1_Object);
					}
					else
						// �浹�� �Ͼ�� �ʾҴٸ�
						;
				}

				// �浹�˻縦 �ߴٸ�, �׷����� ������Ʈ ����Ʈ�� �ִ´�.
				m_CollisionObject.emplace(Layer2_Object);
			}
			// �浹�˻縦 �ߴٸ�, �׷����� ������Ʈ ����Ʈ�� �ִ´�.
			m_CollisionObject.emplace(Layer1_Object);
		}
	}
}

void CCollision_Manager::LateTick(void)
{
	if (m_vCollision.empty())
		return;

	// ������ �ִ� ������ �����Ѵ�.
	for (auto& layers : m_vCollision) {

		// ���̾���� ������ �ִ� ������Ʈ ����Ʈ�� �����´�.
		const list<CGameObject*>& Layer1_ObjectList = layers.first->Get_ObjectList();
		const list<CGameObject*>& Layer2_ObjectList = layers.second->Get_ObjectList();

		// ������Ʈ ����Ʈ�� �ִ� ������Ʈ���� ������ �ִ� �浹 ���� ������ �����Ѵ�.
		for (auto& Layer1_Object : Layer1_ObjectList) {
			CCollider* Collider_Component_Object_Layer1 = dynamic_cast<CCollider*>(Layer1_Object->Get_Component(TEXT("Com_Collider")));

			if (nullptr == Collider_Component_Object_Layer1)
				continue;

			Collider_Component_Object_Layer1->Get_ColObjects().clear();
		}
		for (auto& Layer2_Object : Layer2_ObjectList) {
			CCollider* Collider_Component_Object_Layer2 = dynamic_cast<CCollider*>(Layer2_Object->Get_Component(TEXT("Com_Collider")));

			if (nullptr == Collider_Component_Object_Layer2)
				continue;

			Collider_Component_Object_Layer2->Get_ColObjects().clear();
		}
	}
}

void CCollision_Manager::Render(void)
{
	if (m_CollisionObject.empty())
		return;

	// �浹ü ������Ʈ�� ���̾� ������ ���� �׸���.

	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	for (auto& Object : m_CollisionObject) {
		CCollider* Collider_Component_Object = dynamic_cast<CCollider*>(Object->Get_Component(TEXT("Com_Collider")));

		if (nullptr != Collider_Component_Object)
			Collider_Component_Object->Render();


		// �÷��̾� ����, ���߿� ���� ����
		Collider_Component_Object = dynamic_cast<CCollider*>(Object->Get_Component(TEXT("Com_Collider_Block")));

		if (nullptr != Collider_Component_Object)
			Collider_Component_Object->Render();
	}

	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	// �ݸ��� ������Ʈ�� ������ �ִ� �浹ó���� �Ǵ� ������Ʈ���� ������ ���� �����Ѵ�.
	m_CollisionObject.clear();
}

void CCollision_Manager::Free()
{

}