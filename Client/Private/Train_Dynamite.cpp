#include "stdafx.h"
#include "Train_Dynamite.h"
#include "GameInstance.h"
#include "Train_Head.h"

CTrain_Dynamite::CTrain_Dynamite(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CTrain(pGraphic_Device)
{
	m_eType = OBJECT_TYPE::OBJECT_TRAIN_DYNAMITE;
}

CTrain_Dynamite::CTrain_Dynamite(const CTrain_Dynamite & rhs)
	: CTrain(rhs)
{
	m_eType = rhs.m_eType;
}

HRESULT CTrain_Dynamite::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTrain_Dynamite::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_TrainComponents(pArg)))
		return E_FAIL;

	return S_OK;
}

void CTrain_Dynamite::Tick(_float fTimeDelta)
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

void CTrain_Dynamite::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CTrain_Dynamite::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTrain_Dynamite::SetUp_TrainComponents(void * pArg)
{
	if (FAILED(__super::SetUp_TrainComponents(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_VIBuffer(TEXT("Train_Dynamite.dat"))))
		return E_FAIL;

	return S_OK;
}

CTrain_Dynamite * CTrain_Dynamite::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTrain_Dynamite*	pInstance = new CTrain_Dynamite(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created Train_Dynamite"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTrain_Dynamite::Clone(void * pArg)
{
	CTrain_Dynamite*	pInstance = new CTrain_Dynamite(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created Train_Dynamite"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrain_Dynamite::Free()
{
	__super::Free();
}
