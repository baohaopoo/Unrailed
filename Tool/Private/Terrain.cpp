#include "stdafx.h"
#include "Terrain.h"
#include "GameInstance.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CTerrain::CTerrain(const CTerrain & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTerrain::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))	
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CTerrain::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	// 가지고 있는 내부 오브젝트의 Tuck/LateTick은 불릴 필요가 없다.

	Safe_Release(pGameInstance);

}

void CTerrain::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	//for (auto& Object : m_vObjects) {
	//	dynamic_cast<CRenderer*>(Object->Get_Component(Component_Renderer))
	//		->Add_RenderGroup(CRenderer::GROUP_NONBLEND, Object);
	//}
}

HRESULT CTerrain::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	//for (auto& Object : m_vObjects) {
	//	Object->Render();
	//}

	return S_OK;
}

HRESULT CTerrain::SetUp_Components(void* pArg)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CTerrain::MAPDESC	MapDesc;

	if (nullptr == pArg)
		return E_FAIL;

	memcpy(&MapDesc, pArg, sizeof(CTerrain::MAPDESC));

	if (MapDesc.iMapSizeX < 1 || MapDesc.iMapSizeY < 1)
		return E_FAIL;

	// 01. 인자로 받은 가로, 세로 길이를 가진 큐브들로 이루어진 맵을 터레인 레이어에 로드한다.

	// 02. 맵을 피킹하여 오브젝트를 맵 레이어에서 생성할 수 있도록한다. 생성된 오브젝트 또한 피킹이 가능하고, 그 위에 오브젝트 생성이 가능하다.


	// 맵의 큰 큐브 베이스 추가 -> 이건 제외
	//CTransform::TRANSFORMDESC TransDesc;
	//_float fY = 2.f;
	//D3DXMatrixScaling(&TransDesc.m_LocalScale, MapDesc.iMapSizeX, fY, MapDesc.iMapSizeY);
	//D3DXMatrixTranslation(&TransDesc.m_LocalTransform, 0.f, -fY + 0.99f, 0.f);
	//pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Terrain, Prototype_Object_CubeObject, &TransDesc);
	//
	//POINT pt{};
	//pt.x = MapDesc.iMapSizeX + 1;
	//pt.y = MapDesc.iMapSizeY + 1;
	//pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Terrain, Prototype_Object_PickingObject, &pt);



	// 인자로 받은 가로, 세로 길이를 가진 큐브들로 이루어진 맵을 터레인 레이어에 로드한다.
	for (int i = 0; i < MapDesc.iMapSizeY; ++i) {
		for (int j = 0; j < MapDesc.iMapSizeX; ++j) {
			CTransform::TRANSFORMDESC TransDesc;
			// 맵에서 로드할 경우 우선 큐브의 텍스쳐는 0번 텍스쳐로 고정한다.
			TransDesc.m_iTextureNumber = 0;
			
			// 월드에서 큐브의 길이는 1이다.
			_float3 vPos{};
			vPos.x = -0.5 * (MapDesc.iMapSizeX - 1) + j;
			vPos.y = 0.f;
			vPos.z = -0.5 * (MapDesc.iMapSizeY - 1) + i;

			memcpy(&TransDesc.m_WorldMatrix.m[3][0], &vPos, sizeof(_float3));

			pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Terrain, Prototype_Object_CubeObject, &TransDesc);
		}
	}

	return S_OK;
}

CTerrain * CTerrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTerrain*	pInstance = new CTerrain(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTerrain::Clone(void * pArg)
{
	CTerrain*	pInstance = new CTerrain(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}