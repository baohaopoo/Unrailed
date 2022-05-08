#include "stdafx.h"
#include "..\Public\StartBtn.h"
#include "GameInstance.h"
#include "Level_Loading.h"

StartBtn::StartBtn(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

StartBtn::StartBtn(const StartBtn & rhs)
	: CGameObject(rhs)
{
}

HRESULT StartBtn::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT StartBtn::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	D3DXCreateFont(m_pGraphic_Device, 30, 10, 920, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"오이체", &font);
	m_pTransformCom->Scaled(_float3(0.6f, 0.6f, 0.6f));


	//로비용

	D3DXCreateFont(m_pGraphic_Device, 99, 49, 920, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"오이체", &font2);

	//성욱이의 회전목마

	D3DXCreateFont(m_pGraphic_Device, 30, 11, 920, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"오이체", &font3);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	CTransform*		terrain = (CTransform*)pGameInstance->Get_Component(LEVEL_LOBBY, TEXT("Layer_BackGround"), TEXT("Com_Transform"));
	_float3 terrainpos = terrain->Get_State(CTransform::STATE_POSITION);


	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, { terrainpos.x-1.f, terrainpos.y, terrainpos.z +0.4f });


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 0.f, 0.f, terrainpos.z -1.99f });
	return S_OK;

}

void StartBtn::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();




	CTransform*		pTargetTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_LOBBY, TEXT("Layer_UIObject"), TEXT("Com_Transform"));
	if (nullptr == pTargetTransform)
		return;


	//상대와 나의 radius, center값 넣어줘야함.
	m_isCol = CheckIntersect(pTargetTransform->Get_State(CTransform::STATE_POSITION), 0.5f, m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.5f); //타겟 center, 타겟 반지름값. );


	if (true == m_isCol)
	{
		//충돌되면
		/*if (!m_bStartLoading) {*/
			if (FAILED(Ready_Layer_ProgressBar(TEXT("Layer_ProgressBar"))))
				return;
			/*m_bStartLoading = true;*/
		//}
	}
	else
	{

	}


}

void StartBtn::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_UI, this);
}

HRESULT StartBtn::Render()
{
	//if (m_isDraw) {
	//	if (FAILED(__super::Render()))
	//		return E_FAIL;

	//	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(0)))
	//		return E_FAIL;

	//	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	//		return E_FAIL;




	//	m_pVIBufferCom->Render();

	//	//D3DXCreateSphere(m_pGraphic_Device, 1.0f, 15, 15, &mesh, 0);
	//	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//	//mesh->DrawSubset(0);
	//	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);



	//}


	//글꼴
	char string2[100] = "게임모드 \n이곳에 서주세요";
	RECT rt2 = { 740,450,900,650 };
	font->DrawTextA(NULL, string2, -1, &rt2, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));


	char string1[100] = "로비";
	RECT rt1 = { 100,100,300,300 };
	font2->DrawTextA(NULL, string1, -1, &rt1, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));



	char string3[300] = "COPRODUCER\n\n현우는 수석\n마취총을맞은 주현이\n비버의 삶 민희\n성욱이의 회전목마";
	RECT rt3 = { 100,450,300,650 };
	font3->DrawTextA(NULL, string3, -1, &rt3, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));



	return S_OK;
}

HRESULT StartBtn::Ready_Layer_ProgressBar(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOBBY, pLayerTag, TEXT("Prototype_GameObject_ProgressBar"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT StartBtn::SetUp_Components()
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

	//너때문에 고생오지게 함.
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sky"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	return S_OK;
}

bool StartBtn::CheckIntersect(_float3 center1, float radius1, _float3 center2, float radius2)
{
	float fDist;
	_float3 vSub;

	vSub = center2 - center1;
	fDist = D3DXVec3Length(&vSub);
	if (fDist <= (radius1 + radius2))
		return true;

	return false;
}

StartBtn * StartBtn::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	StartBtn*	pInstance = new StartBtn(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created StartBtn"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * StartBtn::Clone(void * pArg)
{
	StartBtn*	pInstance = new StartBtn(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created StartBtn"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void StartBtn::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
