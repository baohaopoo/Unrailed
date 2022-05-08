#include "stdafx.h"
#include "Train_Rail.h"
#include "GameInstance.h"
#include "Train_Head.h"

CTrain_Rail::CTrain_Rail(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CTrain(pGraphic_Device)
{
	m_eType = OBJECT_TYPE::OBJECT_TRAIN_RAIL;
}

CTrain_Rail::CTrain_Rail(const CTrain_Rail & rhs)
	: CTrain(rhs)
{
	m_eType = rhs.m_eType;
}

HRESULT CTrain_Rail::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))	
		return E_FAIL;

	return S_OK;
}

HRESULT CTrain_Rail::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_TrainComponents(pArg)))
		return E_FAIL;

	return S_OK;
}

void CTrain_Rail::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	
	
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	if (!m_bMove) {
		CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Train"));

		if (nullptr != pLayer) {
			list<CGameObject*> ObjectList = pLayer->Get_ObjectList();
			for (auto& Object : ObjectList) {
				if (Object->Get_Type() == OBJECT_TYPE::OBJECT_TRAIN_HEAD) {
					m_bMove = dynamic_cast<CTrain_Head*>(Object)->bMove();
				}
			}
		}
	}

	if (m_bMove) {
		Follow_Rail(fTimeDelta);
	}
}

void CTrain_Rail::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CTrain_Rail::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CTrain_Rail::TrainState(bool onrail)
{

	if (onrail)
	{
		if (FAILED(SetUp_VIBuffer(TEXT("railonrail.dat"))))
			return;
	}
	else {
		if (FAILED(SetUp_VIBuffer(TEXT("Train_Rail.dat"))))
			return;
	}

}

HRESULT CTrain_Rail::SetUp_TrainComponents(void * pArg)
{
	if (FAILED(__super::SetUp_TrainComponents(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_VIBuffer(TEXT("Train_Rail.dat"))))
		return E_FAIL;

	return S_OK;
}

void CTrain_Rail::GetMaterial(int num)
{
	materialNum += num;
	if (materialNum == 2) {
		if (FAILED(Ready_Layer_Rail(TEXT("Layer_Rail"))))
			return;
	}
}

HRESULT CTrain_Rail::Ready_Layer_Rail(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	CTransform::TRANSFORMDESC tradesc;

	tradesc.m_WorldMatrix.m[3][0] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).x;
	tradesc.m_WorldMatrix.m[3][1] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y+0.5f;
	tradesc.m_WorldMatrix.m[3][2] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).z;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Rail_Straight"), &tradesc)))
		return E_FAIL;

	return S_OK;
}

CTrain_Rail * CTrain_Rail::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTrain_Rail*	pInstance = new CTrain_Rail(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created Train_Rail"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTrain_Rail::Clone(void * pArg)
{
	CTrain_Rail*	pInstance = new CTrain_Rail(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created Train_Rail"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrain_Rail::Free()
{
	__super::Free();
}
