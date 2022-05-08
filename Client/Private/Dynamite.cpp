#include "stdafx.h"
#include "Dynamite.h"
#include "GameInstance.h"
#include "Player.h"
#include "Effect_TrainSmoke.h"
#include "Effect_Explo.h"
#include "Tree.h"
#include "Rock.h"
#include "SoundMgr.h"
CDynamite::CDynamite(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
	m_eType = OBJECT_TYPE::OBJECT_DYNAMITE;
}

CDynamite::CDynamite(const CDynamite & rhs)
	: CGameObject(rhs)
{

	m_eType = rhs.m_eType;
}

HRESULT CDynamite::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDynamite::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CDynamite::Tick(_float fTimeDelta)
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
	if (fEffectTime > 0.1f)
	{

		CEffect_TrainSmoke::TRAINSMOKEDESC TrainSmokeDesc;
		TrainSmokeDesc.eSmoke = CEffect_TrainSmoke::ST_DYNAMITE;
		TrainSmokeDesc.iTextureNumber = 50;
		TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][0] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).x;
		TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][1] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
		TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][2] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).z;

		if (fEffectTime > 0.3f)
		{
			pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Effect_Smoke_Dynamite"), TEXT("Prototype_GameObject_Effect_TrainSmoke"), &TrainSmokeDesc.TransDesc);
		}

		if (fTime > 5.f) /* 플레이어가 들었다 내려 놓으면으로 조건 바꾸기 */
		{

			CEffect_Explo::EXPLODESC	ExploDesc;
			ExploDesc.iTextureNumber = 43;
			ExploDesc.TransDesc.m_WorldMatrix.m[3][0] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).x;
			ExploDesc.TransDesc.m_WorldMatrix.m[3][1] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
			ExploDesc.TransDesc.m_WorldMatrix.m[3][2] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).z;
			for (_uint i = 0; i < 30; i++)
			{
				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Effect_Explo"), TEXT("Prototype_GameObject_Effect_Explo"), &ExploDesc.TransDesc);
				m_bDead = true;
			}

			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EXPLO);
			CSoundMgr::Get_Instance()->PlaySound(TEXT("explo.mp3"), CSoundMgr::CHANNELID::EXPLO, 0.7f);
			CLayer* pLayerMap = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, Layer_Map);
			list<CGameObject*>& ObjectList = pLayerMap->Get_ObjectList();

			if (!ObjectList.empty())
			{
				for (auto& Object : ObjectList)
				{
					// 오브젝트 타입이 나무와 관련된 타입일 경우
					/////////////////////////////// 나무! //////////////////////////////
					if (Object->Get_Type() == OBJECT_TYPE::OBJECT_TREE_DARK1 ||
						Object->Get_Type() == OBJECT_TYPE::OBJECT_TREE_DARK2 ||
						Object->Get_Type() == OBJECT_TYPE::OBJECT_TREE_DARK3 ||
						Object->Get_Type() == OBJECT_TYPE::OBJECT_TREE_DARK4 ||
						Object->Get_Type() == OBJECT_TYPE::OBJECT_TREE_DARK_HIT1 ||
						Object->Get_Type() == OBJECT_TYPE::OBJECT_TREE_DARK_HIT2 ||
						Object->Get_Type() == OBJECT_TYPE::OBJECT_TREE_LIGHT ||
						Object->Get_Type() == OBJECT_TYPE::OBJECT_TREE_LIGHT_HIT1 ||
						Object->Get_Type() == OBJECT_TYPE::OBJECT_TREE_LIGHT_HIT2)
					{
						//////////////////////////////////////////////////////////////////
						// 해당 오브젝트의 위치를 가져온다.
						_float3 vResourcePos = dynamic_cast<CTransform*>(Object->Get_Component(Component_Transform))->Get_State(CTransform::STATE_POSITION);

						// 다이너마이트와 가져온 오브젝트의 위치 사이의 값이 4.f이하인 경우 
						if (pGameInstance->CheckIntersect(vResourcePos, 0.f, m_pTransformCom->Get_State(CTransform::STATE_POSITION), 3.5f)) {
							// 해당 오브젝트가 데미지를 입는다.
							dynamic_cast<CTree*>(Object)->Dynamite();
						}
					}
					else if (
						/////////////////////////////// 돌! //////////////////////////////
						Object->Get_Type() == OBJECT_TYPE::OBJECT_ROCK1 ||
						Object->Get_Type() == OBJECT_TYPE::OBJECT_ROCK2 ||
						Object->Get_Type() == OBJECT_TYPE::OBJECT_ROCK3 ||
						Object->Get_Type() == OBJECT_TYPE::OBJECT_ROCK_HIT1 ||
						Object->Get_Type() == OBJECT_TYPE::OBJECT_ROCK_HIT2)
					{
						//////////////////////////////////////////////////////////////////
						// 해당 오브젝트의 위치를 가져온다.
						_float3 vResourcePos = dynamic_cast<CTransform*>(Object->Get_Component(Component_Transform))->Get_State(CTransform::STATE_POSITION);

						// 다이너마이트와 가져온 오브젝트의 위치 사이의 값이 4.f이하인 경우 
						if (pGameInstance->CheckIntersect(vResourcePos, 0.f, m_pTransformCom->Get_State(CTransform::STATE_POSITION), 3.5f))
						{
							// 해당 오브젝트가 데미지를 입는다.
							dynamic_cast<CRock*>(Object)->Dynamite();
						}
					}

				}
			}
			fTime = 0.f;
		}

		__super::Tick(fTimeDelta);

	}
}

void CDynamite::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CDynamite::Render()
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

HRESULT CDynamite::SetUp_Components(void* pArg)
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransDesc;

	if (nullptr == pArg)
		return E_FAIL;
	else {
		TransDesc = *((CTransform::TRANSFORMDESC*)pArg);
	}
	_float3 vPos{};
	memcpy(&vPos, TransDesc.m_WorldMatrix.m[3], sizeof(_float3));

	vPos.y = 1.f;

	TransDesc.fSpeedPerSec = 0.3f;
	TransDesc.fRotationPerSec = D3DXToRadian(10.0f);
	TransDesc.fScalePerSec = 1.f;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(Component_Renderer, LEVEL_STATIC, Prototype_Compo_Renderer, (CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	_float fScale = 10.f;

	TransDesc.m_WorldMatrix._11 /= fScale;
	TransDesc.m_WorldMatrix._12 /= fScale;
	TransDesc.m_WorldMatrix._13 /= fScale;

	TransDesc.m_WorldMatrix._21 /= fScale;
	TransDesc.m_WorldMatrix._22 /= fScale;
	TransDesc.m_WorldMatrix._23 /= fScale;

	TransDesc.m_WorldMatrix._31 /= fScale;
	TransDesc.m_WorldMatrix._32 /= fScale;
	TransDesc.m_WorldMatrix._33 /= fScale;

	memcpy(TransDesc.m_WorldMatrix.m[3], &vPos, sizeof(_float3));

	if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Dynamite.dat"))))
		return E_FAIL;


	if (FAILED(__super::SetUp_Components(Component_Transform, LEVEL_STATIC, Prototype_Compo_Transform, (CComponent**)&m_pTransformCom, &TransDesc)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(Component_Texture, LEVEL_STATIC, TEXT("Prototype_Component_Texture_HyunWoo"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	/* For.Com_Collider */
	CCollider::ColliDesc colDesc;
	colDesc.pTransform = m_pTransformCom;


	//explosion이 안생겼을떄
	colDesc.localMatrix.m[0][0] = 1.f;
	colDesc.localMatrix.m[1][1] = 1.f;
	colDesc.localMatrix.m[2][2] = 1.f;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	if (FAILED(__super::SetUp_Components(TEXT("Com_Collider"), LEVEL_STATIC, TEXT("Prototype_Component_Collider"), (CComponent**)&m_pColliderCom, &colDesc)))
		return E_FAIL;

	return S_OK;
}

CDynamite * CDynamite::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDynamite*	pInstance = new CDynamite(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created Dynamite"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDynamite::Clone(void * pArg)
{
	CDynamite*	pInstance = new CDynamite(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created Dynamite"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDynamite::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
