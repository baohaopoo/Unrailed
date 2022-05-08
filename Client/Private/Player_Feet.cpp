#include "stdafx.h"
#include "..\Public\Player_Feet.h"
#include "GameInstance.h"

CPlayer_Feet::CPlayer_Feet(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
	, m_eDir(DIRECTION::DIR_FRONT)
{
}

CPlayer_Feet::CPlayer_Feet(const CPlayer_Feet & rhs)
	: CGameObject(rhs)
{
	m_eDir = rhs.m_eDir;
}

HRESULT CPlayer_Feet::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer_Feet::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	m_pTransformCom->Scaled(_float3(0.001f, 0.001f, 0.001f));

	return S_OK;
}

void CPlayer_Feet::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	timecontrol += fTimeDelta;
	if (timecontrol >= 0.2f)
	{
		timecontrol = 0.f;
	}
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));
	_float3		vPlayerPosition = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, pPlayerTransform->Get_State(CTransform::STATE_RIGHT));
	//m_pTransformCom->Set_State(CTransform::STATE_UP, pPlayerTransform->Get_State(CTransform::STATE_UP));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, pPlayerTransform->Get_State(CTransform::STATE_LOOK));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { vPlayerPosition.x, vPlayerPosition.y, vPlayerPosition.z });
	_float3 vFeetPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	if (timecontrol <= 0.00001f)
		vTempPosition = vFeetPosition;


	if ((vFeetPosition.x != vTempPosition.x) && (vFeetPosition.z == vTempPosition.z))		// 좌,우로 움직일 때의 발움직임
	{
		if (ObjectRotate == false)
		{
			ObjectTurn += 0.5f;
			//vFeetPosition.x = vPlayerPosition.x - 20.f;
			vFeetPosition.y = vPlayerPosition.y;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vFeetPosition);
			if (ObjectTurn >= 20.f)
			{
				ObjectRotate = true;
			}
		}

		else if (ObjectRotate)
		{
			ObjectTurn -= 0.5f;
			//vFeetPosition.x = vPlayerPosition.x - 20.f;
			vFeetPosition.y = vPlayerPosition.y;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vFeetPosition);
			if (ObjectTurn <= -30.f)
			{
				ObjectRotate = false;
			}
		}

		_float4x4 playermatrix = pPlayerTransform->Get_WorldMatrix();
		_float4x4 worldMatrix = m_pTransformCom->Get_WorldMatrix();
		_float3 vPos{};
		memcpy(&vPos, worldMatrix.m[3], sizeof(_float3));
		D3DXMatrixRotationX(&worldMatrix, D3DXToRadian(ObjectTurn));

		memcpy(worldMatrix.m[3], &vPos, sizeof(_float3));
		m_pTransformCom->Set_WorldMatrix(worldMatrix*playermatrix);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vFeetPosition);

	}

	else if ((vFeetPosition.x == vTempPosition.x) && (vFeetPosition.z != vTempPosition.z))		// 상, 하로 움직일 때 발 움직임
	{
		if (ObjectRotate == false)
		{
			ObjectTurn += 0.5f;
			vFeetPosition.y = vPlayerPosition.y;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vFeetPosition);
			if (ObjectTurn >= 20.f)
			{
				ObjectRotate = true;
			}
		}

		else if (ObjectRotate)
		{
			ObjectTurn -= 0.5f;
			vFeetPosition.y = vPlayerPosition.y;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vFeetPosition);
			if (ObjectTurn <= -30.f)
			{
				ObjectRotate = false;
			}
		}

		_float4x4 playermatrix = pPlayerTransform->Get_WorldMatrix();
		_float4x4 worldMatrix = m_pTransformCom->Get_WorldMatrix();
		_float3 vPos{};
		memcpy(&vPos, worldMatrix.m[3], sizeof(_float3));
		D3DXMatrixRotationX(&worldMatrix, D3DXToRadian(ObjectTurn));

		memcpy(worldMatrix.m[3], &vPos, sizeof(_float3));
		m_pTransformCom->Set_WorldMatrix(worldMatrix*playermatrix);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vFeetPosition);
	}

	if ((vFeetPosition.x == vTempPosition.x) && (vFeetPosition.z == vTempPosition.z))		// 멈춰있으면 발의 Rotate각도를 0.f로 설정
	{
		_float4x4 playermatrix = pPlayerTransform->Get_WorldMatrix();
		_float4x4 worldMatrix = m_pTransformCom->Get_WorldMatrix();
		_float3 vPos{};
		memcpy(&vPos, worldMatrix.m[3], sizeof(_float3));
		D3DXMatrixRotationX(&worldMatrix, D3DXToRadian(0.f));

		memcpy(worldMatrix.m[3], &vPos, sizeof(_float3));
		m_pTransformCom->Set_WorldMatrix(worldMatrix*playermatrix);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vFeetPosition);
	}
}

void CPlayer_Feet::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CPlayer_Feet::Render()
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

	//m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CPlayer_Feet::SetUp_Components(void* pArg)
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	//ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	TransformDesc.fSpeedPerSec = 8.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	TransformDesc.fScalePerSec = 1.f;

	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Object"), (CComponent**)&m_pVIBufferCom, TEXT("Frog_Right_Leg.dat"))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_HyunWoo"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;


	return S_OK;
}

HRESULT CPlayer_Feet::SetUp_OnTerrain()
{
	CGameInstance*			pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	_float3		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	CVIBuffer_Terrain*		pTerrainBuffer = (CVIBuffer_Terrain*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer"));
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	CTransform*		pTerrainTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Com_Transform"));
	if (nullptr == pTerrainTransform)

		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pTerrainBuffer->Compute_Height(pTerrainTransform, vPosition));


	Safe_Release(pGameInstance);
	return S_OK;
}


CPlayer_Feet * CPlayer_Feet::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer_Feet*	pInstance = new CPlayer_Feet(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CPlayer_Feet"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayer_Feet::Clone(void * pArg)
{
	CPlayer_Feet*	pInstance = new CPlayer_Feet(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CPlayer_Feet"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_Feet::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
