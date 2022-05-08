#include "stdafx.h"
#include "..\Public\ExitBtn.h"
#include "GameInstance.h"

ExitBtn::ExitBtn(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

ExitBtn::ExitBtn(const ExitBtn & rhs)
	: CGameObject(rhs)
{
}

HRESULT ExitBtn::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT ExitBtn::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	D3DXCreateFont(m_pGraphic_Device, 30, 10, 920, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"����ü", &font);

	m_pTransformCom->Scaled(_float3(1.f, 0.4f, 0.4f));

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	CTransform*		terrain = (CTransform*)pGameInstance->Get_Component(LEVEL_LOBBY, TEXT("Layer_BackGround"), TEXT("Com_Transform"));
	_float3 terrainpos = terrain->Get_State(CTransform::STATE_POSITION);


	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, { terrainpos.x-1, terrainpos.y, terrainpos.z-1.1f });

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 2.5f, terrainpos.y, terrainpos.z - 1.f });

	return S_OK;
}

void ExitBtn::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CTransform*		pTargetTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_LOBBY, TEXT("Layer_UIObject"), TEXT("Com_Transform"));
	if (nullptr == pTargetTransform)
		return;

	//���� ���� radius, center�� �־������.
	m_isCol = CheckIntersect(pTargetTransform->Get_State(CTransform::STATE_POSITION), 1.f, m_pTransformCom->Get_State(CTransform::STATE_POSITION), 1.f); //Ÿ�� center, Ÿ�� ��������. );

	if (true == m_isCol)
	{
		if (FAILED(Ready_Layer_ProgressBarExit(TEXT("Layer_ProgressBarExit"))))
			return;
	}
	else
	{

	}

	Safe_Release(pGameInstance);
}

void ExitBtn::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT ExitBtn::Render()
{
	//if (m_isDraw)
	//{
	//	if (FAILED(__super::Render()))
	//		return E_FAIL;

	//	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(0)))
	//		return E_FAIL;

	//	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	//		return E_FAIL;

	//	//�۲�
	//	char string1[100] = "������ \n�̰��� ���ּ���";
	//	RECT rt1 = { 740,270,900,380 };
	//	font->DrawTextA(NULL, string1, -1, &rt1, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));


	//	m_pVIBufferCom->Render();

		//D3DXCreateSphere(m_pGraphic_Device, 1.0f, 15, 15, &mesh, 0);
		//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		//mesh->DrawSubset(0);
		//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);


	

	//�۲�
	char string1[100] = "������ \n�̰��� ���ּ���";
	RECT rt1 = { 740,270,900,380 };
	font->DrawTextA(NULL, string1, -1, &rt1, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));


	return S_OK;
}

HRESULT ExitBtn::Ready_Layer_ProgressBarExit(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_ProgressBarExit"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT ExitBtn::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), (CComponent**)&m_pVIBufferCom, nullptr)))
		return E_FAIL;



	//�ʶ����� ��������� ��.
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sky"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	return S_OK;
}

bool ExitBtn::CheckIntersect(_float3 center1, float radius1, _float3 center2, float radius2)
{
	float fDist;
	_float3 vSub;

	vSub = center2 - center1;
	fDist = D3DXVec3Length(&vSub);
	if (fDist <= (radius1 + radius2))
		return true;

	return false;
}

ExitBtn * ExitBtn::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	ExitBtn*	pInstance = new ExitBtn(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created ExitBtn"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * ExitBtn::Clone(void * pArg)
{
	ExitBtn*	pInstance = new ExitBtn(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created ExitBtn"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void ExitBtn::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
