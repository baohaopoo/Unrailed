#include "stdafx.h"
#include "..\Public\Giant_LeftHand.h"
#include "GameInstance.h"
#include "SoundMgr.h"
CGiant_LeftHand::CGiant_LeftHand(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
	m_eType = OBJECT_TYPE::OBJECT_GIANTLEFTHAND;
}

CGiant_LeftHand::CGiant_LeftHand(const CGiant_LeftHand & rhs)
	: CGameObject(rhs)
{

	m_eType = rhs.m_eType;
}

HRESULT CGiant_LeftHand::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGiant_LeftHand::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CGiant_LeftHand::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	fTime += fTimeDelta;
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	CTransform*		pGiantBodyTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Giant"), TEXT("Com_Transform"));
	_float3		vLeftHandPosition = pGiantBodyTransform->Get_State(CTransform::STATE_POSITION);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { vLeftHandPosition.x, vLeftHandPosition.y, vLeftHandPosition.z });


	if (fTime > 1.f && fTime<23.f)						// 어깨 들썩 들썩
	{
		if (ObjectRotate == false)
		{
			ObjectTurn += 0.2f*fTimeDelta*55.f;
			if (ObjectTurn >= 40.f)
			{
				ObjectRotate = true;
			}
		}

		else
		{
			ObjectTurn -= 0.2f*fTimeDelta*55.f;
			if (ObjectTurn <= 0.f)
			{
				ObjectRotate = false;
			}
		}

		_float4x4 LeftHandMatrix = pGiantBodyTransform->Get_WorldMatrix();
		_float4x4 worldMatrix = m_pTransformCom->Get_WorldMatrix();
		_float3 vPos{};
		memcpy(&vPos, worldMatrix.m[3], sizeof(_float3));
		D3DXMatrixRotationZ(&worldMatrix, D3DXToRadian(ObjectTurn));
		//D3DXMatrixRotationX(&worldMatrix, D3DXToRadian(ObjectTurn));
		memcpy(worldMatrix.m[3], &vPos, sizeof(_float3));
		m_pTransformCom->Set_WorldMatrix(worldMatrix*LeftHandMatrix);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vLeftHandPosition);
	}


	else if (fTime >= 24.f && fTime < 24.5f)
	{
		ObjectTurn = 0.f;
	}

	else if (fTime >= 24.5f && fTime <33.f)			// 손 앞으로 흔들기
	{
		if (ObjectRotate == false)
		{
			ObjectTurn -= 0.2f*fTimeDelta*55.f;
			if (ObjectTurn <= -90.f)
			{
				ObjectRotate = true;
			}
		}

		else
		{
			ObjectTurn += 0.2f*fTimeDelta*55.f;
			if (ObjectTurn >= 0.f)
			{
				ObjectRotate = false;
			}
		}

		_float4x4 LeftHandMatrix = pGiantBodyTransform->Get_WorldMatrix();
		_float4x4 worldMatrix = m_pTransformCom->Get_WorldMatrix();
		_float3 vPos{};
		memcpy(&vPos, worldMatrix.m[3], sizeof(_float3));
		D3DXMatrixRotationX(&worldMatrix, D3DXToRadian(ObjectTurn));
		memcpy(worldMatrix.m[3], &vPos, sizeof(_float3));
		m_pTransformCom->Set_WorldMatrix(worldMatrix*LeftHandMatrix);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vLeftHandPosition);
	}

	else if (fTime >= 34.5f && fTime<35.f)						// 손 앞으로 뻗어있는 각도 세팅(90도)
	{
		_float4x4 LeftHandMatrix = pGiantBodyTransform->Get_WorldMatrix();
		_float4x4 worldMatrix = m_pTransformCom->Get_WorldMatrix();
		_float3 vPos{};
		memcpy(&vPos, worldMatrix.m[3], sizeof(_float3));
		//D3DXMatrixRotationZ(&worldMatrix, D3DXToRadian(90.f));
		//D3DXMatrixRotationZ(&TransDesc.m_WorldMatrix, D3DXToRadian(90.f));
		//memcpy(worldMatrix.m[3], &vPos, sizeof(_float3));
		D3DXMatrixRotationX(&worldMatrix, D3DXToRadian(-90.f));
		memcpy(worldMatrix.m[3], &vPos, sizeof(_float3));
		m_pTransformCom->Set_WorldMatrix(worldMatrix*LeftHandMatrix);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vLeftHandPosition);
	}

	else if (fTime >= 38.f && fTime < 43.f)							// 팔 안쪽으로 굽히기
	{
		m_pTransformCom->Turned(_float3(0.f, 1.f, 0.f), fLeftTurn*fTimeDelta*55.f);
	}

	else if (fTime >= 43.f && fTime < 60.f)
	{
		++createMeteo;
		if (createMeteo < 3)
		{
			CTransform::TRANSFORMDESC	TransDesc;
			pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_MeteoGiant"), TEXT("Prototype_GameObject_MeteoGiant"), &TransDesc);
		}
	}

	else if (fTime >= 60.f && fTime < 65.5f)							//팔 바깥쪽으로 피기
	{
		m_pTransformCom->Turned(_float3(0.f, 1.f, 0.f), fRightTurn*fTimeDelta*55.f);
	}

	else if (fTime >= 65.5f && fTime < 65.8f)
	{
		ObjectTurn = -90.f;
	}

	else if (fTime >= 66.f && fTime < 75.f)							//팔 들어올리기
	{

		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::WARNING);
		CSoundMgr::Get_Instance()->PlaySound(TEXT("warning.mp3"), CSoundMgr::CHANNELID::WARNING, SOUND_MAX);
		if (ObjectRotate == false)
		{
			ObjectTurn -= 0.2f*fTimeDelta*55.f;
			if (ObjectTurn <= -180.f)
			{
				ObjectRotate = true;
			}
		}

		else
		{
			ObjectTurn += 0.2f*fTimeDelta*55.f;
			if (ObjectTurn >= -90.f)
			{
				ObjectRotate = false;
			}
		}

		_float4x4 LeftHandMatrix = pGiantBodyTransform->Get_WorldMatrix();
		_float4x4 worldMatrix = m_pTransformCom->Get_WorldMatrix();
		_float3 vPos{};
		memcpy(&vPos, worldMatrix.m[3], sizeof(_float3));
		D3DXMatrixRotationX(&worldMatrix, D3DXToRadian(ObjectTurn));
		memcpy(worldMatrix.m[3], &vPos, sizeof(_float3));
		m_pTransformCom->Set_WorldMatrix(worldMatrix*LeftHandMatrix);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vLeftHandPosition);
	}


	else if (fTime >= 75.f && fTime < 75.3f)
	{

		ObjectTurn = -180.f;
	}

	else if (fTime >= 75.3f && fTime < 90.3f)
	{
		fMeteo += fTimeDelta;
		fMeteoDestroy += fTimeDelta;
		if (fMeteo >= 3.f)
		{
			for (int i = 0; i < 3; ++i)
			{
				CTransform::TRANSFORMDESC	TransDesc;
				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Meteo"), TEXT("Prototype_GameObject_Meteo"), &TransDesc);
			}
			fMeteo = 0.f;
		}

		if (/*fMeteoDestroy*/fTime >= 83.f && fTime < 86.f)
		{
			//CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EXPLO1);
			//CSoundMgr::Get_Instance()->PlaySound(TEXT("explo.mp3"), CSoundMgr::CHANNELID::EXPLO, 0.9f);
		}

		if (/*fMeteoDestroy*/ fTime >= 87.f && fTime < 90.f)
		{
			//CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EXPLO);
			//CSoundMgr::Get_Instance()->PlaySound(TEXT("explo.mp3"), CSoundMgr::CHANNELID::EXPLO1, 0.9f);
		}

		//if (fMeteoDestroy >= 15.5f)
		//{
		//	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EXPLO);
		//	CSoundMgr::Get_Instance()->PlaySound(TEXT("explo.mp3"), CSoundMgr::CHANNELID::EXPLO, 0.9f);
		//}

	}


	else if (fTime >= 90.3f && fTime < 94.f)				// 팔 내리기
	{
		if (ObjectRotate == false)
		{
			ObjectTurn -= 0.8f*fTimeDelta*55.f;
			if (ObjectTurn <= -180.f)
			{
				ObjectRotate = true;
			}
		}

		else
		{
			ObjectTurn += 0.8f*fTimeDelta*55.f;
			if (ObjectTurn >= 0.f)
			{
				ObjectRotate = false;
			}
		}

		_float4x4 LeftHandMatrix = pGiantBodyTransform->Get_WorldMatrix();
		_float4x4 worldMatrix = m_pTransformCom->Get_WorldMatrix();
		_float3 vPos{};
		memcpy(&vPos, worldMatrix.m[3], sizeof(_float3));
		D3DXMatrixRotationX(&worldMatrix, D3DXToRadian(ObjectTurn));
		memcpy(worldMatrix.m[3], &vPos, sizeof(_float3));
		m_pTransformCom->Set_WorldMatrix(worldMatrix*LeftHandMatrix);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vLeftHandPosition);
	}


	else if (fTime > 100.f)				// 반복되게 하는 코드. 시간을 0.f로 초기화해서 다시한다.
	{
		ObjectTurn = 0.f;
		fTime = 0.f;
		createMeteo = 0;
	}
}

void CGiant_LeftHand::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	m_pRendererCom->Add_RenderGroup_NonCulling(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CGiant_LeftHand::Render()
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

HRESULT CGiant_LeftHand::SetUp_Components(void* pArg)
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

	TransDesc.fSpeedPerSec = 0.3f;
	TransDesc.fRotationPerSec = D3DXToRadian(10.0f);
	TransDesc.fScalePerSec = 1.f;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(Component_Renderer, LEVEL_STATIC, Prototype_Compo_Renderer, (CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	D3DXMatrixRotationY(&TransDesc.m_WorldMatrix, D3DXToRadian(180.f));
	// 배율
	/*_float fScale = 30.f;

	TransDesc.m_WorldMatrix._11 /= fScale;
	TransDesc.m_WorldMatrix._12 /= fScale;
	TransDesc.m_WorldMatrix._13 /= fScale;

	TransDesc.m_WorldMatrix._21 /= fScale;
	TransDesc.m_WorldMatrix._22 /= fScale;
	TransDesc.m_WorldMatrix._23 /= fScale;

	TransDesc.m_WorldMatrix._31 /= fScale;
	TransDesc.m_WorldMatrix._32 /= fScale;
	TransDesc.m_WorldMatrix._33 /= fScale;*/

	//memcpy(TransDesc.m_WorldMatrix.m[3], &vPos, sizeof(_float3));

	if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("GiantLeftHand.dat"))))
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

CGiant_LeftHand * CGiant_LeftHand::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGiant_LeftHand*	pInstance = new CGiant_LeftHand(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created Giant_LeftHand"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CGiant_LeftHand::Clone(void * pArg)
{
	CGiant_LeftHand*	pInstance = new CGiant_LeftHand(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created Clone Giant_LeftHand"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGiant_LeftHand::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
