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

	// ������ �ִ� ���� ������Ʈ�� Tuck/LateTick�� �Ҹ� �ʿ䰡 ����.

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

	// 01. ���ڷ� ���� ����, ���� ���̸� ���� ť���� �̷���� ���� �ͷ��� ���̾ �ε��Ѵ�.

	// 02. ���� ��ŷ�Ͽ� ������Ʈ�� �� ���̾�� ������ �� �ֵ����Ѵ�. ������ ������Ʈ ���� ��ŷ�� �����ϰ�, �� ���� ������Ʈ ������ �����ϴ�.


	// ���� ū ť�� ���̽� �߰� -> �̰� ����
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



	// ���ڷ� ���� ����, ���� ���̸� ���� ť���� �̷���� ���� �ͷ��� ���̾ �ε��Ѵ�.
	for (int i = 0; i < MapDesc.iMapSizeY; ++i) {
		for (int j = 0; j < MapDesc.iMapSizeX; ++j) {
			CTransform::TRANSFORMDESC TransDesc;
			// �ʿ��� �ε��� ��� �켱 ť���� �ؽ��Ĵ� 0�� �ؽ��ķ� �����Ѵ�.
			TransDesc.m_iTextureNumber = 0;
			
			// ���忡�� ť���� ���̴� 1�̴�.
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