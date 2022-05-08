
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

	// 우선 충돌 처리를 하고, 처리된 오브젝트를 그릴 오브젝트 리스트에 넣는다.
	for (auto& layers : m_vCollision) {

		// 레이어들이 가지고 있는 오브젝트 리스트를 가져온다.
		const list<CGameObject*>& Layer1_ObjectList = layers.first->Get_ObjectList();
		const list<CGameObject*>& Layer2_ObjectList = layers.second->Get_ObjectList();

		// 오브젝트 리스트에 있는 오브젝트 끼리 충돌 연산을 한다.
		for (auto& Layer1_Object : Layer1_ObjectList) {
			for (auto& Layer2_Object : Layer2_ObjectList) {

				CCollider* Collider_Component_Object_Layer1 = dynamic_cast<CCollider*>(Layer1_Object->Get_Component(TEXT("Com_Collider")));
				CCollider* Collider_Component_Object_Layer2 = dynamic_cast<CCollider*>(Layer2_Object->Get_Component(TEXT("Com_Collider")));

				if (nullptr == Collider_Component_Object_Layer1 || nullptr == Collider_Component_Object_Layer2)
					continue;
				else {
					// aabb 식으로 충돌 처리를 한다.
					const _float4x4& worldMatrixObject1 = Collider_Component_Object_Layer1->Get_WorldMatrix();
					const _float4x4& worldMatrixObject2 = Collider_Component_Object_Layer2->Get_WorldMatrix();
					const _float4x4& localMatrixObject1 = Collider_Component_Object_Layer1->Get_LocalMatrix();
					const _float4x4& localMatrixObject2 = Collider_Component_Object_Layer2->Get_LocalMatrix();

					// 두 오브젝트의 로컬의 중점 위치에 월드의 행렬 정보를 곱해 얻은 위치와 월드행렬의 라업룩 길이를 구한다.
					_float3 vPosObject1{}, vPosObject2;
					memcpy(&vPosObject1, localMatrixObject1.m[3], sizeof(_float3));
					memcpy(&vPosObject2, localMatrixObject2.m[3], sizeof(_float3));

					// 월드에서의 중점 위치를 얻었다.
					D3DXVec3TransformCoord(&vPosObject1, &vPosObject1, &worldMatrixObject1);
					D3DXVec3TransformCoord(&vPosObject2, &vPosObject2, &worldMatrixObject2);

					// 각 중점 사이의 차이의 절댓값을 구한다.
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


					// 각 축의 중점 사이의 길이보다 월드행렬의 라업룩 길이를 더한 값이 크다면, 그 축으로는 겹친다는 소리다.
					// 모든 축이 전부 겹친다면 해당 오브젝트는 서로 겹쳤다는 소리다.
					if (
						(abs_fDistX < ((fLocalX1 + fLocalX2) * 0.5f)) &&
						(abs_fDistZ < ((fLocalZ1 + fLocalZ2) * 0.5f))
						)
					{
						// 충돌이 일어났다면, 서로의 충돌 체크 오브젝트 리스트에 충돌된 오브젝트 정보를 넣는다.
						Collider_Component_Object_Layer1->Push_ColObject(Layer2_Object);
						Collider_Component_Object_Layer2->Push_ColObject(Layer1_Object);
					}
					else
						// 충돌이 일어나지 않았다면
						;
				}

				// 충돌검사를 했다면, 그려지는 오브젝트 리스트에 넣는다.
				m_CollisionObject.emplace(Layer2_Object);
			}
			// 충돌검사를 했다면, 그려지는 오브젝트 리스트에 넣는다.
			m_CollisionObject.emplace(Layer1_Object);
		}
	}
}

void CCollision_Manager::LateTick(void)
{
	if (m_vCollision.empty())
		return;

	// 기존에 있던 정보를 리셋한다.
	for (auto& layers : m_vCollision) {

		// 레이어들이 가지고 있는 오브젝트 리스트를 가져온다.
		const list<CGameObject*>& Layer1_ObjectList = layers.first->Get_ObjectList();
		const list<CGameObject*>& Layer2_ObjectList = layers.second->Get_ObjectList();

		// 오브젝트 리스트에 있는 오브젝트들이 가지고 있는 충돌 관련 정보를 리셋한다.
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

	// 충돌체 오브젝트를 와이어 프레임 모드로 그린다.

	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	for (auto& Object : m_CollisionObject) {
		CCollider* Collider_Component_Object = dynamic_cast<CCollider*>(Object->Get_Component(TEXT("Com_Collider")));

		if (nullptr != Collider_Component_Object)
			Collider_Component_Object->Render();


		// 플레이어 전용, 나중에 수정 예정
		Collider_Component_Object = dynamic_cast<CCollider*>(Object->Get_Component(TEXT("Com_Collider_Block")));

		if (nullptr != Collider_Component_Object)
			Collider_Component_Object->Render();
	}

	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	// 콜리전 오브젝트가 가지고 있는 충돌처리가 되는 오브젝트들의 정보를 전부 리셋한다.
	m_CollisionObject.clear();
}

void CCollision_Manager::Free()
{

}