#include "stdafx.h"
#include "..\Public\Camera_Default.h"
#include "GameInstance.h"

CCamera_Default::CCamera_Default(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
{
}

CCamera_Default::CCamera_Default(const CCamera_Default & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Default::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Default::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Default::Tick(_float fTimeDelta)
{
	//CGameInstance* pGameInstance = CGameInstance::GetInstance();
	//if (pGameInstance->Get_DIKeyState(DIK_W))
	//{
	//	m_pTransform->Go_Straight(fTimeDelta);
	//}
	//if (GetKeyState('S') & 0x8000)
	//{
	//	m_pTransform->Go_BackWard(fTimeDelta);
	//}
	//if (GetKeyState('A') & 0x8000)
	//{
	//	m_pTransform->Go_Left(fTimeDelta);
	//}
	//if (GetKeyState('D') & 0x8000)
	//{
	//	m_pTransform->Go_Right(fTimeDelta);
	//}

	if (GetAsyncKeyState('1'))
		ChangeCamera = true;

	else if (GetAsyncKeyState('2'))
		ChangeCamera = false;

	if (ChangeCamera == true)
		Camera_Player(fTimeDelta);

	else if (ChangeCamera == false)
		Camera_Base(fTimeDelta);


	__super::Tick(fTimeDelta);

}


void CCamera_Default::Camera_Base(_float fTimeDelta)
{

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	//CTransform::TRANSFORMDESC	camdesc;

	Safe_AddRef(pGameInstance);
	f3pTime += fTimeDelta;
	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (nullptr == pPlayerTransform)
		return;

	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	_float3		vPosition = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_float3		vPositionCamera = m_pTransform->Get_State(CTransform::STATE_POSITION);

	vPositionCamera.x = vPosition.x - 10.f;
	vPositionCamera.y = 30.f;
	vPositionCamera.z = -35.f;
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPositionCamera);

	m_pTransform->LookAt(pPlayerTransform->Get_State(CTransform::STATE_POSITION));
	Safe_Release(pGameInstance);

	//if (f3pTime > 84.f)
	//{
	//	m_pTransform->CameraShake(fTimeDelta);
	//	if (f3pTime > 91.f)
	//	{
	//		f3pTime = -2.f;
	//	}
	//}

}

void CCamera_Default::Camera_Player(_float fTimeDelta)
{

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	f1pTime += fTimeDelta;
	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (nullptr == pPlayerTransform)
		return;

	CTransform*		pGiantTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Giant"), TEXT("Com_Transform"));
	if (nullptr == pGiantTransform)
		return;

	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));


	_float3		vPosition = pGiantTransform->Get_State(CTransform::STATE_POSITION);
	_float3		vPosition1 = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_float3		vPositionCamera = m_pTransform->Get_State(CTransform::STATE_POSITION);


	vPositionCamera.x = vPosition1.x;
	vPositionCamera.y = 20.f;
	vPositionCamera.z = -40.f;
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPositionCamera);

	m_pTransform->LookAt({ vPosition1.x, 10.f, vPosition1.z });
	Safe_Release(pGameInstance);


	//if (f1pTime > 84.f)
	//{
	//	m_pTransform->CameraShake(fTimeDelta);
	//	if (f1pTime > 91.f)
	//	{
	//		f1pTime = -2.f;
	//	}
	//}


	//CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	//Safe_AddRef(pGameInstance);

	//CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));

	//_float3		vPosition = pPlayerTransform->Get_State(CTransform::STATE_POSITION);		// 기차의 좌표
	//_float3		vPositionCamera = m_pTransform->Get_State(CTransform::STATE_POSITION);		// 카메라의 좌표
	//if (vPosition.x != vPositionCamera.x || vPosition.z != vPositionCamera.z)
	//{
	//	vPositionCamera.x = vPosition.x;
	//	vPositionCamera.y = vPosition.y + 2.0f;
	//	vPositionCamera.z = vPosition.z - 3.f;
	//}

	//m_pTransform->Set_State(CTransform::STATE_POSITION, vPositionCamera);


	//m_vMouse = Get_Mouse();		//마우스 좌표 받아오기

	//_float3 vDirection = m_vMouse - m_vPreMouse;

	//m_pTransform->Turn(_float3(0.f, 1.f, 0.f), 0.05f*fTimeDelta * vDirection.x);		//뒤에 인자에 감도조절 0.5곱하기
	//																					//m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_UP), fTimeDelta * vDummy.x);
	//m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_RIGHT), 0.1f*fTimeDelta * vDirection.y);
	//Set_Mouse();
	//ShowCursor(FALSE);

	//CTransform* pCamelcol = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Camel"), TEXT("Com_Transform"));
	//if (nullptr == pCamelcol)
	//	return;

	//m_isCol = CheckIntersect(pPlayerTransform->Get_State(CTransform::STATE_POSITION), 0.5f, pCamelcol->Get_State(CTransform::STATE_POSITION), 0.5f);

	////if (m_isCol)
	////{
	////	m_pTransform->CameraShake(fTimeDelta);
	////}

	//Safe_Release(pGameInstance);


	//CGameInstance*      pGameInstance = CGameInstance::GetInstance();
	//CTransform*      pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));
	//_float3		PlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);


	//_long      MouseMove;


	//if (GetAsyncKeyState(VK_LBUTTON)) {

	//	m_pTransform->Set_State(CTransform::STATE_POSITION, _float3{ PlayerPos.x, PlayerPos.y, PlayerPos.z + 2 });


	//	if (MouseMove = pGameInstance->Get_DIMMoveState(CInput_Device::DIMM_X))
	//	{
	//		m_pTransform->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta*MouseMove*0.1f);
	//	}

	//	else if (MouseMove = pGameInstance->Get_DIMMoveState(CInput_Device::DIMM_Y))
	//	{
	//		m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_LOOK), fTimeDelta* MouseMove * 0.1f);
	//	}
	//}
	//else
	//{

	//	_float3      vPosition = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	//	_float3      vRight = pPlayerTransform->Get_State(CTransform::STATE_RIGHT);
	//	_float3      vUp = pPlayerTransform->Get_State(CTransform::STATE_UP);
	//	_float3      vLook = pPlayerTransform->Get_State(CTransform::STATE_LOOK);



	//	_float3      vPositionCamera = m_pTransform->Get_State(CTransform::STATE_POSITION);      // 카메라의 좌표

	//	m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);
	//	m_pTransform->Set_State(CTransform::STATE_UP, vUp);
	//	m_pTransform->Set_State(CTransform::STATE_LOOK, vLook);
	//	//if (vPosition.x != vPositionCamera.x || vPosition.z != vPositionCamera.z)
	//	{
	//		vPositionCamera.x = vPosition.x;
	//		vPositionCamera.y = vPosition.y + 1.f;
	//		vPositionCamera.z = vPosition.z - 2.f;

	//		//vPositionCamera.x = vPosition.x;
	//		//vPositionCamera.y = vPosition.y;
	//		//vPositionCamera.z = vPosition.z;

	//		m_pTransform->Set_State(CTransform::STATE_POSITION, vPositionCamera);
	//	}
	//	m_pTransform->LookAt(vPosition);

	//}

	//__super::Tick(fTimeDelta);
}

bool CCamera_Default::CheckIntersect(_float3 center1, float radius1, _float3 center2, float radius2)
{
	float fDist;
	_float3 vSub;

	vSub = center2 - center1;
	fDist = D3DXVec3Length(&vSub);

	if (fDist <= (radius1 + radius2))
		return true;

	return false;
}


void CCamera_Default::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_Default::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

CCamera_Default * CCamera_Default::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Default*	pInstance = new CCamera_Default(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CCamera_Default"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Default::Clone(void * pArg)
{
	CCamera_Default*	pInstance = new CCamera_Default(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CCamera_Default"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Default::Free()
{
	__super::Free();

}
