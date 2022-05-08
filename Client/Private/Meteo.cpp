#include "stdafx.h"
#include "Meteo.h"
#include "GameInstance.h"
#include "Player.h"
#include "Effect_TrainSmoke.h"
#include "Effect_Explo.h"
#include "SoundMgr.h"
#include "Unbreakable_Rock.h"

CMeteo::CMeteo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
	m_eType = OBJECT_TYPE::OBJECT_METEO;
}

CMeteo::CMeteo(const CMeteo & rhs)
	: CGameObject(rhs)
{

	m_eType = rhs.m_eType;
}




HRESULT CMeteo::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMeteo::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CMeteo::Tick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
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
	fEffectTime += fTimeDelta;
	fTime += fTimeDelta;



	Boom();
	if (fEffectTime > 0.1f)
	{
		CEffect_TrainSmoke::TRAINSMOKEDESC TrainSmokeDesc;

		TrainSmokeDesc.eSmoke = CEffect_TrainSmoke::ST_METEO;
		TrainSmokeDesc.iTextureNumber = 38;

		TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][0] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).x;
		TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][1] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
		TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][2] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).z;

		pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Effect_Smoke_Dynamite"), TEXT("Prototype_GameObject_Effect_TrainSmoke"), &TrainSmokeDesc.TransDesc);

		fEffectTime = 0.f;
	}


	fPos -= *D3DXVec3Normalize(&vUp, &vUp) * fTimeDelta * 2.f;

	m_pTransformCom->Turn(_float3(0.f, 1.f, 1.f), fTimeDelta * 2.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, fPos);

	__super::Tick(fTimeDelta);

}

void CMeteo::LateTick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	fTime += fTimeDelta;

	int iRandNum = rand() % 50;

	if (fPos.y <= 4.f)
	{
		if (iRandNum == 0)
			CSoundMgr::Get_Instance()->PlaySound(TEXT("explo.mp3"), CSoundMgr::CHANNELID::EXPLO, 0.9f);

		else if (iRandNum == 1)
			CSoundMgr::Get_Instance()->PlaySound(TEXT("explo.mp3"), CSoundMgr::CHANNELID::EXPLO1, 0.9f);

		else if (iRandNum == 2)
			CSoundMgr::Get_Instance()->PlaySound(TEXT("explo.mp3"), CSoundMgr::CHANNELID::EXPLO2, 0.9f);

		else if (iRandNum == 3)
			CSoundMgr::Get_Instance()->PlaySound(TEXT("explo.mp3"), CSoundMgr::CHANNELID::EXPLO3, 0.9f);

		else if (iRandNum == 4)
			CSoundMgr::Get_Instance()->PlaySound(TEXT("explo.mp3"), CSoundMgr::CHANNELID::EXPLO4, 0.9f);
	}

	if (fPos.y <= 1.f)
	{
		CEffect_Explo::EXPLODESC	ExploDesc;
		ExploDesc.eExplo = CEffect_Explo::ET_METEO;
		ExploDesc.iTextureNumber = 43;
		ExploDesc.TransDesc.m_WorldMatrix.m[3][0] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).x;
		ExploDesc.TransDesc.m_WorldMatrix.m[3][1] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
		ExploDesc.TransDesc.m_WorldMatrix.m[3][2] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).z;
		for (_uint i = 0; i < 20; i++)
		{
			pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Effect_Explo"), TEXT("Prototype_GameObject_Effect_Explo"), &ExploDesc.TransDesc);
		}
		m_bDead = true;
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CMeteo::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	auto& ObjectList = m_pVIBufferCom->Get_lstVIBuffer();

	for (auto& Object : ObjectList) {

		if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(Object.second)))
			return E_FAIL;

		Object.first->Render();
	}

	return S_OK;
}

void CMeteo::Boom()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	CLayer* pLayerMap = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, Layer_Map);
	list<CGameObject*>& ObjectList = pLayerMap->Get_ObjectList();

	if (!ObjectList.empty())
	{
		for (auto& Object : ObjectList)
		{
			// 오브젝트 타입이 나무와 관련된 타입일 경우
			/////////////////////////////// 나무! //////////////////////////////
			if (Object->Get_Type() == OBJECT_TYPE::OBJECT_UNBREAKABLE_ROCK1 ||
				Object->Get_Type() == OBJECT_TYPE::OBJECT_UNBREAKABLE_ROCK2 ||
				Object->Get_Type() == OBJECT_TYPE::OBJECT_UNBREAKABLE_ROCK3
				)
			{
				//////////////////////////////////////////////////////////////////
				// 해당 오브젝트의 위치를 가져온다.
				_float3 vResourcePos = dynamic_cast<CTransform*>(Object->Get_Component(Component_Transform))->Get_State(CTransform::STATE_POSITION);

				// 다이너마이트와 가져온 오브젝트의 위치 사이의 값이 4.f이하인 경우 
				if (pGameInstance->CheckIntersect(vResourcePos, 2.5f, m_pTransformCom->Get_State(CTransform::STATE_POSITION), 3.5f)) {
					// 해당 오브젝트가 데미지를 입는다.
					dynamic_cast<CUnbreakable_Rock*>(Object)->Meteo();
				}
			}

		}

	}
}

HRESULT CMeteo::SetUp_Components(void* pArg)
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransDesc;

	if (nullptr == pArg)
		return E_FAIL;
	else {
		TransDesc = *((CTransform::TRANSFORMDESC*)pArg);
	}


	random_device rd;


	/*_float3 vPos{};
	memcpy(&vPos, TransDesc.m_WorldMatrix.m[3], sizeof(_float3));*/
	////////////////* 초기 위치잡기 (랜덤하게 잡아줌) *///////////
	_float fX = TransDesc.m_WorldMatrix.m[3][0] + rand() % 20 + 3.f;
	_float fY = TransDesc.m_WorldMatrix.m[3][1] + 15.f;
	_float fZ = TransDesc.m_WorldMatrix.m[3][2] + rand() % 20 + 1.f;

	fPos = { fX,fY,fZ };
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////* 크기 잡기 *////////////////////////
	_float fScale = 3.f;

	TransDesc.m_WorldMatrix._11 /= fScale;
	TransDesc.m_WorldMatrix._12 /= fScale;
	TransDesc.m_WorldMatrix._13 /= fScale;

	TransDesc.m_WorldMatrix._21 /= fScale;
	TransDesc.m_WorldMatrix._22 /= fScale;
	TransDesc.m_WorldMatrix._23 /= fScale;

	TransDesc.m_WorldMatrix._31 /= fScale;
	TransDesc.m_WorldMatrix._32 /= fScale;
	TransDesc.m_WorldMatrix._33 /= fScale;
	////////////////////////*축 비틀기*////////////////////////
	_float Ux = TransDesc.m_WorldMatrix.m[1][0];
	_float Uy = TransDesc.m_WorldMatrix.m[1][1];
	_float Uz = TransDesc.m_WorldMatrix.m[1][2];

	vUp = { Ux,Uy,Uz };
	_float4x4 RotationMatrix;
	int iAngle = rand() % 49 + 1.f;
	D3DXMatrixRotationAxis(&RotationMatrix, &_float3(1.f, 0.f, 0.f), D3DXToRadian(iAngle));
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	memcpy(TransDesc.m_WorldMatrix.m[3], &fPos, sizeof(_float3));
	//////////////////////////////////////////////////////////////
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(Component_Renderer, LEVEL_STATIC, Prototype_Compo_Renderer, (CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Meteo.dat"))))
		return E_FAIL;


	if (FAILED(__super::SetUp_Components(Component_Transform, LEVEL_STATIC, Prototype_Compo_Transform, (CComponent**)&m_pTransformCom, &TransDesc)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(Component_Texture, LEVEL_STATIC, TEXT("Prototype_Component_Texture_HyunWoo"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	TransDesc.fSpeedPerSec = 0.3f;
	TransDesc.fRotationPerSec = D3DXToRadian(10.0f);
	TransDesc.fScalePerSec = 1.f;

	//CCollider::ColliDesc colDesc;

	//colDesc.pTransform = m_pTransformCom;

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	/* For.Com_Collider */
	//if (FAILED(__super::SetUp_Components(TEXT("Com_Collider"), LEVEL_STATIC, TEXT("Prototype_Component_Collider"), (CComponent**)&m_pColliderCom, &colDesc)))
	//	return E_FAIL;

	return S_OK;
}

CMeteo * CMeteo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMeteo*	pInstance = new CMeteo(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created Dynamite"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMeteo::Clone(void * pArg)
{
	CMeteo*	pInstance = new CMeteo(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created Dynamite"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMeteo::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
