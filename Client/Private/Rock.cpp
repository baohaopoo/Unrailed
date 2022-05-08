#include "stdafx.h"
#include "Rock.h"
#include "GameInstance.h"
#include "Effect_Explo.h"

CRock::CRock(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device), m_iLife(3)
{
	m_eType = OBJECT_TYPE::OBJECT_ROCK1;
}

CRock::CRock(const CRock & rhs)
	: CGameObject(rhs)
{
	m_iLife = rhs.m_iLife;
	m_eType = rhs.m_eType;
	m_fEffectTime = rhs.m_fEffectTime;
}

HRESULT CRock::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CRock::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CRock::Tick(_float fTimeDelta)
{


	__super::Tick(fTimeDelta);

	/*m_fEffectTime += fTimeDelta;

	Ready_Layer_Effect(OBJECT_PICKAX);*/

}

void CRock::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (m_iLife <= 0) {

		if (FAILED(Ready_Layer_reward(TEXT("Layer_Iron"))))
			return;

		m_bDead = true;
	}
	else
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CRock::Render()
{
	/*	if (m_isRender)*/ {
		if (FAILED(__super::Render()))
			return E_FAIL;

		if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
			return E_FAIL;

		// 라이프는 최대 3, 1이되고 다음 타이밍에 삭제된다.
		auto& ObjectList = m_pVIBufferCom->Get_lstVIBuffer();

		for (auto& Object : ObjectList) {


			if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(Object.second)))
				return E_FAIL;

			Object.first->Render();
		}


	}	return S_OK;
}

void CRock::Dynamite()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	m_iLife = 0;
	CEffect_Explo::EXPLODESC	ExploDesc;

	ExploDesc.iTextureNumber = 28;

	ExploDesc.TransDesc.m_WorldMatrix.m[3][0] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).x;
	ExploDesc.TransDesc.m_WorldMatrix.m[3][1] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
	ExploDesc.TransDesc.m_WorldMatrix.m[3][2] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).z;

	for (_uint i = 0; i < 20; i++)
	{
		pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Effect_Explo"), TEXT("Prototype_GameObject_Effect_Explo"), &ExploDesc.TransDesc);
	}
}

HRESULT CRock::SetUp_Components(void* pArg)
{
	/* For.Com_Transform */
	CRock::ROCKDESC		RockDesc;

	if (nullptr == pArg)
		return E_FAIL;
	else {
		RockDesc = *((CRock::ROCKDESC*)pArg);
	}

	RockDesc.TransDesc.fSpeedPerSec = 0.3f;
	RockDesc.TransDesc.fRotationPerSec = D3DXToRadian(10.0f);
	RockDesc.TransDesc.fScalePerSec = 1.0f;

	// rock의 스케일 조정
	RockDesc.TransDesc.m_WorldMatrix._11 /= 30.f;
	RockDesc.TransDesc.m_WorldMatrix._12 /= 30.f;
	RockDesc.TransDesc.m_WorldMatrix._13 /= 30.f;
	RockDesc.TransDesc.m_WorldMatrix._21 /= 30.f;
	RockDesc.TransDesc.m_WorldMatrix._22 /= 30.f;
	RockDesc.TransDesc.m_WorldMatrix._23 /= 30.f;
	RockDesc.TransDesc.m_WorldMatrix._31 /= 30.f;
	RockDesc.TransDesc.m_WorldMatrix._32 /= 30.f;
	RockDesc.TransDesc.m_WorldMatrix._33 /= 30.f;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(Component_Renderer, LEVEL_STATIC, Prototype_Compo_Renderer, (CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;
	/* For.Com_VIBuffer */
	switch (RockDesc.iRockNumber)
	{
	case 1:
		if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Rock1.dat"))))
			return E_FAIL;
		m_eType = OBJECT_TYPE::OBJECT_ROCK1;
		break;
	case 2:
		if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Rock2.dat"))))
			return E_FAIL;
		m_eType = OBJECT_TYPE::OBJECT_ROCK2;
		break;
	case 3:
		if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Rock3.dat"))))
			return E_FAIL;
		m_eType = OBJECT_TYPE::OBJECT_ROCK3;
		break;
	default:
		break;
	}


	if (FAILED(__super::SetUp_Components(Component_Transform, LEVEL_STATIC, Prototype_Compo_Transform, (CComponent**)&m_pTransformCom, &RockDesc.TransDesc)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(Component_Texture, LEVEL_STATIC, TEXT("Prototype_Component_Texture_JuHyun"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;



	CCollider::ColliDesc colDesc;
	colDesc.pTransform = m_pTransformCom;

	if (FAILED(__super::SetUp_Components(TEXT("Com_Collider"), LEVEL_STATIC, TEXT("Prototype_Component_Collider"), (CComponent**)&m_pColliderCom, &colDesc)))
		return E_FAIL;



	return S_OK;
}

bool CRock::CheckIntersect(_float3 center1, float radius1, _float3 center2, float radius2)
{
	float fDist;
	_float3 vSub;

	vSub = center2 - center1;
	fDist = D3DXVec3Length(&vSub);
	if (fDist <= (radius1 + radius2))
		return true;

	return false;
}



void CRock::Get_Damage(void)
{
	--m_iLife;
	switch (m_iLife)
	{
	case 0:
	{
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		// 데미지를 입어 체력이 0이 되었다. 해당 자리에 객체를 생성한다.
		if (FAILED(Ready_Layer_reward(TEXT("Layer_Iron"))))
			return;

		//CTransform::TRANSFORMDESC tradesc;
		//tradesc.m_WorldMatrix.m[3][0] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).x;
		//tradesc.m_WorldMatrix.m[3][1] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
		//tradesc.m_WorldMatrix.m[3][2] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).z;


		////이것떄뮨이다

		//if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_"), &tradesc)))
		//	return;

		/*pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Effect_Explo"), (void*)&m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		return;*/
		m_bDead = true;
	}
	break;
	// case 1, 2 둘다 기존에 있던 컴포넌트를 삭제하고 새로운 컴포넌트를 넣는다.
	case 1:
		Delete_Component(Component_VIBuffer);

		if (m_eType == OBJECT_ROCK_HIT1) {
			if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Rockhit2.dat"))))
				return;
			m_eType = OBJECT_TYPE::OBJECT_ROCK_HIT2;
		}
		break;
	case 2:
		Delete_Component(Component_VIBuffer);

		//if (m_eType == OBJECT_ROCK1 || m_eType == OBJECT_ROCK2 || m_eType == OBJECT_ROCK3) {
		//	if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Rockhit1.dat"))))
		//		return;
		//	//m_eType = OBJECT_TYPE::OBJECT_ROCK_HIT1;
		//}


		break;

	}
}

HRESULT CRock::Ready_Layer_Effect(OBJECT_TYPE eType)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	CGameObject* pColObject = m_pColliderCom->Get_CollisionObject();

	if (pColObject != nullptr) //충돌하는 물체가 있다면
	{
		if (pColObject->Get_Type() == eType)		//곡괭이 
		{
			if (m_fEffectTime>1.0f)
			{
				Get_Damage();
				CTransform::TRANSFORMDESC tradesc;
				tradesc.m_WorldMatrix.m[3][0] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).x;
				tradesc.m_WorldMatrix.m[3][1] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
				tradesc.m_WorldMatrix.m[3][2] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).z;

				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Effect_Explo"), TEXT("Prototype_GameObject_Effect_Explo"), &tradesc);

				m_fEffectTime = 0.f;
			}
		}
	}

	return S_OK;
}

HRESULT CRock::Ready_Layer_reward(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();


	CTransform::TRANSFORMDESC tradesc;

	tradesc.m_WorldMatrix.m[3][0] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).x;
	tradesc.m_WorldMatrix.m[3][1] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
	tradesc.m_WorldMatrix.m[3][2] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).z;


	//이것떄뮨이다

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Iron"), &tradesc)))
		return E_FAIL;



	return S_OK;
}

CRock * CRock::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRock*	pInstance = new CRock(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created Rock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRock::Clone(void * pArg)
{
	CRock*	pInstance = new CRock(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created Rock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRock::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pColliderCom);
}
