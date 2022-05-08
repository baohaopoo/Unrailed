#include "stdafx.h"
#include "Train_Material.h"
#include "GameInstance.h"
#include "SoundMgr.h"
#include "Train_Head.h"

CTrain_Material::CTrain_Material(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CTrain(pGraphic_Device)
{
	m_eType = OBJECT_TYPE::OBJECT_TRAIN_MATERIAL;
}

CTrain_Material::CTrain_Material(const CTrain_Material & rhs)
	: CTrain(rhs)
{
	m_eType = rhs.m_eType;
}

HRESULT CTrain_Material::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))	
		return E_FAIL;

	return S_OK;
}

HRESULT CTrain_Material::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_TrainComponents(pArg)))
		return E_FAIL;

	return S_OK;
}

void CTrain_Material::Tick(_float fTimeDelta)
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

void CTrain_Material::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CTrain_Material::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTrain_Material::SetUp_TrainComponents(void * pArg)
{
	if (FAILED(__super::SetUp_TrainComponents(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_VIBuffer(TEXT("Train_Material.dat"))))
		return E_FAIL;

	return S_OK;
}

void CTrain_Material::GetMaterial()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	
	pGameInstance->Delete_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Iron"), *(m_Ironvec.begin()));
	pGameInstance->Delete_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Wood"), *(m_Woodvec.begin()));
	m_Ironvec.pop_back();
	m_Woodvec.pop_back();
}

void CTrain_Material::PushMaterial(string name,CGameObject* pObject)
{
	if (name == string("Wood")) {
		m_Woodvec.push_back(pObject);
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PUSHMATERIAL);
		CSoundMgr::Get_Instance()->PlaySound(TEXT("railrail.mp3"), CSoundMgr::CHANNELID::PUSHMATERIAL, SOUND_MAX);
	}
	if (name == string("Iron")) {
		m_Ironvec.push_back(pObject);
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PUSHMATERIAL);
		CSoundMgr::Get_Instance()->PlaySound(TEXT("railrail.mp3"), CSoundMgr::CHANNELID::PUSHMATERIAL, SOUND_MAX);
	}

	if (m_Ironvec.size() > 0 && m_Woodvec.size() > 0 )
	{
		GetMaterial();
	}
}

void CTrain_Material::TrainState(bool wood, bool iron)
{
	if (true == wood) {
		if (FAILED(SetUp_VIBuffer(TEXT("Train_Material[Wood].dat"))))
			return;

	}
	else if (true == iron) {
		if (FAILED(SetUp_VIBuffer(TEXT("Train_Material[Iron].dat"))))
			return;
	}
	else  if((true == iron) && (true == wood))
	{
		if (FAILED(SetUp_VIBuffer(TEXT("both.dat"))))
			return;

	}
	else {
		if (FAILED(SetUp_VIBuffer(TEXT("Train_Material.dat"))))
			return;
	}

}

void CTrain_Material::both()
{
	if (FAILED(SetUp_VIBuffer(TEXT("both.dat"))))
		return;
}

CTrain_Material * CTrain_Material::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTrain_Material*	pInstance = new CTrain_Material(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created Train_Material"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTrain_Material::Clone(void * pArg)
{
	CTrain_Material*	pInstance = new CTrain_Material(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created Train_Material"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrain_Material::Free()
{
	__super::Free();

}
