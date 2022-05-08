#include "stdafx.h"
#include "..\Public\lifeUI.h"
#include "GameInstance.h"

lifeUI::lifeUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

lifeUI::lifeUI(const lifeUI & rhs)
	: CGameObject(rhs)
{
}

HRESULT lifeUI::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT lifeUI::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	D3DXCreateFont(m_pGraphic_Device, 15, 7, 920, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"오이체", &font);

	m_fSizeX = 60.f;
	m_fSizeY = 60.f;
	m_fX = 260.0f;
	m_fY = 50.0f;

	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinCX, g_iWinCY, 0.f, 1.f);

	m_pTransformCom->Scaled(_float3(1.f, 1.f, 1.f));

	return S_OK;
}

void lifeUI::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);



	_float3 Btn = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

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

void lifeUI::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	timeControl += fTimeDelta;
	if (timeControl > 2.f) {

		++speed;
		timeControl = 0.f;
	}


	m_pRendererCom->Add_RenderGroup_UI(this);
}

HRESULT lifeUI::Render()
{
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

	//for(int i = 0; i<100; ++i)
	_tchar string1[100] = L"속도 \n\n";


	_tchar szBuff[256] = L"";
	wsprintf(szBuff, L"%d.m/s", speed);
	lstrcat(string1, szBuff);

	//.m / s;
	RECT rt1 = { 244,30,300,300 };
	font->DrawTextW(NULL, string1, -1, &rt1, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));



	return S_OK;
}

HRESULT lifeUI::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_btn"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	return S_OK;
}

lifeUI * lifeUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	lifeUI*	pInstance = new lifeUI(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created lifeUI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * lifeUI::Clone(void * pArg)
{
	lifeUI*	pInstance = new lifeUI(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created lifeUI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void lifeUI::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
