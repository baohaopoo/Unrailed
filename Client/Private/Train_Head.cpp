#include "stdafx.h"
#include "Train_Head.h"
#include "GameInstance.h"
#include "Effect_TrainSmoke.h"
#include "Rail_Manager.h"
#include "SoundMgr.h"

CTrain_Head::CTrain_Head(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CTrain(pGraphic_Device)
{
	m_eType = OBJECT_TYPE::OBJECT_TRAIN_HEAD;
}

CTrain_Head::CTrain_Head(const CTrain_Head & rhs)
	: CTrain(rhs)
{
	m_eType = rhs.m_eType;
}

HRESULT CTrain_Head::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))	
		return E_FAIL;

	return S_OK;
}

HRESULT CTrain_Head::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_TrainComponents(pArg)))
		return E_FAIL;

	return S_OK;
}

void CTrain_Head::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fStartTime += fTimeDelta;
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	// 이건 나중에 삭제할 코드, 움직이는지 확인하는 용도 *****
	// 아직 움직이지 않을 때
	if (!m_bMove)
	{
		// 현재 맵에 존재하는 레일의 정보를 들고온다.
		vector<CGameObject*>& RailInfo = CRail_Manager::GetInstance()->Get_vRailInfo();
		if (m_fStartTime > 10.f && m_iStart == 0)
		{
			CSoundMgr::Get_Instance()->PlaySound(TEXT("StartSound.mp3"), CSoundMgr::CHANNELID::TRAINSTART, 0.7f);
			m_iStart = 1;
		}

		if (m_fStartTime > 18.f)
		{
			StartMove();
		}
	}

	// 움직임이 시작됬을 경우에
	if (m_bMove)
	{
		// 레일을 따라가게한다.
		Follow_Rail(fTimeDelta);
		CSoundMgr::Get_Instance()->PlaySound(TEXT("TrainSound.mp3"), CSoundMgr::CHANNELID::TRAINGO, 0.4f);
	}

	// 기차 위에 연기를 나게한디.
	Create_Smoke(fTimeDelta);

	vector<CGameObject*>& RailInfo = CRail_Manager::GetInstance()->Get_vRailInfo();

	if ((RailInfo.size() - 4) <= m_iRailNumber)
	{

		if ((RailInfo.size() - 1) <= m_iRailNumber)
		{
			m_bMove = false;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::TRAINWARNING);
		}
		else
		{
			CSoundMgr::Get_Instance()->PlaySound(TEXT("TrainWarningSound_Short.mp3"), CSoundMgr::CHANNELID::TRAINWARNING, 0.5f);
		}

	}

}

void CTrain_Head::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CTrain_Head::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTrain_Head::SetUp_TrainComponents(void * pArg)
{
	if (FAILED(__super::SetUp_TrainComponents(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_VIBuffer(TEXT("Train_Head2.dat"))))
		return E_FAIL;

	return S_OK;
}

CTrain_Head * CTrain_Head::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTrain_Head*	pInstance = new CTrain_Head(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created Train_Head"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTrain_Head::Clone(void * pArg)
{
	CTrain_Head*	pInstance = new CTrain_Head(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created Train_Head"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrain_Head::Free()
{
	__super::Free();
}

void CTrain_Head::Create_Smoke(_float _fDeltaTime)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	fEffectTime += _fDeltaTime;

	if (fEffectTime>0.1f)
	{
		CEffect_TrainSmoke::TRAINSMOKEDESC TrainSmokeDesc;
		TrainSmokeDesc.iTextureNumber = 37;
		TrainSmokeDesc.eSmoke = CEffect_TrainSmoke::ST_TRAIN;


		_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		D3DXVec3Normalize(&vLook, &vLook);

		vPos += (vLook * 0.6f);

		vPos.y += 0.6f;

		memcpy(TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3], &vPos, sizeof(_float3));

		pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Effect_Smoke"), TEXT("Prototype_GameObject_Effect_TrainSmoke"), &TrainSmokeDesc.TransDesc);
		fEffectTime = 0.f;
	}
}
