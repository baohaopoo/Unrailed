#include "stdafx.h"
#include "Tree.h"
#include "GameInstance.h"
#include "Effect_Explo.h"

CTree::CTree(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device), m_iLife(3)
{
	m_eType = OBJECT_TYPE::OBJECT_TREE_DARK1;
}

CTree::CTree(const CTree & rhs)
	: CGameObject(rhs)
{
	m_iLife = rhs.m_iLife;
	m_eType = rhs.m_eType;
}

HRESULT CTree::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTree::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CTree::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	CGameObject* pColObject = m_pColliderCom->Get_CollisionObject();
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	if (pColObject != nullptr)
	{
		if (pColObject->Get_Type() == OBJECT_TYPE::OBJECT_DYNAMITE)
		{

			m_dynamiteTime += fTimeDelta;
			if (m_dynamiteTime > 4) {
				m_iLife = 0;
		
				m_dynamiteTime = 0.f;
			}

		}

	}
}

void CTree::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (m_iLife <= 0) {

		if (FAILED(Ready_Layer_reward(TEXT("Layer_Wood"))))
			return;

		m_bDead = true;
	}
	else
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CTree::Render()
{
	/*if (m_isRender)*/ {
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
	}
	return S_OK;
}

void CTree::Dynamite()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	random_device(dre);
	m_iLife = 0;
	CEffect_Explo::EXPLODESC	ExploDesc;

	ExploDesc.iTextureNumber = uid(dre);

	ExploDesc.TransDesc.m_WorldMatrix.m[3][0] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).x;
	ExploDesc.TransDesc.m_WorldMatrix.m[3][1] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
	ExploDesc.TransDesc.m_WorldMatrix.m[3][2] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).z;

	for (_uint i = 0; i < 20; i++)
	{
		pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Effect_Explo"), TEXT("Prototype_GameObject_Effect_Explo"), &ExploDesc.TransDesc);
	}
}

HRESULT CTree::Ready_Layer_reward(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();


	CTransform::TRANSFORMDESC tradesc;

	tradesc.m_WorldMatrix.m[3][0] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).x;
	tradesc.m_WorldMatrix.m[3][1] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
	tradesc.m_WorldMatrix.m[3][2] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).z;


	//이것떄뮨이다

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Wood"), &tradesc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CTree::Ready_Layer_Effect(OBJECT_TYPE eType)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	CGameObject* pColObject = m_pColliderCom->Get_CollisionObject();

	if (pColObject != nullptr) //충돌하는 물체가 있다면
	{
		/*if (pColObject->Get_Type() == && PlayerState == 2)*/		//곡괭이 
		{
			if (m_fEffectTime>1.0f)
			{
			
				Get_Damage();
				CTransform::TRANSFORMDESC tradesc;


				tradesc.m_WorldMatrix.m[3][0] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).x;
				tradesc.m_WorldMatrix.m[3][1] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
				tradesc.m_WorldMatrix.m[3][2] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).z;



				/*_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
				memcpy(tradesc.m_WorldMatrix.m[3], &vPos, sizeof(_float3));*/



				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Effect_Explo"), TEXT("Prototype_GameObject_Effect_Explo"), &tradesc);

				m_fEffectTime = 0.f;
			}
		}
	}
	return S_OK;
}



HRESULT CTree::SetUp_Components(void* pArg)
{
	/* For.Com_Transform */
	CTree::TreeDESC		TreeDesc;

	if (nullptr == pArg)
		return E_FAIL;
	else {
		TreeDesc = *((CTree::TreeDESC*)pArg);
	}

	TreeDesc.TransDesc.m_WorldMatrix._42 = 0.5;

	if (goMakeWood)
	{
		//여기서 정보저장해서 나갈준비
		woodPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	}
	TreeDesc.TransDesc.fSpeedPerSec = 0.3f;
	TreeDesc.TransDesc.fRotationPerSec = D3DXToRadian(10.0f);
	TreeDesc.TransDesc.fScalePerSec = 1.f;

	// rock의 스케일 조정
	TreeDesc.TransDesc.m_WorldMatrix._11 /= 20.f;
	TreeDesc.TransDesc.m_WorldMatrix._12 /= 20.f;
	TreeDesc.TransDesc.m_WorldMatrix._13 /= 20.f;
	TreeDesc.TransDesc.m_WorldMatrix._21 /= 20.f;
	TreeDesc.TransDesc.m_WorldMatrix._22 /= 20.f;
	TreeDesc.TransDesc.m_WorldMatrix._23 /= 20.f;
	TreeDesc.TransDesc.m_WorldMatrix._31 /= 20.f;
	TreeDesc.TransDesc.m_WorldMatrix._32 /= 20.f;
	TreeDesc.TransDesc.m_WorldMatrix._33 /= 20.f;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(Component_Renderer, LEVEL_STATIC, Prototype_Compo_Renderer, (CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	switch (TreeDesc.iTreeNumber)
	{
	case 1:
		if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Tree_Dark_Ver1.dat"))))
			return E_FAIL;
		m_eType = OBJECT_TYPE::OBJECT_TREE_DARK1;
		break;
	case 2:
		if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Tree_Dark_Ver2.dat"))))
			return E_FAIL;
		m_eType = OBJECT_TYPE::OBJECT_TREE_DARK2;
		break;
	case 3:
		if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Tree_Dark_Ver3.dat"))))
			return E_FAIL;
		m_eType = OBJECT_TYPE::OBJECT_TREE_DARK3;
		break;
	case 4:
		if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Tree_Dark_Ver4.dat"))))
			return E_FAIL;
		m_eType = OBJECT_TYPE::OBJECT_TREE_DARK4;
		break;
	case 5:
		if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Tree_Light.dat"))))
			return E_FAIL;
		m_eType = OBJECT_TYPE::OBJECT_TREE_LIGHT;
		break;
	default:
		break;
	}

	// Tree의 스케일 조정
	if (FAILED(__super::SetUp_Components(Component_Transform, LEVEL_STATIC, Prototype_Compo_Transform, (CComponent**)&m_pTransformCom, &TreeDesc.TransDesc)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(Component_Texture, LEVEL_STATIC, TEXT("Prototype_Component_Texture_HyunWoo"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;


	CCollider::ColliDesc colDesc;

	colDesc.pTransform = m_pTransformCom;

	/* For.Com_Collider */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Collider"), LEVEL_STATIC, TEXT("Prototype_Component_Collider"), (CComponent**)&m_pColliderCom, &colDesc)))
		return E_FAIL;

	return S_OK;
}

void CTree::Get_Damage(void)
{

	--m_iLife;
	switch (m_iLife)
	{
	case 0:
		// 데미지를 입어 체력이 0이 되었다. 해당 자리에 객체를 생성한다.
		//if (FAILED(Ready_Layer_reward(TEXT("Layer_Wood")))) 
		//	return;
		if (FAILED(Ready_Layer_reward(TEXT("Layer_Wood"))))
			return;


		m_bDead = true;
		break;
		// case 1, 2 둘다 기존에 있던 컴포넌트를 삭제하고 새로운 컴포넌트를 넣는다.
	case 1:
		Delete_Component(Component_VIBuffer);
		
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, (m_pTransformCom->Get_State(CTransform::STATE_RIGHT) *= 0.9));
		m_pTransformCom->Set_State(CTransform::STATE_UP, (m_pTransformCom->Get_State(CTransform::STATE_UP) *= 0.9));
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, (m_pTransformCom->Get_State(CTransform::STATE_LOOK) *= 0.9));

		//pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Effect_Explo"), &tradesc);

		if (m_eType == OBJECT_TREE_LIGHT) {
			if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Tree_Light_Hit2.dat"))))
				return;
			m_eType = OBJECT_TREE_LIGHT_HIT2;
		}
		else {
			if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Tree_Dark_Hit2.dat"))))
				return;
			m_eType = OBJECT_TREE_DARK_HIT2;
		}
		break;

	case 2:
		Delete_Component(Component_VIBuffer);

		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, (m_pTransformCom->Get_State(CTransform::STATE_RIGHT) *= 0.8));
		m_pTransformCom->Set_State(CTransform::STATE_UP, (m_pTransformCom->Get_State(CTransform::STATE_UP) *= 0.8));
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, (m_pTransformCom->Get_State(CTransform::STATE_LOOK) *= 0.8));

		if (m_eType == OBJECT_TREE_LIGHT_HIT1) {
			if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Tree_Light_Hit1.dat"))))
				return;
		}
		else {
			if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Tree_Dark_Hit1.dat"))))
				return;
		}
		break;
	}
}

bool CTree::Making_Wood()
{
	//이곳에서 Play Level로 던져줄거다. 뭐를?
	//해당 아이의 위치를


	goMakeWood = true;
	return goMakeWood;

}

_float3 CTree::getWoodPos()
{
	return woodPos;
}

CTree * CTree::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTree*	pInstance = new CTree(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created Tree"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTree::Clone(void * pArg)
{
	CTree*	pInstance = new CTree(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone Tree"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTree::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pColliderCom);
}
