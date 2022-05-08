#include "stdafx.h"
#include "..\Public\Giant_RightHand.h"
#include "GameInstance.h"

CGiant_RightHand::CGiant_RightHand(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
	m_eType = OBJECT_TYPE::OBJECT_GIANTRIGHTHAND;
}

CGiant_RightHand::CGiant_RightHand(const CGiant_RightHand & rhs)
	: CGameObject(rhs)
{

	m_eType = rhs.m_eType;
}

HRESULT CGiant_RightHand::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGiant_RightHand::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CGiant_RightHand::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	fTime += fTimeDelta;
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	CTransform*		pGiantBodyTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Giant"), TEXT("Com_Transform"));
	_float3		vLeftHandPosition = pGiantBodyTransform->Get_State(CTransform::STATE_POSITION);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { vLeftHandPosition.x, vLeftHandPosition.y, vLeftHandPosition.z });


	if (fTime > 1.f && fTime<23.f)
	{
		if (ObjectRotate == false)
		{
			ObjectTurn += 0.2f*fTimeDelta*55.f;
			if (ObjectTurn >= 0.f)
			{
				ObjectRotate = true;
			}
		}

		if (ObjectRotate)
		{
			ObjectTurn -= 0.2f*fTimeDelta*55.f;
			if (ObjectTurn <= -40.f)
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

	else if (fTime >= 60.f && fTime < 65.5f)							//팔 바깥쪽으로 피기
	{
		m_pTransformCom->Turned(_float3(0.f, 1.f, 0.f), fRightTurn*fTimeDelta*55.f);
	}

	else if (fTime >= 65.5f && fTime < 65.8f)
	{
		ObjectTurn = -90.f;
	}

	else if (fTime >= 66.f && fTime < 75.f)
	{
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
	}
}

void CGiant_RightHand::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	m_pRendererCom->Add_RenderGroup_NonCulling(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CGiant_RightHand::Render()
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

HRESULT CGiant_RightHand::SetUp_Components(void* pArg)
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

	if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("GiantRightHand.dat"))))
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

CGiant_RightHand * CGiant_RightHand::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGiant_RightHand*	pInstance = new CGiant_RightHand(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created Giant_RightHand"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CGiant_RightHand::Clone(void * pArg)
{
	CGiant_RightHand*	pInstance = new CGiant_RightHand(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created Clone Giant_RightHand"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGiant_RightHand::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}

