#include "stdafx.h"
#include "..\Public\BucketProgressBar.h"
#include "GameInstance.h"
#include "SoundMgr.h"

BucketProgressBar::BucketProgressBar(LPDIRECT3DDEVICE9 pGrahpic_Device)
	:CGameObject(pGrahpic_Device)
{
}

BucketProgressBar::BucketProgressBar(const BucketProgressBar & rhs)
	: CGameObject(rhs)
{
}

HRESULT BucketProgressBar::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT BucketProgressBar::NativeConstruct(void * pArg)
{

	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	m_fSizeX = 0.2f;
	m_fSizeY = 0.f;


	m_pTransformCom->Scaled(_float3(0.5f, 0.1f, 0.5f));
	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinCX, g_iWinCY, 0.f, 1.f);


	return S_OK;
}

void BucketProgressBar::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	if (m_pTransformCom != nullptr)
		return;


	m_pTransformCom->Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinCX * 0.5f, -m_fY + g_iWinCY * 0.5f, 0.0f));



	//if (m_fSizeX < 1.f)
	//{

	//	timeControl += fTimeDelta;
	//	if (timeControl > 1) {

	//		m_fSizeX += 0.2f;
	//		
	//		CSoundMgr::Get_Instance()->PlaySound(TEXT("progress_Bar.mp3"), CSoundMgr::CHANNELID::PROGRESS_BAR, SOUND_DEFAULT);
	//		if (m_fSizeX >= 1.f)
	//		{
	//			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PROGRESS_BAR);
	//		}

	//		timeControl = 0.f;
	//	}
	//}
}

void BucketProgressBar::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (m_fSizeY < 0.5f)
	{

		timeControl += fTimeDelta;
		if (timeControl > 0.2) {

			m_fSizeY += 0.2f;
			m_pTransformCom->Scaled(_float3(0.5f, m_fSizeY, 0.5f));

			CSoundMgr::Get_Instance()->PlaySound(TEXT("progress_Bar.mp3"), CSoundMgr::CHANNELID::PROGRESS_BAR, SOUND_DEFAULT);
			if (m_fSizeY >= 0.5f)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PROGRESS_BAR);
			}

			timeControl = 0.f;
		}
	}
	else if(m_fSizeY >=0.5){
			m_pTransformCom->Scaled(_float3(0.5f, m_fSizeY, 0.5f));
	}


	m_pRendererCom->Add_RenderGroup_UI(this);
}

HRESULT BucketProgressBar::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	_float4x4			ViewMatrix, ProjMatrix;
	D3DXMatrixIdentity(&ViewMatrix);


	_float4x4 OriginView;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &OriginView);
	_float4x4 OriginProj;
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &OriginProj);

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &OriginView);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &OriginProj);


	m_pVIBufferCom->Render();
	return S_OK;
}

HRESULT BucketProgressBar::SetUp_Components(void* pArg)
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


	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_UI"), (CComponent**)&m_pVIBufferCom, nullptr)))
		return E_FAIL;


	//너때문에 고생오지게 함.
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_WaterProgressBar"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	return S_OK;
}

BucketProgressBar * BucketProgressBar::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	BucketProgressBar*	pInstance = new BucketProgressBar(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created BucketProgressBar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * BucketProgressBar::Clone(void * pArg)
{
	BucketProgressBar*	pInstance = new BucketProgressBar(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created BucketProgressBar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void BucketProgressBar::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
