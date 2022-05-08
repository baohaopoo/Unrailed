#include "stdafx.h"
#include "..\Public\MenuBtn.h"
#include "GameInstance.h"
#include "Player.h"

MenuBtn::MenuBtn(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

MenuBtn::MenuBtn(const MenuBtn & rhs)
	: CGameObject(rhs)
{
}

HRESULT MenuBtn::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT MenuBtn::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;


	if (FAILED(SetUp_Components()))
		return E_FAIL;




	m_fSizeX = 180.f;
	m_fSizeY = 40.f;
	m_fX = 120.0f;
	m_fY = 110.0f;
	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinCX, g_iWinCY, 0.f, 1.f);
	m_pTransformCom->Scaled(_float3(1.f, 1.f, 1.f));
	return S_OK;

}

void MenuBtn::Tick(_float fTimeDelta)
{

	__super::Tick(fTimeDelta);


	//피킹을 위한..
	RECT rc{ 60,110,200,120 };
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);


	if (PtInRect(&rc, pt) && GetAsyncKeyState(VK_LBUTTON)) {


		if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
			return;

	}

	m_pTransformCom->Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinCX * 0.5f, -m_fY + g_iWinCY * 0.5f, 0.0f));

	timecontrol += fTimeDelta;

	if (timecontrol > 1) {

		m_pTransformCom->Scaled(_float3(m_fSizeX + 4, m_fSizeY + 4, 1.f));

	}
	if (timecontrol > 2) {

		m_pTransformCom->Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
		timecontrol = 0.f;
	}
}

HRESULT MenuBtn::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_MenuPoP"))))
		return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Sky"))))
	//	return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}



void MenuBtn::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup_UI(this);
}

HRESULT MenuBtn::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	//미션 버튼 업데이트 
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	CLayer* pLLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Player"));
	CPlayer* player = dynamic_cast<CPlayer*>(*pLLayer->Get_ObjectList().begin());

	if (!player->MissionBtnUpdate())
	{
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
	if (player->MissionBtnUpdate()) {
		if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(1)))
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

HRESULT MenuBtn::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;

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

MenuBtn * MenuBtn::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	MenuBtn*	pInstance = new MenuBtn(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created MenuBtn"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * MenuBtn::Clone(void * pArg)
{
	MenuBtn*	pInstance = new MenuBtn(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created MenuBtn"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void MenuBtn::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
