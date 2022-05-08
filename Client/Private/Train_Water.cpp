#include "stdafx.h"
#include "Train_Water.h"
#include "GameInstance.h"
#include "Effect_Explo.h"
#include "Train_Head.h"
#include "ToolUI.h"
#include "SoundMgr.h"
#include "Effect_TrainSmoke.h"
CTrain_Water::CTrain_Water(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CTrain(pGraphic_Device)
{
	m_eType = OBJECT_TYPE::OBJECT_TRAIN_WATER;
}

CTrain_Water::CTrain_Water(const CTrain_Water & rhs)
	: CTrain(rhs)
{
	m_eType = rhs.m_eType;
}

HRESULT CTrain_Water::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTrain_Water::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_TrainComponents(pArg)))
		return E_FAIL;

	return S_OK;
}

void CTrain_Water::Tick(_float fTimeDelta)
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

	__super::Tick(fTimeDelta);
	//CGameInstance* pGameInstance = CGameInstance::GetInstance();




	time += fTimeDelta;

	if (!isFire) {
		FireOn();
	}

}

void CTrain_Water::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	
	//isFire = false;
}

HRESULT CTrain_Water::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CTrain_Water::FireGone(_float fTimeDelta)
{
	isFire = true;

	if (FAILED(SetUp_VIBuffer(TEXT("Train_Water.dat"))))
		return;
	
	//time += fTimeDelta;
	//if (time > 14) {
	//	isFire = false;
	//	time = 0.f;
	//}

	
	//isFire = false;

}

void CTrain_Water::FireOn()
{
	//기차가 배치되고 대락 3초뒤.
	CGameInstance* pGameInstance = CGameInstance::GetInstance();


	CLayer* pSmokeLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Effect_Smoke_Dynamite"));

	if (nullptr != pSmokeLayer) {
		list<CGameObject*>& ObjectList = pSmokeLayer->Get_ObjectList();

		if (!ObjectList.empty()) {
			for (auto& Object : ObjectList) {
				CEffect_TrainSmoke* pSmokeColor = dynamic_cast<CEffect_TrainSmoke*>(Object);

				if (pSmokeColor->Get_Time() > 0.8f) {
					pSmokeColor->ChangeColor(51);
					if (pSmokeColor->Get_Time() > 1.4f)
					{
						pSmokeColor->ChangeColor(52);
					}
				}
			}
		}
	}






	if (time > 30) {
		if (FAILED(SetUp_VIBuffer(TEXT("Train_Water2.dat"))))
			return;
	}
	if (time > 35) {
		if (FAILED(SetUp_VIBuffer(TEXT("Train_Water3.dat"))))
			return;
	}

	if (time > 40)
	{
		if (FAILED(SetUp_VIBuffer(TEXT("Train_Water4.dat"))))
			return;

		//TOOLUI
		_float3 pColPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);


		ToolUI::TOOLDES tooldesc;


		tooldesc.tradesc.m_WorldMatrix.m[3][0] = pColPosition.x;
		tooldesc.tradesc.m_WorldMatrix.m[3][1] = pColPosition.y + 2;
		tooldesc.tradesc.m_WorldMatrix.m[3][2] = pColPosition.z;

		tooldesc.tradesc.m_WorldMatrix.m[0][0] = 0.5f;
		tooldesc.tradesc.m_WorldMatrix.m[1][1] = 0.5f;
		tooldesc.tradesc.m_WorldMatrix.m[2][2] = 0.5f;



	/*	tooldesc.tk = ToolUI::TOOLKIND::WARNING;

		pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_MaterialTrainUI"), TEXT("Prototype_GameObject_ToolUI"), &tooldesc);
*/

		CEffect_TrainSmoke::TRAINSMOKEDESC   TrainSmokeDesc;
		TrainSmokeDesc.eSmoke = CEffect_TrainSmoke::ST_DYNAMITE;
		TrainSmokeDesc.iTextureNumber = 38;
		TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][0] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).x;
		TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][1] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y + 0.25f;
		TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][2] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).z;

		pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Effect_Smoke_Dynamite"), TEXT("Prototype_GameObject_Effect_TrainSmoke"), &TrainSmokeDesc.TransDesc);
	
		CSoundMgr::Get_Instance()->PlaySound(TEXT("Burning.mp3"), CSoundMgr::CHANNELID::TRAINFIRE, SOUND_DEFAULT);
	}

	if (time > 50)
	{
		time = 0.f;
	}
}

HRESULT CTrain_Water::SetUp_TrainComponents(void * pArg)
{
	if (FAILED(__super::SetUp_TrainComponents(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_VIBuffer(TEXT("Train_Water.dat"))))
		return E_FAIL;

	return S_OK;
}

CTrain_Water * CTrain_Water::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTrain_Water*	pInstance = new CTrain_Water(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created Train_Water"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTrain_Water::Clone(void * pArg)
{
	CTrain_Water*	pInstance = new CTrain_Water(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created Train_Water"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrain_Water::Free()
{
	__super::Free();
}
 