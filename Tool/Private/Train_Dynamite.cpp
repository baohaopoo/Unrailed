#include "stdafx.h"
#include "Train_Dynamite.h"
#include "GameInstance.h"

CTrain_Dynamite::CTrain_Dynamite(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device), m_iLife(3)
{
	m_eType = (OBJECT_TYPE)(int)OBJECT_TYPE::OBJECT_TRAIN_DYNAMITE;
}

CTrain_Dynamite::CTrain_Dynamite(const CTrain_Dynamite & rhs)
	: CGameObject(rhs)
{
	m_iLife = rhs.m_iLife;
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

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CTrain_Dynamite::Tick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	
	__super::Tick(fTimeDelta);

	Safe_Release(pGameInstance);
}

void CTrain_Dynamite::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (m_iLife <= 0)
		m_bDead;
	else
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CTrain_Dynamite::Render()
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

HRESULT CTrain_Dynamite::SetUp_Components(void* pArg)
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
	vPos.y = 0.5f;

	TransDesc.fSpeedPerSec = 0.3f;
	TransDesc.fRotationPerSec = D3DXToRadian(10.0f);
	TransDesc.fScalePerSec = 1.f;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(Component_Renderer, LEVEL_STATIC, Prototype_Compo_Renderer, (CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_VIBuffer */

	// 배율
	_float fScale = 30.f;
	
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

	if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Train_Dynamite.dat"))))
		return E_FAIL;

	// Tree의 스케일 조정
	if (FAILED(__super::SetUp_Components(Component_Transform, LEVEL_STATIC, Prototype_Compo_Transform, (CComponent**)&m_pTransformCom, &TransDesc)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(Component_Texture, LEVEL_STATIC, TEXT("Prototype_Component_Texture_HyunWoo"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;


	return S_OK;
}

void CTrain_Dynamite::Get_Damage(void)
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
		if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Rock2.dat"))))
			return;
		break;
	case 2:
		Delete_Component(Component_VIBuffer);
		if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Rock3.dat"))))
			return;
		break;
	}
}

CTrain_Dynamite * CTrain_Dynamite::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTrain_Dynamite*	pInstance = new CTrain_Dynamite(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created Rock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTrain_Dynamite::Clone(void * pArg)
{
	CTrain_Dynamite*	pInstance = new CTrain_Dynamite(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created Rock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrain_Dynamite::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
