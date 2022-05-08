#include "stdafx.h"
#include "Rock.h"
#include "GameInstance.h"

CRock::CRock(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device), m_iLife(3)
{
	m_eType = (OBJECT_TYPE)(int)OBJECT_TYPE::OBJECT_ROCK1;
}

CRock::CRock(const CRock & rhs)
	: CGameObject(rhs)
{
	m_iLife = rhs.m_iLife;
	m_eType = rhs.m_eType;
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
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	
	__super::Tick(fTimeDelta);

	Safe_Release(pGameInstance);
}

void CRock::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (m_iLife <= 0)
		m_bDead;
	else
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CRock::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	// 라이프는 최대 3, 1이되고 다음 타이밍에 삭제된다.
	auto& ObjectList = m_pVIBufferCom->Get_lstVIBuffer();

	for (auto& Object : ObjectList) {

		if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
			return E_FAIL;

		if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(Object.second)))
			return E_FAIL;

		Object.first->Render();
	}

	return S_OK;
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
	RockDesc.TransDesc.fScalePerSec = 1.f;

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

	if (FAILED(__super::SetUp_Components(Component_Transform, LEVEL_STATIC, Prototype_Compo_Transform, (CComponent**)&m_pTransformCom, &RockDesc.TransDesc)))
		return E_FAIL;
	
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

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(Component_Texture, LEVEL_STATIC, TEXT("Prototype_Component_Texture_JuhYeon"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;



	return S_OK;
}

void CRock::Get_Damage(void)
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
		if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Rockhit2.dat"))))
			m_eType = OBJECT_TYPE::OBJECT_ROCK_HIT2;
			return;
		break;
	case 2:
		Delete_Component(Component_VIBuffer);
		if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Rockhit1.dat"))))
			m_eType = OBJECT_TYPE::OBJECT_ROCK_HIT1;
			return;
		break;
	}
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

}
