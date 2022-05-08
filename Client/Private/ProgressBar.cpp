#include "stdafx.h"
#include "..\Public\ProgressBar.h"
#include "GameInstance.h"
#include "..\Public\Level_GamePlay.h"
#include "SoundMgr.h"
ProgressBar::ProgressBar(LPDIRECT3DDEVICE9 pGrahpic_Device)
	:CGameObject(pGrahpic_Device)
{
}

ProgressBar::ProgressBar(const ProgressBar & rhs)
	: CGameObject(rhs)
{
}

HRESULT ProgressBar::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT ProgressBar::NativeConstruct(void * pArg)
{

	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	D3DXCreateFont(m_pGraphic_Device, 15, 7, 920, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"오이체", &font);

	m_fSizeX = 1.f;
	m_fSizeY = 13.f;
	m_fX = 740.0f;
	m_fY = 540.0f;

	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinCX, g_iWinCY, 0.f, 1.f);

//	m_pTransformCom->Scaled(_float3(1.f, 1.f, 1.f));

	return S_OK;
}

void ProgressBar::Tick(_float fTimeDelta)
{


	__super::Tick(fTimeDelta);


	//Exitbtn 트랜스폼 얻어와
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	CTransform*		progressTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_LOBBY, TEXT("Layer_LobbyBtn"), TEXT("Com_Transform"));
	_float3 btnpos = progressTransform->Get_State(CTransform::STATE_POSITION);


	m_pTransformCom->Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinCX * 0.5f, -m_fY + g_iWinCY * 0.5f, 0.0f));
	CTransform*		pTargetTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_LOBBY, TEXT("Layer_UIObject"), TEXT("Com_Transform"));
	if (nullptr == pTargetTransform)
		return;


	//상대와 나의 radius, center값 넣어줘야함.
	m_isCol = CheckIntersect(pTargetTransform->Get_State(CTransform::STATE_POSITION), 1.f, progressTransform->Get_State(CTransform::STATE_POSITION),1.f); //타겟 center, 타겟 반지름값. );


	if (m_isCol)
	{
		m_isRender = true; //충돌했을때만 보여라.

		if (m_fSizeX < 300.f)
		{
			m_fSizeX += 1.3f;
			CSoundMgr::Get_Instance()->PlaySound(TEXT("progress_Bar.mp3"), CSoundMgr::CHANNELID::PROGRESS_BAR, SOUND_DEFAULT);
			if (m_fSizeX >= 300.f)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PROGRESS_BAR);
			}
		}
	}

	else
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PROGRESS_BAR);
		if (m_fSizeX >= 1.f)
			m_fSizeX -= 4.f;

	}





}

void ProgressBar::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	m_pRendererCom->Add_RenderGroup_UI(this);
}

HRESULT ProgressBar::Render()
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

	m_pVIBufferCom->Render();

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &OriginView);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &OriginProj);

	return S_OK;
}

void ProgressBar::LoadingGo()
{
	m_isGo = true;
}



HRESULT ProgressBar::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_UI"), (CComponent**)&m_pVIBufferCom, nullptr)))
		return E_FAIL;

	/* For.Com_Texture */
	//if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_LOADING, TEXT("	Prototype_Component_Texture_Default"), (CComponent**)&m_pTextureCom, nullptr)))
	//	return E_FAIL;

	//너때문에 고생오지게 함.
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_ProgressBar"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	return S_OK;
}

ProgressBar * ProgressBar::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	ProgressBar*	pInstance = new ProgressBar(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created ProgressBar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * ProgressBar::Clone(void * pArg)
{
	ProgressBar*	pInstance = new ProgressBar(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created ProgressBar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void ProgressBar::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}

bool ProgressBar::CheckIntersect(_float3 center1, float radius1, _float3 center2, float radius2)
{
	float fDist;
	_float3 vSub;

	vSub = center2 - center1;
	fDist = D3DXVec3Length(&vSub);
	if (fDist <= (radius1 + radius2))
		return true;

	return false;
}
