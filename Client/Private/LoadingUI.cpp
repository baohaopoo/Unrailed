#include "stdafx.h"
#include"..\Public\LoadingUI.h"
#include "GameInstance.h"

LoadingUI::LoadingUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

LoadingUI::LoadingUI(const LoadingUI & rhs)
	:CGameObject(rhs)
{
}

HRESULT LoadingUI::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT LoadingUI::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fSizeX = 1280.f;
	m_fSizeY = 780.f;
	m_fX = 640.0f;
	m_fY = 340.0f;

	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinCX, g_iWinCY, 0.f, 1.f);
	m_pTransformCom->Scaled(_float3(1.f, 3.f, 1.f));
	return S_OK;

	return S_OK;

	
}

void LoadingUI::Tick(_float fTimeDelta)
{
	_float3 Btn = m_pTransformCom->Get_State(CTransform::STATE_POSITION);


	if (GetAsyncKeyState('T'))
	{
		//pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_LoadingUI"),m_pT
		//);
	}


	RECT rc{ Btn.x - 100, Btn.y + 100, Btn.x + 100, Btn.y - 100/*300,500,500,600*/ };
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);


	if (PtInRect(&rc, pt) && GetAsyncKeyState(VK_LBUTTON)) {
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(10.f, 20.f, 0.f));

	}
	m_pTransformCom->Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinCX * 0.5f, -m_fY + g_iWinCY * 0.5f, 0.0f));

}

void LoadingUI::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup_UI(this);
}

HRESULT LoadingUI::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	_float4x4			ViewMatrix, ProjMatrix;
	D3DXMatrixIdentity(&ViewMatrix);















	//직교 투영용
	_float4x4 OriginView;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &OriginView);
	_float4x4 OriginProj;
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &OriginProj);

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pVIBufferCom->Render();

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &OriginView);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &OriginProj);


	/*_float4x4			ViewMatrix, ProjMatrix;

	D3DXMatrixLookAtLH(&ViewMatrix, &_float3(0.f, 1.f, -10.f), &_float3(0.f, 0.f, 0.f), &_float3(0.f, 1.f, 0.f));
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);

	D3DXMatrixPerspectiveFovLH(&ProjMatrix, D3DXToRadian(60.0f), (_float)g_iWinCX / g_iWinCY, 0.2f, 300.f);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &ProjMatrix);

	m_pVIBufferCom->Render();*/





	return S_OK;
}

HRESULT LoadingUI::SetUp_Components()
{






	CTransform::TRANSFORMDESC		TransformDesc;

	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	TransformDesc.fScalePerSec = 1.f;

	/* For.Com_Tranform */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom, nullptr)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_emptyLoading"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	return S_OK;



}

LoadingUI * LoadingUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	LoadingUI*	pInstance = new LoadingUI(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created LoadingUI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * LoadingUI::Clone(void * pArg)
{
	LoadingUI*	pInstance = new LoadingUI(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created LoadingUI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void LoadingUI::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
