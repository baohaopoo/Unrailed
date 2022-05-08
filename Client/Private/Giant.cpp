#include "stdafx.h"
#include "..\Public\Giant.h"
#include "GameInstance.h"
#include "SoundMgr.h"
CGiant::CGiant(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
	m_eType = OBJECT_TYPE::OBJECT_GIANT;
}

CGiant::CGiant(const CGiant & rhs)
	: CGameObject(rhs)
{
	m_eType = rhs.m_eType;
}

HRESULT CGiant::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGiant::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;


	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3( 20.f, 5.f, 40.f ));
	return S_OK;
}

void CGiant::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	_float3 GiantPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	fTime += fTimeDelta;

	if (fTime > 30.f)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::GIANT);
		CSoundMgr::Get_Instance()->PlaySound(TEXT("GiantLaugh.mp3"), CSoundMgr::CHANNELID::GIANT, SOUND_MAX);
		fTime = 0.f;
	}
	if (m_updown == false)
	{
		GiantPos.y += 0.04f;
		fup += 0.04f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, GiantPos);
		if (fup >= 3.f)
		{
			fup = 0.f;
			m_updown = true;
		}
	}

	else if (m_updown == true)
	{
		GiantPos.y -= 0.04f;
		fdown -= 0.04f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, GiantPos);
		if (fdown <= -3.f)
		{
			fdown = 0.f;
			m_updown = false;
		}
	}
}

void CGiant::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup_NonCulling(CRenderer::GROUP_NONBLEND, this);		//나중에 합칠 때 Engine 프로젝트의 Transform클래스에 추가해준다.
}

HRESULT CGiant::Render()
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

HRESULT CGiant::SetUp_Components(void * pArg)
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransDesc;

	if (nullptr == pArg)
		return E_FAIL;
	else
	{
		TransDesc = *((CTransform::TRANSFORMDESC*)pArg);
	}
	_float3 vPos{};
	memcpy(&vPos, TransDesc.m_WorldMatrix.m[3], sizeof(_float3));

	TransDesc.fSpeedPerSec = 1.f;
	TransDesc.fRotationPerSec = D3DXToRadian(10.0f);
	TransDesc.fScalePerSec = 1.f;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(Component_Renderer, LEVEL_STATIC, Prototype_Compo_Renderer, (CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	D3DXMatrixRotationY(&TransDesc.m_WorldMatrix, D3DXToRadian(180.f));
	// 배율
	//_float fScale = 10.f;

	//TransDesc.m_WorldMatrix._11 *= 1.5f;
	//TransDesc.m_WorldMatrix._12 /= fScale;
	//TransDesc.m_WorldMatrix._13 /= fScale;

	//TransDesc.m_WorldMatrix._21 /= fScale;
	//TransDesc.m_WorldMatrix._22 *= 1.2f;
	//TransDesc.m_WorldMatrix._23 /= fScale;

	//TransDesc.m_WorldMatrix._31 /= fScale;
	//TransDesc.m_WorldMatrix._32 /= fScale;
	//TransDesc.m_WorldMatrix._33 /= fScale;

	memcpy(TransDesc.m_WorldMatrix.m[3], &vPos, sizeof(_float3));

	if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("GiantBody.dat"))))
		return E_FAIL;

	// Tree의 스케일 조정
	if (FAILED(__super::SetUp_Components(Component_Transform, LEVEL_STATIC, Prototype_Compo_Transform, (CComponent**)&m_pTransformCom, &TransDesc)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(Component_Texture, LEVEL_STATIC, TEXT("Prototype_Component_Texture_HyunWoo"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	CCollider::ColliDesc colDesc;

	colDesc.localMatrix;
	colDesc.pTransform = m_pTransformCom;
	//	m_pTransformCom->Scaled(_float3(0.1f,0.1f, 0.1f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	/* For.Com_Collider */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Collider"), LEVEL_STATIC, TEXT("Prototype_Component_Collider"), (CComponent**)&m_pColliderCom, &colDesc)))
		return E_FAIL;

	return S_OK;
}

CGiant * CGiant::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGiant*	pInstance = new CGiant(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created Giant"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CGiant::Clone(void * pArg)
{
	CGiant*	pInstance = new CGiant(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created Clone Giant"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGiant::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}