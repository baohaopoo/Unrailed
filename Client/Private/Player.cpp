#include "stdafx.h"
#include "..\Public\Player.h"
#include "GameInstance.h"
// ��ö�� ���
#include "Tree.h"
#include "Rock.h"
#include "Material_Iron.h"
#include "Material_Wood.h"
#include "Train_Material.h"
#include "Train_Rail.h"
#include "Train_Water.h"
#include "Player_RightHand.h"
#include "Player_LeftHand.h"
#include "SoundMgr.h"
#include "Effect_Explo.h"
#include "Effect_TrainSmoke.h"
#include "ToolUI.h"

#include "Rail.h"
#include "Rail_Invisible.h"
#include "Rail_Manager.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
	, m_eDir(DIRECTION::DIR_FRONT)
{
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject(rhs)
{
	m_eDir = rhs.m_eDir;
	m_eHoldingState = rhs.m_eHoldingState;
}

HRESULT CPlayer::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	m_pTransformCom->Scaled(_float3(0.025, 0.025, 0.025));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { -10.f, 0.98f, 5.f });



	D3DXCreateFont(m_pGraphic_Device, 18, 5, 920, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"����ü", &font);

	D3DXCreateFont(m_pGraphic_Device, 18, 5, 920, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"����ü", &font2);




	return S_OK;
}

void CPlayer::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	
	if (!pGameInstance->Get_DIKeyState(DIK_P))
		m_bPressButton = false;

	ShowRailPos(fTimeDelta);

	// Rain
	Rain(fTimeDelta);

	// meteo control
	Meteo(fTimeDelta);

	// RailTrainBomb �Լ� 
	RailTrainBomb(fTimeDelta);

	// PressSpace �Լ� -  Tool �鋚��, ���� �浹 ��ȣ�ۿ� ��� �Լ�.
	pressSpace(fTimeDelta);


	//Player Control
	timecontrol += fTimeDelta;
	if (timecontrol >= 1.f)
		timecontrol = 1.f;

	PlayerMove(fTimeDelta);
}

void CPlayer::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	// ���⼭ ������Ʈ�� �о��.
	PushOut_Collider(fTimeDelta);


	//DIK_RETURN PRESS   �÷��̾ EnterŰ�� �������� ������ �Լ�.
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	if (!pGameInstance->Get_DIKeyState(DIK_RETURN))
	{
		m_bPressEnter = false;
	}

	CTransform::TRANSFORMDESC		TransformDesc;
	CGameObject* pColObject = m_pColliderCom->Get_CollisionObject();

	//returnŰ�� �ȴ������鼭, ��� �ִ� ������Ʈ�� �ְ�, �浹��ü�� �����鼭, ���� returnŰ�� �����ٸ�,
	if (!m_bPressEnter && holdObj != nullptr && pColObject == nullptr && pGameInstance->Get_DIKeyState(DIK_RETURN))
	{
		m_bPressEnter = true;
		pressReturn(fTimeDelta);
		return;
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CPlayer::Render()
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

HRESULT CPlayer::SetUp_Components(void* pArg)
{/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	//ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	TransformDesc.fSpeedPerSec = 4.f;
	/*TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);*/
	TransformDesc.fScalePerSec = 1.f;

	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Object"), (CComponent**)&m_pVIBufferCom, TEXT("Frog.dat"))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_HyunWoo"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	CCollider::ColliDesc colDesc;

	colDesc.pTransform = m_pTransformCom;

	/* For.Com_Collider */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Collider"), LEVEL_STATIC, TEXT("Prototype_Component_Collider"), (CComponent**)&m_pColliderCom, &colDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::SetUp_OnTerrain()
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

bool CPlayer::CheckIntersect(_float3 center1, float radius1, _float3 center2, float radius2)
{
	float fDist;
	_float3 vSub;

	vSub = center2 - center1;
	fDist = D3DXVec3Length(&vSub);
	if (fDist <= (radius1 + radius2))
		return true;

	return false;
}

void CPlayer::PushOut_Collider(_float _fDeltaTime)
{
	// ���⼭ ������Ʈ�� ũ�� ������ �о��.

	// �浹�� �� ������Ʈ�� �����´�.
	CGameObject* pColObject = m_pColliderCom->Get_CollisionObject();

	// ���� �浹�� ������Ʈ�� �����Ѵٸ�
	if (nullptr != pColObject) {
		// �ش��ϴ� ������Ʈ�� ������� �ѱ��.
		OBJECT_TYPE eType = pColObject->Get_Type();

		if (eType == OBJECT_TYPE::OBJECT_AXE ||
			eType == OBJECT_TYPE::OBJECT_BUCKET ||
			eType == OBJECT_TYPE::OBJECT_PICKAX ||
			eType == OBJECT_TYPE::OBJECT_RAIL_CURVE ||
			eType == OBJECT_TYPE::OBJECT_RAIL_STRAIGHT ||
			eType == OBJECT_TYPE::OBJECT_MATERIAL_IRON ||
			eType == OBJECT_TYPE::OBJECT_MATERIAL_WOOD)
		{
			;
		}
		// ������ �ƴ϶��
		else {
			_float3		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_float3		vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

			vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * m_pTransformCom->Get_TransformState().fSpeedPerSec * _fDeltaTime;

			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
		}
	}
}

void CPlayer::PlayerMove(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	CTransform::TRANSFORMDESC TransformDesc;
	if (pGameInstance->Get_DIKeyState(DIK_UP))
	{
		if (pGameInstance->Get_DIKeyState(DIK_LEFT))
		{
			m_pTransformCom->TurnAround(_float3(0.f, 1.f, 0.f), DIRECTION::DIR_FRONTLEFT);
			m_pTransformCom->Go_Straight(fTimeDelta / sqrtf(2.f));
		}
		else if (pGameInstance->Get_DIKeyState(DIK_RIGHT))
		{
			m_pTransformCom->TurnAround(_float3(0.f, 1.f, 0.f), DIRECTION::DIR_FRONTRIGHT);
			m_pTransformCom->Go_Straight(fTimeDelta / sqrtf(2.f));
		}
		else
		{
			m_pTransformCom->TurnAround(_float3(0.f, 1.f, 0.f), DIRECTION::DIR_FRONT);
			m_pTransformCom->Go_Straight(fTimeDelta);
		}



		if ((GetKeyState(VK_SHIFT) & 0x8000)/* && (timecontrol >= 0.f)*/)
		{
			if (timecontrol > 0.2f)
			{
				CEffect_TrainSmoke::TRAINSMOKEDESC TrainSmokeDesc;

				TrainSmokeDesc.eSmoke = CEffect_TrainSmoke::ST_DYNAMITE;
				TrainSmokeDesc.iTextureNumber = 37;
				TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][0] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).x;
				TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][1] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y - 0.5f;
				TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][2] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).z;

				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Effect_Smok_Dash"), TEXT("Prototype_GameObject_Effect_TrainSmoke"), &TrainSmokeDesc.TransDesc);

				timecontrol = 0.f;
			}

			TransformDesc.fSpeedPerSec = 70.f;
			m_pTransformCom->Go_Straight(fTimeDelta);
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DASH);
			CSoundMgr::Get_Instance()->PlaySound(TEXT("Dash.mp3"), CSoundMgr::CHANNELID::DASH, SOUND_DEFAULT);
			//timecontrol -= 0.1f;
		}
	}

	if (pGameInstance->Get_DIKeyState(DIK_DOWN))
	{
		if (pGameInstance->Get_DIKeyState(DIK_LEFT))
		{
			m_pTransformCom->TurnAround(_float3(0.f, 1.f, 0.f), DIRECTION::DIR_BACKLEFT);
			m_pTransformCom->Go_Straight(fTimeDelta / sqrtf(2.f));
		}
		else if (pGameInstance->Get_DIKeyState(DIK_RIGHT))
		{
			m_pTransformCom->TurnAround(_float3(0.f, 1.f, 0.f), DIRECTION::DIR_BACKRIGHT);
			m_pTransformCom->Go_Straight(fTimeDelta / sqrtf(2.f));
		}

		else
		{
			m_pTransformCom->TurnAround(_float3(0.f, 1.f, 0.f), DIRECTION::DIR_BACK);
			m_pTransformCom->Go_Straight(fTimeDelta);
		}

		if ((GetKeyState(VK_SHIFT) & 0x8000))
		{
			if (timecontrol > 0.2f)
			{
				CEffect_TrainSmoke::TRAINSMOKEDESC TrainSmokeDesc;

				TrainSmokeDesc.eSmoke = CEffect_TrainSmoke::ST_DYNAMITE;
				TrainSmokeDesc.iTextureNumber = 37;
				TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][0] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).x;
				TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][1] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y - 0.5f;
				TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][2] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).z;

				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Effect_Smok_Dash"), TEXT("Prototype_GameObject_Effect_TrainSmoke"), &TrainSmokeDesc.TransDesc);

				timecontrol = 0.f;
			}

			TransformDesc.fSpeedPerSec = 70.f;
			m_pTransformCom->Go_Straight(fTimeDelta);
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DASH);
			CSoundMgr::Get_Instance()->PlaySound(TEXT("Dash.mp3"), CSoundMgr::CHANNELID::DASH, SOUND_DEFAULT);
			//timecontrol -= 0.1f;
		}

	}

	if (pGameInstance->Get_DIKeyState(DIK_RIGHT))
	{
		if (pGameInstance->Get_DIKeyState(DIK_UP))
		{
			m_pTransformCom->TurnAround(_float3(0.f, 1.f, 0.f), DIRECTION::DIR_FRONTRIGHT);
			m_pTransformCom->Go_Straight(fTimeDelta / sqrtf(2.f));
		}


		else if (pGameInstance->Get_DIKeyState(DIK_DOWN))
		{
			m_pTransformCom->TurnAround(_float3(0.f, 1.f, 0.f), DIRECTION::DIR_BACKRIGHT);
			m_pTransformCom->Go_Straight(fTimeDelta / sqrtf(2.f));
		}

		else
		{
			m_pTransformCom->TurnAround(_float3(0.f, 1.f, 0.f), DIRECTION::DIR_RIGHT);
			m_pTransformCom->Go_Straight(fTimeDelta);
		}

		if ((GetKeyState(VK_SHIFT) & 0x8000))
		{
			if (timecontrol > 0.2f)
			{
				CEffect_TrainSmoke::TRAINSMOKEDESC TrainSmokeDesc;

				TrainSmokeDesc.eSmoke = CEffect_TrainSmoke::ST_DYNAMITE;
				TrainSmokeDesc.iTextureNumber = 37;
				TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][0] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).x;
				TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][1] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y - 0.5f;
				TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][2] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).z;

				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Effect_Smok_Dash"), TEXT("Prototype_GameObject_Effect_TrainSmoke"), &TrainSmokeDesc.TransDesc);

				timecontrol = 0.f;
			}
			TransformDesc.fSpeedPerSec = 70.f;
			m_pTransformCom->Go_Straight(fTimeDelta);
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DASH);
			CSoundMgr::Get_Instance()->PlaySound(TEXT("Dash.mp3"), CSoundMgr::CHANNELID::DASH, SOUND_DEFAULT);
			//timecontrol -= 0.1f;
		}
	}

	if (pGameInstance->Get_DIKeyState(DIK_LEFT))
	{
		if (pGameInstance->Get_DIKeyState(DIK_UP))
		{
			m_pTransformCom->TurnAround(_float3(0.f, 1.f, 0.f), DIRECTION::DIR_FRONTLEFT);
			m_pTransformCom->Go_Straight(fTimeDelta / sqrtf(2.f));
		}
		else if (pGameInstance->Get_DIKeyState(DIK_DOWN))
		{
			m_pTransformCom->TurnAround(_float3(0.f, 1.f, 0.f), DIRECTION::DIR_BACKLEFT);
			m_pTransformCom->Go_Straight(fTimeDelta / sqrtf(2.f));
		}

		else
		{
			m_pTransformCom->TurnAround(_float3(0.f, 1.f, 0.f), DIRECTION::DIR_LEFT);
			m_pTransformCom->Go_Straight(fTimeDelta);
		}

		if ((GetKeyState(VK_SHIFT) & 0x8000))
		{
			if (timecontrol > 0.2f)
			{
				CEffect_TrainSmoke::TRAINSMOKEDESC TrainSmokeDesc;

				TrainSmokeDesc.eSmoke = CEffect_TrainSmoke::ST_DYNAMITE;
				TrainSmokeDesc.iTextureNumber = 37;
				TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][0] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).x;
				TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][1] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y - 0.5f;
				TrainSmokeDesc.TransDesc.m_WorldMatrix.m[3][2] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).z;

				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Effect_Smok_Dash"), TEXT("Prototype_GameObject_Effect_TrainSmoke"), &TrainSmokeDesc.TransDesc);

				timecontrol = 0.f;
			}

			TransformDesc.fSpeedPerSec = 70.f;
			m_pTransformCom->Go_Straight(fTimeDelta);
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DASH);
			CSoundMgr::Get_Instance()->PlaySound(TEXT("Dash.mp3"), CSoundMgr::CHANNELID::DASH, SOUND_DEFAULT);
			//timecontrol -= 0.1f;
		}
	}
}

void CPlayer::Meteo(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	if (pGameInstance->Get_DIKeyState(DIKEYBOARD_0))
	{
		CTransform::TRANSFORMDESC	TransDesc;
		pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Meteo"), TEXT("Prototype_GameObject_Meteo"), &TransDesc);
	}
}

void CPlayer::Rain(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	if (pGameInstance->Get_DIKeyState(DIKEYBOARD_NUMPAD0))
	{
		CTransform::TRANSFORMDESC	TransDesc;
		pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Rain"), TEXT("Prototype_GameObject_Effect_Rain"), &TransDesc);


		raingo += fTimeDelta;
		if (raingo > 2) {
			//�� ������ ������ �� �ٲ�.

			//
			CLayer* pLLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Train"));

			list<CGameObject*>& objectList = pLLayer->Get_ObjectList();
			CGameObject* pTrainObject = nullptr;
			for (auto& object : objectList) {
				if (object->Get_Type() == OBJECT_TYPE::OBJECT_TRAIN_WATER)
					pTrainObject = object;
			}
			if (nullptr != pTrainObject)
				dynamic_cast<CTrain_Water*>(pTrainObject)->FireGone(fTimeDelta);

			raingo = 0.f;
		}

	}
}

void CPlayer::addArm(_float fTimeDelta)
{
	//���� ������ �޾��ִ� �Լ�.
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	CTransform::TRANSFORMDESC		TransformDesc;
	CGameObject* pColObject = m_pColliderCom->Get_CollisionObject();
	CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_RightHand"));
	CPlayer_RightHand* pRightHand = dynamic_cast<CPlayer_RightHand*>(*pLayer->Get_ObjectList().begin());

	CLayer* pLLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_LeftHand"));
	CPlayer_LeftHand* pLeftHand = dynamic_cast<CPlayer_LeftHand*>(*pLLayer->Get_ObjectList().begin());

}

void CPlayer::pressReturn(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	
	// ���⼭ ���� ��ġ���� ���� ����� �� ť���� ������ �����´�.
	CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, Layer_Terrain);
	list<CGameObject*>& ObjectList = pLayer->Get_ObjectList();

	_float3 vPlayerPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	ObjectList.sort([&vPlayerPos](CGameObject* Object1, CGameObject* Object2) {

		// �÷��̾��� ��ġ�� �ٸ� ����Ʈ�� ������Ʈ�� ���Ͽ� �����Ѵ�.
		_float3 vPos_Cube1 = dynamic_cast<CTransform*>(Object1->Get_Component(Component_Transform))->Get_State(CTransform::STATE_POSITION);
		_float3 vPos_Cube2 = dynamic_cast<CTransform*>(Object2->Get_Component(Component_Transform))->Get_State(CTransform::STATE_POSITION);

		return
			pow(vPlayerPos.x - vPos_Cube1.x, 2.f) + pow(vPlayerPos.z - vPos_Cube1.z, 2.f) <
			pow(vPlayerPos.x - vPos_Cube2.x, 2.f) + pow(vPlayerPos.z - vPos_Cube2.z, 2.f);
	});

	// �Ÿ������� ���ĵ� ������Ʈ ����Ʈ�߿��� ���� ����� ������Ʈ�� ��ġ�� �����´�.
	_float3 vObjectPos = dynamic_cast<CTransform*>((*ObjectList.begin())->Get_Component(Component_Transform))->Get_State(CTransform::STATE_POSITION);

	CTransform::TRANSFORMDESC tradesc;
	memcpy(tradesc.m_WorldMatrix.m[3], &vObjectPos, sizeof(_float3));
	tradesc.m_WorldMatrix.m[3][1] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y - 0.5f;

	//m_eHoldingState �� ������ �絿���϶�, 
	if (m_eHoldingState == HS_WATERBUCKET) {
		//pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_WaterBucket"), TEXT("Prototype_GameObject_WaterBucket"), &tradesc);
	} 
	if (m_eHoldingState == HS_DYNMITE) {
		//���̳ʸ���Ʈ �������,
		CGameInstance* pGameInstance = CGameInstance::GetInstance();


		CTransform::TRANSFORMDESC tradesc;
		tradesc.m_WorldMatrix.m[3][0] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).x;
		tradesc.m_WorldMatrix.m[3][1] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
		tradesc.m_WorldMatrix.m[3][2] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).z;


		pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Dynamite"), TEXT("Prototype_GameObject_Dynamite"), &tradesc);
		//	m_eHoldingState = HS_NONE;

	}

	if (m_eHoldingState == HS_DYNMITE)
	{

		CGameInstance* pGameInstance = CGameInstance::GetInstance();


		CTransform::TRANSFORMDESC tradesc;
		tradesc.m_WorldMatrix.m[3][0] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).x;
		tradesc.m_WorldMatrix.m[3][1] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
		tradesc.m_WorldMatrix.m[3][2] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).z;


		pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Dynamite"), TEXT("Prototype_GameObject_Dynamite"), &tradesc);
		//	m_eHoldingState = HS_NONE;

	}

	else {
		if (holdObj->Get_Type() == OBJECT_TYPE::OBJECT_MATERIAL_IRON ||
			holdObj->Get_Type() == OBJECT_TYPE::OBJECT_MATERIAL_WOOD) {
			tradesc.m_WorldMatrix.m[3][1] += 0.1f;
		}

		pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, GetTypeName(holdObj->Get_Type()), GetProtoName(holdObj->Get_Type()), &tradesc);

	}
	m_eHoldingState = HS_NONE;
	holdObj = nullptr;




	pLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_RightHand"));

	if (nullptr == pLayer)
		return;

	CPlayer_RightHand* pRightHand = dynamic_cast<CPlayer_RightHand*>(*pLayer->Get_ObjectList().begin());

	pLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_LeftHand"));

	if (nullptr == pLayer)
		return;

	CPlayer_LeftHand* pLeftHand = dynamic_cast<CPlayer_LeftHand*>(*pLayer->Get_ObjectList().begin());

	pRightHand->Change_Hand(0);
	pLeftHand->Change_Hand(false);
	pLeftHand->PlayerLift(0);
	pRightHand->HandAnimation(0);

	//CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
	CSoundMgr::Get_Instance()->PlaySound(TEXT("Objectdown.mp3"), CSoundMgr::CHANNELID::LIFTDOWN, SOUND_DEFAULT);
}


void CPlayer::pressSpace(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_RightHand"));

	if (nullptr == pLayer)
		return;

	CPlayer_RightHand* pRightHand = dynamic_cast<CPlayer_RightHand*>(*pLayer->Get_ObjectList().begin());

	CLayer* pLLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_LeftHand"));
	CPlayer_LeftHand* pLeftHand = dynamic_cast<CPlayer_LeftHand*>(*pLLayer->Get_ObjectList().begin());
	CGameObject* pColObject;


	switch (m_eHoldingState)
	{
	case Client::CPlayer::HS_NONE:
		break;
	case Client::CPlayer::HS_AXE:
	{
		CGameObject* pColObject = m_pColliderCom->Get_CollisionObject();
		//������������ ��ö�� �غ���
		if (pColObject != nullptr)
		{
			if (pColObject->Get_Type() == OBJECT_TYPE::OBJECT_TREE_DARK1 || pColObject->Get_Type() == OBJECT_TREE_DARK2 || pColObject->Get_Type() == OBJECT_TREE_DARK3
				|| pColObject->Get_Type() == OBJECT_TYPE::OBJECT_TREE_DARK4 || pColObject->Get_Type() == OBJECT_TREE_LIGHT || pColObject->Get_Type() == OBJECT_TREE_DARK_HIT1
				|| pColObject->Get_Type() == OBJECT_TYPE::OBJECT_TREE_DARK_HIT2 || pColObject->Get_Type() == OBJECT_TREE_LIGHT_HIT1 || pColObject->Get_Type() == OBJECT_TREE_LIGHT_HIT2)
			{
				Time += fTimeDelta;
				if (Time > 1.0f)
				{
					/* Effect */
					CComponent* pColTransform = pColObject->Get_Component(TEXT("Com_Transform"));
					_float3 pColPosition = dynamic_cast<CTransform*>(pColTransform)->Get_State(CTransform::STATE_POSITION);

					CEffect_Explo::EXPLODESC	ExploDesc;
					ExploDesc.iTextureNumber = rand() % 3 + 22;

					memcpy(ExploDesc.TransDesc.m_WorldMatrix.m[3], &pColPosition, sizeof(_float3));

					pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Effect_Explo"), TEXT("Prototype_GameObject_Effect_Explo"), &ExploDesc.TransDesc);
					////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					pRightHand->PlayerCol(true);
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
					CSoundMgr::Get_Instance()->PlaySound(TEXT("Woodhit.mp3"), CSoundMgr::CHANNELID::PLAYER, SOUND_MAX);
					dynamic_cast<CTree*>(pColObject)->Get_Damage();


					Time = 0.f;
				}
			}
		}

		else if (pColObject == nullptr)
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
			pRightHand->PlayerCol(false);
		}
	}
	break;
	case Client::CPlayer::HS_PICKAXE: //���
	{

		pColObject = m_pColliderCom->Get_CollisionObject();
		//������������ ��ö�� �غ���
		if (pColObject != nullptr) {
			if (pColObject->Get_Type() == OBJECT_TYPE::OBJECT_ROCK1 || pColObject->Get_Type() == OBJECT_ROCK2 || pColObject->Get_Type() == OBJECT_ROCK3
				|| pColObject->Get_Type() == OBJECT_TYPE::OBJECT_ROCK_HIT1 || pColObject->Get_Type() == OBJECT_ROCK_HIT2)
			{
				Time += fTimeDelta;
				if (Time > 1.f) {
					/* Effect */
					CComponent* pColTransform = pColObject->Get_Component(TEXT("Com_Transform"));
					_float3 pColPosition = dynamic_cast<CTransform*>(pColTransform)->Get_State(CTransform::STATE_POSITION);

					CEffect_Explo::EXPLODESC	ExploDesc;
					ExploDesc.iTextureNumber = 28;

					memcpy(ExploDesc.TransDesc.m_WorldMatrix.m[3], &pColPosition, sizeof(_float3));

					pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Effect_Explo"), TEXT("Prototype_GameObject_Effect_Explo"), &ExploDesc.TransDesc);
					////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					pRightHand->PlayerCol(true);
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
					CSoundMgr::Get_Instance()->PlaySound(TEXT("RockHit.mp3"), CSoundMgr::CHANNELID::PLAYER, SOUND_MAX);
					dynamic_cast<CRock*>(pColObject)->Get_Damage();
					Time = 0.f;
				}
			}
		}

		else if (pColObject == nullptr)
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
			pRightHand->PlayerCol(false);
		}
	}
	break;


	case Client::CPlayer::HS_BUCKET:
	{
		//��Ŷ�� ���̳ʸ���Ʈ 
		pColObject = m_pColliderCom->Get_CollisionObject();
		if (pColObject != nullptr)
		{
			//��Ŷ�� ��� ������, ��Ÿ���̶� ������,
			if (pColObject->Get_Type() == OBJECT_TYPE::OBJECT_WATER)
			{
				{
					CGameInstance* pGameInstance = CGameInstance::GetInstance();
					_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);


					waterTime += fTimeDelta;
					if (waterTime > 3) {

						//�浹�Ǹ� TOOLUI ���� ��¦�Ÿ��� ����.

						ToolUI::TOOLDES tooldesc;


						tooldesc.tradesc.m_WorldMatrix.m[3][0] = vPos.x;
						tooldesc.tradesc.m_WorldMatrix.m[3][1] = vPos.y + 1;
						tooldesc.tradesc.m_WorldMatrix.m[3][2] = vPos.z;

						tooldesc.tradesc.m_WorldMatrix.m[0][0] = 0.7f;
						tooldesc.tradesc.m_WorldMatrix.m[1][1] = 1.f;
						tooldesc.tradesc.m_WorldMatrix.m[2][2] = 1.f;

						tooldesc.tk = ToolUI::TOOLKIND::WATER;


						//�÷��̾��� ��ġ�� ������
						CGameInstance* pGameInstance = CGameInstance::GetInstance();
						CTransform*		bucketTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

						if (bucketTransform == nullptr)
							return;



						_float3 bucketpos = bucketTransform->Get_State(CTransform::STATE_POSITION);



						CTransform::TRANSFORMDESC tradesc;
						tradesc.m_WorldMatrix.m[3][0] = bucketpos.x + 0.5f;
						tradesc.m_WorldMatrix.m[3][1] = bucketpos.y;
						tradesc.m_WorldMatrix.m[3][2] = bucketpos.z;




						pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_WaterProgressBar"), TEXT("Prototype_GameObject_WaterProgressBar"), &tradesc);
						pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_MaterialTrainUI"), TEXT("Prototype_GameObject_ToolUI"), &tooldesc);

						m_eHoldingState = HS_WATERBUCKET;
						waterTime = 0.f;

						CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BUCKETWATER);
						CSoundMgr::Get_Instance()->PlaySound(TEXT("bucketfillup.mp3"), CSoundMgr::CHANNELID::BUCKETWATER, SOUND_DEFAULT);

						waterbucketgo += fTimeDelta;
						if (waterbucketgo > 2)
						{
				
							waterbucketgo = 0.f;
						}
					}
				}

			}
		}

		else
		{
			//m_eHoldingState = HS_WATERBUCKET;
	/*		pGameInstance->Delete_Layer(LEVEL_GAMEPLAY, TEXT("Layer_MaterialTrainUI"));
			pGameInstance->Delete_Layer(LEVEL_GAMEPLAY, TEXT("Layer_WaterProgressBar"));*/
			//���⼭ ������Ʈ�� �ٲ�����.

		}


	}
	break;


	case Client::CPlayer::HS_WOOD: //Matieral_Wood �ϋ� Ȥ�� iron�ϋ�
	{

		pColObject = m_pColliderCom->Get_CollisionObject();
		if (pColObject != nullptr)
		{
			//�������� �����ϱ�
			if (pColObject->Get_Type() == OBJECT_TYPE::OBJECT_MATERIAL_WOOD)
			{
				pGameInstance->Delete_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Wood"), pColObject);
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MATERIAL);
				CSoundMgr::Get_Instance()->PlaySound(TEXT("liftup.mp3"), CSoundMgr::CHANNELID::MATERIAL, SOUND_DEFAULT);
				m_eHoldingState = HS_WOOD;
				pRightHand->Change_Hand(4);
				pLeftHand->Change_Hand(true);
				pLeftHand->PlayerLift(1);
				pRightHand->HandAnimation(3);

				++getTreeCnt; //���� ������ �� �ø��� �̼�.
			}

			if (pColObject->Get_Type() == OBJECT_TYPE::OBJECT_TRAIN_MATERIAL) //Material Train�� �浹�ϸ�,
			{

				//�ϴ� wood�� ������
				pRightHand->Change_Hand(0);
				pLeftHand->Change_Hand(false);
				pLeftHand->PlayerLift(0);
				pRightHand->HandAnimation(0);

				CLayer* pLLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Train"));

				list<CGameObject*>& objectList = pLLayer->Get_ObjectList();
				CGameObject* pTrainObject = nullptr;
				for (auto& object : objectList) {
					if (object->Get_Type() == OBJECT_TYPE::OBJECT_TRAIN_MATERIAL)
						pTrainObject = object;
				}
				if (nullptr != pTrainObject)
					dynamic_cast<CTrain_Material*>(pTrainObject)->TrainState(1, 1);

				m_eHoldingState = HS_NONE;
				holdObj = nullptr;
				getTreeCnt = 0;	//������ ������ �� �����ֱ�
				return;



			}
		}
	}
	break;

	case Client::CPlayer::HS_IRON: //Matieral_Wood �ϋ� Ȥ�� iron�ϋ�
	{

		/*	OBJECT_MATERIAL_IRON, OBJECT_MATERIAL_WOOD,*/
		pColObject = m_pColliderCom->Get_CollisionObject();
		if (pColObject != nullptr)
		{


			//�������� �����ϱ�
			if (pColObject->Get_Type() == OBJECT_TYPE::OBJECT_MATERIAL_IRON)
			{
				pGameInstance->Delete_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Iron"), pColObject);
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MATERIAL);
				CSoundMgr::Get_Instance()->PlaySound(TEXT("liftup.mp3"), CSoundMgr::CHANNELID::MATERIAL, SOUND_DEFAULT);
				m_eHoldingState = HS_IRON;
				pRightHand->Change_Hand(5);
				pLeftHand->Change_Hand(true);
				pLeftHand->PlayerLift(1);
				pRightHand->HandAnimation(3);
			}


			if (pColObject->Get_Type() == OBJECT_TYPE::OBJECT_TRAIN_MATERIAL) //Material Train�� �浹�ϸ�,
			{

				//CComponent* pColTransform = pColObject->Get_Component(TEXT("Com_Transform"));
				//_float3 pColPosition = dynamic_cast<CTransform*>(pColTransform)->Get_State(CTransform::STATE_POSITION);

				//CTransform::TRANSFORMDESC tradesc;

				//tradesc.m_WorldMatrix.m[3][0] = pColPosition.x + 0.4f; //iron ������ǥ
				//tradesc.m_WorldMatrix.m[3][1] = pColPosition.y + 0.7f;
				//tradesc.m_WorldMatrix.m[3][2] = pColPosition.z;

				//pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, GetTypeName(holdObj->Get_Type()), GetProtoName(holdObj->Get_Type()), &tradesc);
				//dynamic_cast<CTrain_Material*>(pColObject)->PushMaterial("Iron", pColObject);	//wood���� �־��ش�.

				//�ϴ� wood�� ������
				pRightHand->Change_Hand(0);
				pLeftHand->Change_Hand(false);
				pLeftHand->PlayerLift(0);
				pRightHand->HandAnimation(0);


				//���⼭ �� �ٲ���
				CLayer* pLLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Train"));

				list<CGameObject*>& objectList = pLLayer->Get_ObjectList();
				CGameObject* pTrainObject = nullptr;
				for (auto& object : objectList) {
					if (object->Get_Type() == OBJECT_TYPE::OBJECT_TRAIN_MATERIAL)
						pTrainObject = object;
				}
				if (nullptr != pTrainObject)
					dynamic_cast<CTrain_Material*>(pTrainObject)->both(); //�̰��� ������ �ٲ��ֱ�

																		  //m_eHoldingState = HS_NONE;
				m_eHoldingState = HS_NONE;
				holdObj = nullptr;
				getTreeCnt = 0;	//������ ������ �� �����ֱ�
								//TOOLUI
				{


					CComponent* pColTransform = pColObject->Get_Component(TEXT("Com_Transform"));
					_float3 pColPosition = dynamic_cast<CTransform*>(pColTransform)->Get_State(CTransform::STATE_POSITION);


					ToolUI::TOOLDES tooldesc;


					tooldesc.tradesc.m_WorldMatrix.m[3][0] = pColPosition.x;
					tooldesc.tradesc.m_WorldMatrix.m[3][1] = pColPosition.y + 2;
					tooldesc.tradesc.m_WorldMatrix.m[3][2] = pColPosition.z;

					tooldesc.tradesc.m_WorldMatrix.m[0][0] = 0.7f;
					tooldesc.tradesc.m_WorldMatrix.m[1][1] = 1.f;
					tooldesc.tradesc.m_WorldMatrix.m[2][2] = 1.f;



					tooldesc.tk = ToolUI::TOOLKIND::WOOD;

					pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_MaterialTrainUI"), TEXT("Prototype_GameObject_ToolUI"), &tooldesc);

				}

				//holdObj = nullptr;
				m_eHoldingState = HS_NONE;


				pRightHand->Change_Hand(0);
				pRightHand->HandAnimation(1);


				return;
			}
		}
	}
	break;

	case Client::CPlayer::HS_RAIL1:
		if (!m_bPressButton && pGameInstance->Get_DIKeyState(DIK_P))
		{
			createTile();
			m_eHoldingState = Client::CPlayer::HS_NONE;
			pRightHand->Change_Hand(0);

			dynamic_cast<CRail_Invisible*>(*(pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Rail_Invisible"))->Get_ObjectList().begin()))->ShowVisible(false);
			m_bVisible = false;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::SETRAIL);
			CSoundMgr::Get_Instance()->PlaySound(TEXT("railset.mp3"), CSoundMgr::CHANNELID::SETRAIL, 0.8f);
		}
		break;

	case Client::CPlayer::HS_RAIL2:
		if (!m_bPressButton && pGameInstance->Get_DIKeyState(DIK_P))
		{
			createTile();
			m_eHoldingState = Client::CPlayer::HS_RAIL1;
			pRightHand->Change_Hand(7);
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::SETRAIL);
			CSoundMgr::Get_Instance()->PlaySound(TEXT("railset.mp3"), CSoundMgr::CHANNELID::SETRAIL, 0.8f);
		}
		break;

	case Client::CPlayer::HS_RAIL3:
		if (!m_bPressButton && pGameInstance->Get_DIKeyState(DIK_P))
		{
			createTile();
			m_eHoldingState = Client::CPlayer::HS_RAIL2;
			pRightHand->Change_Hand(8);
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::SETRAIL);
			CSoundMgr::Get_Instance()->PlaySound(TEXT("railset.mp3"), CSoundMgr::CHANNELID::SETRAIL, 0.8f);
		}
		break;



	case Client::CPlayer::HS_WATERBUCKET:
	{
		//waterbucket��� �ִ� �����ϋ�, 
		pColObject = m_pColliderCom->Get_CollisionObject();

		if (pColObject == nullptr)
		{
			pGameInstance->Delete_Layer(LEVEL_GAMEPLAY, TEXT("Layer_MaterialTrainUI"));
			pGameInstance->Delete_Layer(LEVEL_GAMEPLAY, TEXT("Layer_WaterProgressBar"));

		}



		if (pColObject != nullptr)
		{
			if (pColObject->Get_Type() == OBJECT_TYPE::OBJECT_TRAIN_WATER)
			{
				//
				CLayer* pLLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Train"));

				list<CGameObject*>& objectList = pLLayer->Get_ObjectList();
				CGameObject* pTrainObject = nullptr;
				for (auto& object : objectList) {
					if (object->Get_Type() == OBJECT_TYPE::OBJECT_TRAIN_WATER)
						pTrainObject = object;
				}
				if (nullptr != pTrainObject)
				{
					if (!fireEnd)
					{
						CSoundMgr::Get_Instance()->StopSound(CSoundMgr::FIREGONE);
						CSoundMgr::Get_Instance()->StopSound(CSoundMgr::TRAINFIRE);
						CSoundMgr::Get_Instance()->PlaySound(TEXT("FireGone.mp3"), CSoundMgr::CHANNELID::FIREGONE, 0.8f);
					}

					firetime += fTimeDelta;

				
					dynamic_cast<CTrain_Water*>(pTrainObject)->FireGone(fTimeDelta);
			

					fireEnd = true;
				}
			}

			else
			{
				fireEnd = false;
			}
		}


	}
	break;


	case Client::CPlayer::HS_END:
		break;
	default:
		break;
	}


	//�ʱ⿡ �ƹ��͵� �ȵ�� ������ ���� �ڵ�

	if (holdObj == nullptr)
	{
		CGameObject* pColObject = m_pColliderCom->Get_CollisionObject();
		if (pColObject != nullptr)
		{

			if (pColObject->Get_Type() == OBJECT_TYPE::OBJECT_TRAIN_DYNAMITE)
			{
				CTransform::TRANSFORMDESC tradesc;
				tradesc.m_WorldMatrix.m[3][0] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).x;
				tradesc.m_WorldMatrix.m[3][1] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
				tradesc.m_WorldMatrix.m[3][2] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).z;

				holdObj = pColObject;
				m_eHoldingState = HS_DYNMITE;
				pRightHand->Change_Hand(6);
				pLeftHand->Change_Hand(true);
				pLeftHand->PlayerLift(1);
				pRightHand->HandAnimation(3);
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MATERIAL);
				CSoundMgr::Get_Instance()->PlaySound(TEXT("AxeUp.mp3"), CSoundMgr::CHANNELID::AXE, SOUND_MAX);
				//	pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Dynamite"), TEXT("Prototype_GameObject_Dynamite"), &tradesc);

				return;


			}


			if (pColObject->Get_Type() == OBJECT_TYPE::OBJECT_AXE && GetAsyncKeyState(VK_SPACE) & 0x8000) {
				holdObj = pColObject; //��ġ�� ����ֵ��� �־��ְ�
				pGameInstance->Delete_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Hammer"), pColObject); //��ġ ����
				m_eHoldingState = HS_AXE; //��ġ ��� �ִٴ� ���¸� �ǹ� 1
				pRightHand->Change_Hand(1);
				pLeftHand->PlayerLift(0);
				pRightHand->HandAnimation(1);
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::AXE);
				CSoundMgr::Get_Instance()->PlaySound(TEXT("AxeUp.mp3"), CSoundMgr::CHANNELID::AXE, SOUND_MAX);
				//���⼭ UI ����
				pGameInstance->Delete_Layer(LEVEL_GAMEPLAY, TEXT("Layer_AxeUI"));
				return;
			}

			if (pColObject->Get_Type() == OBJECT_TYPE::OBJECT_PICKAX  && GetAsyncKeyState(VK_SPACE) & 0x8000) //�װ� ��̶��
			{
				holdObj = pColObject; //��̸� ����ֵ��� �־��ְ�
				pGameInstance->Delete_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_PickEx"), pColObject); //��� ������
				m_eHoldingState = HS_PICKAXE; //��̸� ��� �ִٴ� ���¸� �ǹ�
				pRightHand->Change_Hand(2);
				pLeftHand->PlayerLift(0);
				pRightHand->HandAnimation(2);
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PICKAX);
				CSoundMgr::Get_Instance()->PlaySound(TEXT("PickAxUp.mp3"), CSoundMgr::CHANNELID::PICKAX, SOUND_MAX);

				//���⼭ UI ����
				pGameInstance->Delete_Layer(LEVEL_GAMEPLAY, TEXT("Layer_PickAx_UI"));
				return;
			}
			if (pColObject->Get_Type() == OBJECT_TYPE::OBJECT_BUCKET  && GetAsyncKeyState(VK_SPACE) & 0x8000) //�װ� �絿�̶��
			{
				holdObj = pColObject; //�絿�̸� ��� �ִٰ� �־��ְ�
				pGameInstance->Delete_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Bucket"), pColObject); //�絿�� ������
				m_eHoldingState = HS_BUCKET; //�絿�̸� ��� �ִٴ� ���¸� �ǹ�
				pRightHand->Change_Hand(3);
				pLeftHand->Change_Hand(true);
				pLeftHand->PlayerLift(1);
				pRightHand->HandAnimation(3);
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BUCKET);
				CSoundMgr::Get_Instance()->PlaySound(TEXT("BucketUp.mp3"), CSoundMgr::CHANNELID::BUCKET, SOUND_DEFAULT);
				//���⼭ UI ����
				pGameInstance->Delete_Layer(LEVEL_GAMEPLAY, TEXT("Layer_BucketUI"));

				return;
			}

			if (pColObject->Get_Type() == OBJECT_TYPE::OBJECT_MATERIAL_WOOD  && GetAsyncKeyState(VK_SPACE) & 0x8000) //�װ� �����̶��
			{
				holdObj = pColObject;
				pGameInstance->Delete_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Wood"), pColObject);
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MATERIAL);
				CSoundMgr::Get_Instance()->PlaySound(TEXT("liftup.mp3"), CSoundMgr::CHANNELID::MATERIAL, SOUND_DEFAULT);
				m_eHoldingState = HS_WOOD;
				pRightHand->Change_Hand(4);
				pLeftHand->Change_Hand(true);
				pLeftHand->PlayerLift(1);
				pRightHand->HandAnimation(3);
				++getTreeCnt;

				return;
			}

			if (pColObject->Get_Type() == OBJECT_TYPE::OBJECT_MATERIAL_IRON  && GetAsyncKeyState(VK_SPACE) & 0x8000) //�װ� ö�̶��
			{
				holdObj = pColObject;
				pGameInstance->Delete_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Iron"), pColObject);
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MATERIAL);
				CSoundMgr::Get_Instance()->PlaySound(TEXT("liftup.mp3"), CSoundMgr::CHANNELID::MATERIAL, SOUND_DEFAULT);
				m_eHoldingState = HS_IRON;
				pRightHand->Change_Hand(5);
				pLeftHand->Change_Hand(true);
				pLeftHand->PlayerLift(1);
				pRightHand->HandAnimation(3);


				return;
			}
			//���̳ʸ���Ʈ ��� �ڵ�.
			if (pColObject->Get_Type() == OBJECT_TYPE::OBJECT_DYNAMITE  && GetAsyncKeyState(VK_SPACE) & 0x8000) //�װ� ���̳ʸ���Ʈ�̶��
			{


				holdObj = pColObject;
				pGameInstance->Delete_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Dynamite"), pColObject);
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MATERIAL);
				CSoundMgr::Get_Instance()->PlaySound(TEXT("liftup.mp3"), CSoundMgr::CHANNELID::MATERIAL, SOUND_DEFAULT);
				m_eHoldingState = HS_DYNMITE;
				pRightHand->Change_Hand(6);
				pLeftHand->Change_Hand(true);
				pLeftHand->PlayerLift(1);
				pRightHand->HandAnimation(3);


				return;
			}

			if (pColObject->Get_Type() == OBJECT_TYPE::OBJECT_DYNAMITE  && GetAsyncKeyState(VK_SPACE) & 0x8000) //�װ� ���̳ʸ���Ʈ�̶��
			{


				holdObj = pColObject;
				pGameInstance->Delete_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Dynamite"), pColObject);
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MATERIAL);
				CSoundMgr::Get_Instance()->PlaySound(TEXT("liftup.mp3"), CSoundMgr::CHANNELID::MATERIAL, SOUND_DEFAULT);
				m_eHoldingState = HS_DYNMITE;
				pRightHand->Change_Hand(6);
				pLeftHand->Change_Hand(true);
				pLeftHand->PlayerLift(1);
				pRightHand->HandAnimation(3);


				return;
			}

			if (pColObject->Get_Type() == OBJECT_TYPE::OBJECT_RAIL  && GetAsyncKeyState(VK_SPACE) & 0x8000) 
			{


				holdObj = pColObject;
				pGameInstance->Delete_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Rail"), pColObject);
				/*CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MATERIAL);
				CSoundMgr::Get_Instance()->PlaySound(TEXT("liftup.mp3"), CSoundMgr::CHANNELID::MATERIAL, SOUND_DEFAULT);*/
				m_eHoldingState = HS_RAIL3;
				pRightHand->Change_Hand(9);
				pLeftHand->Change_Hand(true);
				pLeftHand->PlayerLift(1);		// 0�̸� �޼��� �ְ� �����, 1�̸� �޼��� �����ش�
				pRightHand->HandAnimation(3);	// 3�̸� �������·� ������ش�.


				return;

			}
			
		}
	}
}

void CPlayer::RailTrainBomb(_float fTimeDelta)
{


	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_RightHand"));

	if (nullptr == pLayer)
		return;

	CPlayer_RightHand* pRightHand = dynamic_cast<CPlayer_RightHand*>(*pLayer->Get_ObjectList().begin());

	CLayer* pLLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_LeftHand"));
	CPlayer_LeftHand* pLeftHand = dynamic_cast<CPlayer_LeftHand*>(*pLLayer->Get_ObjectList().begin());


	CGameObject* pColObject = m_pColliderCom->Get_CollisionObject();

	if (pColObject != nullptr)
	{
		if (pColObject->Get_Type() == OBJECT_TYPE::OBJECT_TRAIN_RAIL) //Material Train�� �浹�ϸ�,
		{
			//CGameInstance* pGameInstance = CGameInstance::GetInstance();

			CTransform::TRANSFORMDESC tradesc;

			CComponent* pColTransform = pColObject->Get_Component(TEXT("Com_Transform"));
			_float3 pColPosition = dynamic_cast<CTransform*>(pColTransform)->Get_State(CTransform::STATE_POSITION);

			tradesc.m_WorldMatrix.m[3][0] = pColPosition.x;
			tradesc.m_WorldMatrix.m[3][1] = pColPosition.y + 0.5f;

			tradesc.m_WorldMatrix.m[3][2] = pColPosition.z;

			//MaterialWood / MaterialIron UI ����
			pGameInstance->Delete_Layer(LEVEL_GAMEPLAY, TEXT("Layer_MaterialTrainUI"));
			takeout = false;

			if (!colRailTrain)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MAKERAIL);
				CSoundMgr::Get_Instance()->PlaySound(TEXT("makerail.mp3"), CSoundMgr::CHANNELID::MAKERAIL, SOUND_MAX);
				colRailTrain = true;
			}

			//������ ���� materialĭ
			CLayer* pLLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Train"));

			list<CGameObject*>& objectList = pLLayer->Get_ObjectList();
			CGameObject* pTrainObject = nullptr;
			for (auto& object : objectList) {
				if (object->Get_Type() == OBJECT_TYPE::OBJECT_TRAIN_MATERIAL)
					pTrainObject = object;
			}
			if (nullptr != pTrainObject)
				dynamic_cast<CTrain_Material*>(pTrainObject)->TrainState(0, 0);  //�̰��� ������ �ٲ��ֱ�


																				 //���� ����
			CTransform::TRANSFORMDESC tradesc2;


			tradesc2.m_WorldMatrix.m[3][0] = pColPosition.x;
			tradesc2.m_WorldMatrix.m[3][1] = pColPosition.y + 1.f;
			tradesc2.m_WorldMatrix.m[3][2] = pColPosition.z;

			//�� ���� ���� ����
			for (auto& object : objectList) {
				if (object->Get_Type() == OBJECT_TYPE::OBJECT_TRAIN_RAIL)
					pTrainObject = object;
			}
			if (nullptr != pTrainObject) {
				if (!takeout) {
					dynamic_cast<CTrain_Rail*>(pTrainObject)->TrainState(1);
					m_getrail = true;
				}
			}

			if (m_getrail)
			{
				if (!m_bPressSpace&& pGameInstance->Get_DIKeyState(DIK_SPACE))
				{
					for (auto& object : objectList) {
						if (object->Get_Type() == OBJECT_TYPE::OBJECT_TRAIN_RAIL)
							pTrainObject = object;
					}
					if (nullptr != pTrainObject) {
						dynamic_cast<CTrain_Rail*>(pTrainObject)->TrainState(0);  //rail�غ���
						pRightHand->Change_Hand(9);

						m_eHoldingState = HS_RAIL3;
						m_getrail = false;
					}

					takeout = true;
				}

			}
		}

		else
		{
			colRailTrain = false;
		}
	}
}

void CPlayer::createTile()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	// �ѹ��� �������� �Ѵ�. ������ ���� ��� �ٽ� false������ �ٲ��.
	m_bPressButton = true;

	// ���⼭ ���� ��ġ���� ���� ����� �� ť���� ������ �����´�.
	CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, Layer_Terrain);
	list<CGameObject*>& ObjectList = pLayer->Get_ObjectList();

	_float3 vPlayerPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	ObjectList.sort([&vPlayerPos](CGameObject* Object1, CGameObject* Object2) {

		// �÷��̾��� ��ġ�� �ٸ� ����Ʈ�� ������Ʈ�� ���Ͽ� �����Ѵ�.
		_float3 vPos_Cube1 = dynamic_cast<CTransform*>(Object1->Get_Component(Component_Transform))->Get_State(CTransform::STATE_POSITION);
		_float3 vPos_Cube2 = dynamic_cast<CTransform*>(Object2->Get_Component(Component_Transform))->Get_State(CTransform::STATE_POSITION);

		return
			pow(vPlayerPos.x - vPos_Cube1.x, 2.f) + pow(vPlayerPos.z - vPos_Cube1.z, 2.f) <
			pow(vPlayerPos.x - vPos_Cube2.x, 2.f) + pow(vPlayerPos.z - vPos_Cube2.z, 2.f);
	});

	// �Ÿ������� ���ĵ� ������Ʈ ����Ʈ�߿��� ���� ����� ������Ʈ�� ��ġ�� �����´�.
	_float3 vObjectPos = dynamic_cast<CTransform*>((*ObjectList.begin())->Get_Component(Component_Transform))->Get_State(CTransform::STATE_POSITION);

	// ��ġ�� ���� ������Ʈ�� ��ġ�� ���� ���� �÷��ش�.
	vObjectPos.y += 0.5f;




	// ���� �÷��̾��� ��ġ�� ������ ���Ϸ� ���� ���� �Ÿ��� ������ �ִ��� Ȯ���Ѵ�. ����, �Ÿ��� 1�̸��̶��(������ ���� 0.5�� ó��) ������ �ִ� Ÿ���� ȸ���Ѵ�.
	// ���� �Ÿ��� 1�̻��̶��, �ش� �ڸ��� Ÿ���� �����Ѵ�.

	pLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Rail"));

	list<CGameObject*>* Object_List = nullptr;

	if (pLayer == nullptr)
		;
	else
		Object_List = &pLayer->Get_ObjectList();

	// ������Ʈ ����Ʈ�� �������� �ʴ´ٸ�, ������ �����Ѵ�.
	if (nullptr == Object_List)
		CreateRail(vObjectPos);

	else {
		// ������Ʈ ����Ʈ�� ���������� ������ �������� �ʴ´ٸ�
		if (Object_List->empty()) {
			// ������ �����Ѵ�.
			CreateRail(vObjectPos);
		}
		// ������ ������ �����Ѵٸ�
		else {
			// ������ ������ ��ġ ������ �����ͼ� ���� �÷��̾� ��ġ���� ���̸� ���Ѵ�.
			CGameObject* pRailObject = Object_List->back();
			_float3 vLastRailPos = dynamic_cast<CTransform*>(pRailObject->Get_Component(Component_Transform))->Get_State(CTransform::STATE_POSITION);
			_float fDistance = pow(vPlayerPos.x - vLastRailPos.x, 2.f) + pow(vPlayerPos.z - vLastRailPos.z, 2.f);

			// �Ÿ������� ������ ������ 1 �̸��̶��
			if (fDistance < 1.f) {
				// ������ ������ ȸ���Ͽ� �����´�.
				//WithDraw_LastRail();

				// �÷��̾ ������ �ִ� ������ ������ ���Ѵ�.

			}
			// �Ÿ� ������ ������ ������ 1 �̻��̶��
			else {
				// ������ �����Ѵ�.
				CreateRail(vObjectPos);
			}
		}
	}
}

void CPlayer::GetDynamite()
{
}

bool CPlayer::MissionBtnUpdate()
{
	if (getTreeCnt > 10) {

		//�̼� ���ø�Ʈ 
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		CTransform::TRANSFORMDESC	TransDesc;
		TransDesc.m_WorldMatrix.m[3][0] = 10.f;

		//�̼ǿϷ� ������,
		pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Rain"), TEXT("Prototype_GameObject_Effect_Rain"), &TransDesc);
		pGameInstance->Delete_Layer(LEVEL_GAMEPLAY, TEXT("Layer_MissionBtn"));
		//_tchar string2[100] = L"�̼� �Ϸ�!";
		//RECT rt2 = { 40,100,300,290 };
		//font2->DrawTextW(NULL, string2, -1, &rt2, DT_NOCLIP, D3DXCOLOR(255.0f, 201.0f, 14.0f, 1.0f));
		

		getTreeCnt = 0;
		return true;
	}
	if (getTreeCnt <= 10) {
		_tchar string1[100] = L"����  ���  10��  �����ϱ� x ";
		_tchar szBuff[256] = L"";
		wsprintf(szBuff, L"%d", getTreeCnt);
		lstrcat(string1, szBuff);

		RECT rt1 = { 50,100,370,390 };
		font->DrawTextW(NULL, string1, -1, &rt1, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		return false;
	}

}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer*	pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayer::Clone(void * pArg)
{
	CPlayer*	pInstance = new CPlayer(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}

void CPlayer::PickingTool(_tchar * LayerName)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	//�ش� ������Ʈ�� ������.

	CTransform*		PlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, LayerName, TEXT("Com_Transform"));
	_float3 PlayerPos = PlayerTransform->Get_State(CTransform::STATE_POSITION);

	

	PlayerTransform->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	PlayerTransform->Set_State(CTransform::STATE_LOOK, m_pTransformCom->Get_State(CTransform::STATE_LOOK));
	PlayerTransform->Set_State(CTransform::STATE_RIGHT, m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
	PlayerTransform->Set_State(CTransform::STATE_UP, m_pTransformCom->Get_State(CTransform::STATE_UP));


	_float4x4 playermatrix = PlayerTransform->Get_WorldMatrix();
	_float4x4 myMatrix = m_pTransformCom->Get_WorldMatrix();

	PlayerTransform->Set_WorldMatrix(playermatrix*myMatrix);
	PlayerTransform->Scaled(_float3{ 0.03f, 0.03f, 0.03f });

}

void CPlayer::DropTool(_tchar * LayerName)
{
	

		CGameInstance* pGameInstance = CGameInstance::GetInstance();

		//�ش� ������Ʈ�� ������.
		CGameObject* pColObject = m_pColliderCom->Get_CollisionObject();
		CTransform*		PlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, LayerName, TEXT("Com_Transform"));
		_float3 PlayerPos = PlayerTransform->Get_State(CTransform::STATE_POSITION);


		PlayerTransform->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		PlayerTransform->Set_State(CTransform::STATE_LOOK, m_pTransformCom->Get_State(CTransform::STATE_LOOK));
		PlayerTransform->Set_State(CTransform::STATE_RIGHT, m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
		PlayerTransform->Set_State(CTransform::STATE_UP, m_pTransformCom->Get_State(CTransform::STATE_UP));
		PlayerTransform->Scaled(_float3{ 0.03f, 0.03f, 0.03f });


	

}

const _tchar * CPlayer::GetTypeName(OBJECT_TYPE obj)
{
	if (obj == OBJECT_TYPE::OBJECT_AXE)
		return TEXT("Layer_Hammer");
	else if (obj == OBJECT_TYPE::OBJECT_PICKAX)
		return TEXT("Layer_PickEx");
	else if (obj == OBJECT_TYPE::OBJECT_BUCKET)
		return TEXT("Layer_Bucket");
	else if (obj == OBJECT_TYPE::OBJECT_MATERIAL_WOOD)
		return TEXT("Layer_Wood");
	else if (obj == OBJECT_TYPE::OBJECT_MATERIAL_IRON)
		return TEXT("Layer_Iron");
	else if (obj == OBJECT_TYPE::OBJECT_DYNAMITE)
		return TEXT("Layer_Dynamite");
}

const _tchar * CPlayer::GetProtoName(OBJECT_TYPE obj)
{
	if (obj == OBJECT_TYPE::OBJECT_AXE)
		return TEXT("Prototype_GameObject_Axe");
	else if (obj == OBJECT_TYPE::OBJECT_PICKAX)
		return TEXT("Prototype_GameObject_PickAx");
	else if (obj == OBJECT_TYPE::OBJECT_BUCKET)
		return TEXT("Prototype_GameObject_Bucket");
	else if (obj == OBJECT_TYPE::OBJECT_MATERIAL_WOOD)
		return TEXT("Prototype_GameObject_Wood");
	else if (obj == OBJECT_TYPE::OBJECT_MATERIAL_IRON)
		return TEXT("Prototype_GameObject_Iron");
	else if (obj == OBJECT_TYPE::OBJECT_DYNAMITE)
		return TEXT("Prototype_GameObject_Dynamite");
}



void CPlayer::CreateRail(_float3 _vPos)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	// ������ ������ �����.
	CRail::RAILDESC RailDesc;
	RailDesc.eType_Rail = CRail_Manager::GetInstance()->Get_State();
	memcpy(RailDesc.TransDesc.m_WorldMatrix.m[3], &_vPos, sizeof(_float3));

	// ������ ��ȯ�Ѵ�.
	pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Rail"), TEXT("Prototype_GameObject_Rail"), &RailDesc);

	// �������� ��ȯ�� ������ ������ �����´�.
	list<CGameObject*>& Object_List = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Rail"))->Get_ObjectList();
	CGameObject* pRailObject = Object_List.back();

	// ��ȯ�� ���� ������Ʈ�� ���� �Ŵ����� �־��ش�.
	CRail_Manager::GetInstance()->Push_RailObject(pRailObject);
}

void CPlayer::WithDraw_LastRail()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	// ���� �Ŵ����� ������Ʈ�� ȸ���Ѵ�.
	CRail_Manager::GetInstance()->Pop_RailObject();

	// �������� ��ȯ�� ������ ������ �����´�.
	list<CGameObject*>& Object_List = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Rail"))->Get_ObjectList();
	CGameObject* pRailObject = Object_List.back();

	// ���̾ �ִ� ������ ������ �����Ѵ�.
	pGameInstance->Delete_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Rail"), pRailObject);

	// ������Ʈ ����Ʈ�� �����Ѵٸ�
	if (Object_List.empty())
		return;

	// ȸ���Ҷ�, �������� ������ Ÿ���� ������ Ÿ�Կ� ���� ���� Ÿ���� ���� �Ŵ����� �������ش�.
	pRailObject = Object_List.back();

	CRail::RAIL_TYPE eType = dynamic_cast<CRail*>(pRailObject)->Get_RailType();

	switch (eType)
	{
	case Client::CRail::RAIL_STRAIGHT_LEFT:
	case Client::CRail::RAIL_STRAIGHT_RIGHT:
	case Client::CRail::RAIL_STRAIGHT_UP:
	case Client::CRail::RAIL_STRAIGHT_DOWN:
		CRail_Manager::GetInstance()->Set_State(eType);
		break;
	case Client::CRail::RAIL_ROTATE_LEFT_UP:
	case Client::CRail::RAIL_ROTATE_RIGHT_UP:
		CRail_Manager::GetInstance()->Set_State(CRail::RAIL_STRAIGHT_UP);
		break;
	case Client::CRail::RAIL_ROTATE_LEFT_DOWN:
	case Client::CRail::RAIL_ROTATE_RIGHT_DOWN:
		CRail_Manager::GetInstance()->Set_State(CRail::RAIL_STRAIGHT_DOWN);
		break;
	case Client::CRail::RAIL_ROTATE_UP_LEFT:
	case Client::CRail::RAIL_ROTATE_DOWN_LEFT:
		CRail_Manager::GetInstance()->Set_State(CRail::RAIL_STRAIGHT_LEFT);
		break;
	case Client::CRail::RAIL_ROTATE_UP_RIGHT:
	case Client::CRail::RAIL_ROTATE_DOWN_RIGHT:
		CRail_Manager::GetInstance()->Set_State(CRail::RAIL_STRAIGHT_RIGHT);
		break;
	case Client::CRail::RAIL_END:
		break;
	default:
		break;
	}

}

void CPlayer::ShowRailPos(_float fTimeDelta)
{// �÷��̾��� ���°� ������ ��� �ִ� ���¶��
	if (m_eHoldingState == HS_RAIL1 ||
		m_eHoldingState == HS_RAIL2 ||
		m_eHoldingState == HS_RAIL3) {

		m_fVisibleRailDeltaTime += fTimeDelta;

		if (m_fVisibleRailDeltaTime >= 0.2) {
			m_fVisibleRailDeltaTime = 0.f;

			CGameInstance* pGameInstance = CGameInstance::GetInstance();
			CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Rail_Invisible"));
			if (nullptr == pLayer)
				return;

			CRail_Invisible* pInvisibleRail = dynamic_cast<CRail_Invisible*>(*(pLayer->Get_ObjectList().begin()));
			if (!m_bVisible) {
				// TEXT("Layer_Rail_Invisible")�� ������Ʈ�� ��� ������ ���¸� ��ġ�� ��ҿ� �����Ͽ� �����ش�.
				pInvisibleRail->ShowVisible(true);

				m_bVisible = true;
			}

			// ����Ͽ� ������ ��ġ ������ �����Ѵ�.

			// ���⼭ ���� ��ġ���� ���� ����� �� ť���� ������ �����´�.
			pLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, Layer_Terrain);
			list<CGameObject*>& ObjectList = pLayer->Get_ObjectList();

			_float3 vPlayerPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

			ObjectList.sort([&vPlayerPos](CGameObject* Object1, CGameObject* Object2) {

				// �÷��̾��� ��ġ�� �ٸ� ����Ʈ�� ������Ʈ�� ���Ͽ� �����Ѵ�.
				_float3 vPos_Cube1 = dynamic_cast<CTransform*>(Object1->Get_Component(Component_Transform))->Get_State(CTransform::STATE_POSITION);
				_float3 vPos_Cube2 = dynamic_cast<CTransform*>(Object2->Get_Component(Component_Transform))->Get_State(CTransform::STATE_POSITION);

				return
					pow(vPlayerPos.x - vPos_Cube1.x, 2.f) + pow(vPlayerPos.z - vPos_Cube1.z, 2.f) <
					pow(vPlayerPos.x - vPos_Cube2.x, 2.f) + pow(vPlayerPos.z - vPos_Cube2.z, 2.f);
			});

			// �Ÿ������� ���ĵ� ������Ʈ ����Ʈ�߿��� ���� ����� ������Ʈ�� ��ġ�� �����´�.
			_float3 vObjectPos = dynamic_cast<CTransform*>((*ObjectList.begin())->Get_Component(Component_Transform))->Get_State(CTransform::STATE_POSITION);

			// ��ġ�� ���� ������Ʈ�� ��ġ�� ���� ���� �÷��ش�.
			vObjectPos.y += 0.5f;

			CTransform* pInvisibleTransform = dynamic_cast<CTransform*>(pInvisibleRail->Get_Component(Component_Transform));
			pInvisibleTransform->Set_State(CTransform::STATE_POSITION, vObjectPos);
		}
	}
}

