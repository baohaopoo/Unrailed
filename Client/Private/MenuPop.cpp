#include "stdafx.h"
#include "..\Public\MenuPop.h"
#include "GameInstance.h"

MenuPop::MenuPop(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

MenuPop::MenuPop(const MenuPop & rhs)
	:CGameObject(rhs)
{
}

HRESULT MenuPop::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT MenuPop::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fSizeX = 300.f;
	m_fSizeY = 300.f;
	m_fX = 660.0f;
	m_fY = 300.0f;

	D3DXCreateFont(m_pGraphic_Device, 10, 9, 920, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"오이체", &font);



	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinCX, g_iWinCY, 0.f, 1.f);

	m_pTransformCom->Scaled(_float3(1.f, 1.f, 1.f));
	return S_OK;
}

void MenuPop::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	RECT rc{ 780,100,900,200 };
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);


	if (PtInRect(&rc, pt) && GetAsyncKeyState(VK_LBUTTON)) {


		m_isRender = false;
		//여기서 새 오브젝트 생성
	}




	m_pTransformCom->Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinCX * 0.5f, -m_fY + g_iWinCY * 0.5f, 0.0f));
}

void MenuPop::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup_UI(this);
}

HRESULT MenuPop::Render()
{
	if (m_isRender) {
		if (FAILED(__super::Render()))
			return E_FAIL;

		if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(0)))
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

		m_pVIBufferCom->Render();

		m_pGraphic_Device->SetTransform(D3DTS_VIEW, &OriginView);
		m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &OriginProj);

	}
	return S_OK;
}



HRESULT MenuPop::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	//ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	TransformDesc.fScalePerSec = 1.f;

	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom, nullptr)))
		return E_FAIL;
	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_MenuPOP"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	return S_OK;
}

MenuPop * MenuPop::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	MenuPop*	pInstance = new MenuPop(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created MenuPop"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * MenuPop::Clone(void * pArg)
{
	MenuPop*	pInstance = new MenuPop(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created MenuPop"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void MenuPop::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
