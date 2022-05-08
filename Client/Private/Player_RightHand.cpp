#include "stdafx.h"
#include "..\Public\Player_RightHand.h"
#include "GameInstance.h"
#include "GameObject.h"

CPlayer_RightHand::CPlayer_RightHand(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
	, m_eDir(DIRECTION::DIR_FRONT)
{
}

CPlayer_RightHand::CPlayer_RightHand(const CPlayer_RightHand & rhs)
	: CGameObject(rhs)
{
	m_eDir = rhs.m_eDir;
}

HRESULT CPlayer_RightHand::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer_RightHand::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	m_pTransformCom->Scaled(_float3(0.07, 0.07, 0.07));

	return S_OK;
}

void CPlayer_RightHand::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));
	_float3		vPlayerPosition = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, pPlayerTransform->Get_State(CTransform::STATE_RIGHT));
	m_pTransformCom->Set_State(CTransform::STATE_UP, pPlayerTransform->Get_State(CTransform::STATE_UP));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, pPlayerTransform->Get_State(CTransform::STATE_LOOK));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { vPlayerPosition.x, vPlayerPosition.y, vPlayerPosition.z });
	_float3 vRightHandPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	if (iHandAnimation == 0)
	{
		timecontrol += fTimeDelta;
		if (timecontrol >= 0.2f)
		{
			timecontrol = 0.f;
		}

		if (timecontrol <= 0.0002f)
			vTempPosition = vRightHandPosition;


		if ((vRightHandPosition.x != vTempPosition.x) && (vRightHandPosition.z == vTempPosition.z))		// 좌,우로 움직일 때의 발움직임
		{
			if (ObjectRotate == false)
			{
				ObjectTurn += 0.7f;
				vRightHandPosition.y = vPlayerPosition.y;
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, vRightHandPosition);
				if (ObjectTurn >= 15.f)
				{
					ObjectRotate = true;
				}
			}

			else if (ObjectRotate)
			{
				ObjectTurn -= 0.7f;
				vRightHandPosition.y = vPlayerPosition.y;
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, vRightHandPosition);
				if (ObjectTurn <= -15.f)
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
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vRightHandPosition);

		}

		else if ((vRightHandPosition.x == vTempPosition.x) && (vRightHandPosition.z != vTempPosition.z))
		{
			if (ObjectRotate == false)
			{
				ObjectTurn += 0.7f;
				vRightHandPosition.y = vPlayerPosition.y;
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, vRightHandPosition);
				if (ObjectTurn >= 15.f)
				{
					ObjectRotate = true;
				}
			}

			else if (ObjectRotate)
			{
				ObjectTurn -= 0.7f;
				vRightHandPosition.y = vPlayerPosition.y;
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, vRightHandPosition);
				if (ObjectTurn <= -15.f)
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
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vRightHandPosition);
		}

		if ((vRightHandPosition.x == vTempPosition.x) && (vRightHandPosition.z == vTempPosition.z))
		{
			_float4x4 playermatrix = pPlayerTransform->Get_WorldMatrix();
			_float4x4 worldMatrix = m_pTransformCom->Get_WorldMatrix();
			_float3 vPos{};
			memcpy(&vPos, worldMatrix.m[3], sizeof(_float3));
			D3DXMatrixRotationX(&worldMatrix, D3DXToRadian(0.f));

			memcpy(worldMatrix.m[3], &vPos, sizeof(_float3));
			m_pTransformCom->Set_WorldMatrix(worldMatrix*playermatrix);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vRightHandPosition);
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////

	else if (((iHandAnimation == 1) || (iHandAnimation == 2)) && (m_isCol == true))
	{
		timecontrol += fTimeDelta;
		if (timecontrol >= 1.0f)
		{
			timecontrol = 0.f;
		}

		if (timecontrol <= 0.0002f)
			vTempPosition = vRightHandPosition;


		//if ((vRightHandPosition.x != vTempPosition.x) && (vRightHandPosition.z == vTempPosition.z))		// 좌,우로 움직일 때의 발움직임
		//{
		if (ObjectRotate == false)
		{
			ObjectTurn += 5.f;
			vRightHandPosition.y = vPlayerPosition.y;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vRightHandPosition);
			if (ObjectTurn >= 0.f)
			{
				ObjectRotate = true;
			}
		}

		else if (ObjectRotate)
		{
			ObjectTurn -= 5.f;
			vRightHandPosition.y = vPlayerPosition.y;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vRightHandPosition);
			if (ObjectTurn <= -90.f)
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
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vRightHandPosition);
	}

	else if (iHandAnimation == 3)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vRightHandPosition);
	}
}

void CPlayer_RightHand::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CPlayer_RightHand::Render()
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

HRESULT CPlayer_RightHand::SetUp_Components(void* pArg)
{

	/* For.Com_Transform */
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Object"), (CComponent**)&m_pVIBufferCom, TEXT("Frog_Right_hand.dat"))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_HyunWoo"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;


	return S_OK;
}

HRESULT CPlayer_RightHand::SetUp_OnTerrain()
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

HRESULT CPlayer_RightHand::Change_Hand(int num)
{

	Delete_Component(Component_VIBuffer);

	switch (num)
	{
	case 0:
		if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Object"), (CComponent**)&m_pVIBufferCom, TEXT("Frog_Right_hand.dat"))))
			return E_FAIL;
		break;

	case 1:
		if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Object"), (CComponent**)&m_pVIBufferCom, TEXT("Right_hand_work_AxeDown.dat"))))
			return E_FAIL;
		break;

	case 2:
		if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Object"), (CComponent**)&m_pVIBufferCom, TEXT("Right_hand_work_PickAxeDown.dat"))))
			return E_FAIL;
		break;

	case 3:
		if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Object"), (CComponent**)&m_pVIBufferCom, TEXT("Two_hand work_Bucket.dat"))))
			return E_FAIL;
		break;

	case 4:
		if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Object"), (CComponent**)&m_pVIBufferCom, TEXT("Two_hand work_Wood.dat"))))
			return E_FAIL;
		break;

	case 5:
		if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Object"), (CComponent**)&m_pVIBufferCom, TEXT("Two_hand work_Iron.dat"))))
			return E_FAIL;
		break;

	case 6: //dynamite드는 손.
		if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Object"), (CComponent**)&m_pVIBufferCom, TEXT("Two_hand work_Dynamite.dat"))))
			return E_FAIL;

	case 7:
		if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Object"), (CComponent**)&m_pVIBufferCom, TEXT("Two_hand work_Rail1.dat"))))
			return E_FAIL;
		break;

	case 8:
		if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Object"), (CComponent**)&m_pVIBufferCom, TEXT("Two_hand work_Rail2.dat"))))
			return E_FAIL;
		break;

	case 9:
		if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Object"), (CComponent**)&m_pVIBufferCom, TEXT("Two_hand work_Rail3.dat"))))
			return E_FAIL;
		break;

		
		break;


	default:
		return E_FAIL;

	}



	//if (num == 0)
	//{
	//	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Object"), (CComponent**)&m_pVIBufferCom, TEXT("Frog_Right_hand.dat"))))
	//		return E_FAIL;
	//}
	//else if (num == 1)
	//{
	//	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Object"), (CComponent**)&m_pVIBufferCom, TEXT("Right_hand_work_AxeDown.dat"))))
	//		return E_FAIL;
	//}
	//else if (num == 2)
	//{
	//	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Object"), (CComponent**)&m_pVIBufferCom, TEXT("Right_hand_work_PickAxeDown.dat"))))
	//		return E_FAIL;
	//}
	//else if (num == 3) //버킷
	//{
	//	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Object"), (CComponent**)&m_pVIBufferCom, TEXT("Two_hand work_Bucket.dat"))))
	//		return E_FAIL;
	//}
	//else if (num == 4) //wood
	//{
	//	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Object"), (CComponent**)&m_pVIBufferCom, TEXT("Two_hand work_Wood.dat"))))
	//		return E_FAIL;
	//}
	//else if (num == 5) //Iron
	//{
	//	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Object"), (CComponent**)&m_pVIBufferCom, TEXT("Two_hand work_Iron.dat"))))
	//		return E_FAIL;
	//}
	return S_OK;
}


HRESULT CPlayer_RightHand::HandAnimation(int num)
{
	switch (num)
	{
	case 0:
		iHandAnimation = 0;
		break;
	case 1:
		iHandAnimation = 1;
		break;

	case 2:
		iHandAnimation = 2;
		break;

	case 3:
		iHandAnimation = 3;		// 하고 나서 Tick함수에서 흔들지 않고 아무것도 안하도록 한다.
		break;

	default:
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CPlayer_RightHand::PlayerCol(bool col)
{
	m_isCol = col;

	return S_OK;
}


CPlayer_RightHand * CPlayer_RightHand::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer_RightHand*	pInstance = new CPlayer_RightHand(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CPlayer_RightHand"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayer_RightHand::Clone(void * pArg)
{
	CPlayer_RightHand*	pInstance = new CPlayer_RightHand(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CPlayer_RightHand"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_RightHand::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
