#include "stdafx.h"
#include "Tree.h"
#include "GameInstance.h"

CTree::CTree(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device), m_iLife(3)
{
	m_eType = (OBJECT_TYPE)(int)OBJECT_TYPE::OBJECT_TREE_DARK1;
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
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	
	__super::Tick(fTimeDelta);

	Safe_Release(pGameInstance);
}

void CTree::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (m_iLife <= 0)
		m_bDead;
	else
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CTree::Render()
{
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

	TreeDesc.TransDesc.fSpeedPerSec = 0.3f;
	TreeDesc.TransDesc.fRotationPerSec = D3DXToRadian(10.0f);
	TreeDesc.TransDesc.fScalePerSec = 1.f;

	// rock의 스케일 조정
	TreeDesc.TransDesc.m_WorldMatrix._11 /= 30.f;
	TreeDesc.TransDesc.m_WorldMatrix._12 /= 30.f;
	TreeDesc.TransDesc.m_WorldMatrix._13 /= 30.f;
	TreeDesc.TransDesc.m_WorldMatrix._21 /= 30.f;
	TreeDesc.TransDesc.m_WorldMatrix._22 /= 30.f;
	TreeDesc.TransDesc.m_WorldMatrix._23 /= 30.f;
	TreeDesc.TransDesc.m_WorldMatrix._31 /= 30.f;
	TreeDesc.TransDesc.m_WorldMatrix._32 /= 30.f;
	TreeDesc.TransDesc.m_WorldMatrix._33 /= 30.f;

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

	return S_OK;
}

void CTree::Get_Damage(void)
{
	--m_iLife;

	switch (m_iLife)
	{
	case 0:
		// 데미지를 입어 체력이 0이 되었다. 해당 자리에 객체를 생성한다.

		break;
		// case 1, 2 둘다 기존에 있던 컴포넌트를 삭제하고 새로운 컴포넌트를 넣는다.
	case 1:
		Delete_Component(Component_VIBuffer);

		if (m_eType == OBJECT_TREE_LIGHT) {
			if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Tree_Light_Hit1.dat"))))
				return;
			m_eType = OBJECT_TREE_LIGHT_HIT1;
		}
		else {
			if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Tree_Dark_Hit1.dat"))))
				return;
			m_eType = OBJECT_TREE_DARK_HIT1;
		}
		break;

	case 2:
		Delete_Component(Component_VIBuffer);

		if (m_eType == OBJECT_TREE_LIGHT_HIT1) {
			if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Tree_Light_Hit2.dat"))))
				return;
		}
		else {
			if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Tree_Dark_Hit2.dat"))))
				return;
		}
		break;
	}
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

}
