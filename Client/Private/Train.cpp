#include "stdafx.h"
#include "Train.h"
#include "GameInstance.h"
#include "Effect_Explo.h"
#include "Rail_Manager.h"
#include "SoundMgr.h"

CTrain::CTrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CTrain::CTrain(const CTrain & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTrain::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTrain::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;
}

void CTrain::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CTrain::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CTrain::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	// 라이프는 최대 3, 1이되고 다음 타이밍에 삭제된다.
	auto& ObjectList = m_pVIBufferCom->Get_lstVIBuffer();

	for (auto& Object : ObjectList) {

		if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(Object.second)))
			return E_FAIL;

		Object.first->Render();
	}

	return S_OK;
}

HRESULT CTrain::SetUp_TrainComponents(void * pArg)
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
	vPos.y = 0.5f;

	TransDesc.fSpeedPerSec = 0.1f;
	TransDesc.fRotationPerSec = D3DXToRadian(10.0f);
	TransDesc.fScalePerSec = 1.f;


	// 배율
	_float fScale = 35.f;

	TransDesc.m_WorldMatrix._11 /= fScale;
	TransDesc.m_WorldMatrix._12 /= fScale;
	TransDesc.m_WorldMatrix._13 /= fScale;

	TransDesc.m_WorldMatrix._21 /= fScale;
	TransDesc.m_WorldMatrix._22 /= fScale;
	TransDesc.m_WorldMatrix._23 /= fScale;

	TransDesc.m_WorldMatrix._31 /= fScale;
	TransDesc.m_WorldMatrix._32 /= fScale;
	TransDesc.m_WorldMatrix._33 /= fScale;

	memcpy(TransDesc.m_WorldMatrix.m[3], &vPos, sizeof(_float3));

	// Tree의 스케일 조정
	if (FAILED(__super::SetUp_Components(Component_Transform, LEVEL_STATIC, Prototype_Compo_Transform, (CComponent**)&m_pTransformCom, &TransDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(Component_Renderer, LEVEL_STATIC, Prototype_Compo_Renderer, (CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(Component_Texture, LEVEL_STATIC, TEXT("Prototype_Component_Texture_HyunWoo"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	CCollider::ColliDesc colDesc;

	colDesc.localMatrix._11 *= 1.5f;
	colDesc.localMatrix._12 *= 1.5f;
	colDesc.localMatrix._13 *= 1.5f;
	colDesc.localMatrix._21 *= 1.5f;
	colDesc.localMatrix._22 *= 1.5f;
	colDesc.localMatrix._23 *= 1.5f;
	colDesc.localMatrix._31 *= 1.f;
	colDesc.localMatrix._32 *= 1.f;
	colDesc.localMatrix._33 *= 1.f;
	colDesc.pTransform = m_pTransformCom;

	/* For.Com_Collider */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Collider"), LEVEL_STATIC, TEXT("Prototype_Component_Collider"), (CComponent**)&m_pColliderCom, &colDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTrain::SetUp_VIBuffer(const TCHAR * strFileName)
{
	Delete_Component(Component_VIBuffer);

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, (void*)strFileName)))
		return E_FAIL;

	return S_OK;
}

void CTrain::StartMove()
{
	m_bMove = true;

	// 여기 시작할때 나오는 사운드를 추가한다.
}

void CTrain::Follow_Rail(_float _fDeltaTime)
{
	float fRailTime = 0.f;

	// 목적지에 도착했을 경우
	if (m_bArrive)
	{
		// 더이상 따라가지 않고, 엔딩과 관련된 행동을 한다.
		return;
	}

	// 현재 맵에 존재하는 레일의 정보를 들고온다.
	vector<CGameObject*>& RailInfo = CRail_Manager::GetInstance()->Get_vRailInfo();

	// 따라온 레일의 번호가 마지막 레일의 번호일 경우 
	if ((RailInfo.size() - 1) == m_iRailNumber)
	{
		// 레일이 마지막 레일과 연결되지 않았다면
		if (!CRail_Manager::GetInstance()->Get_Connected()) {
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::TRAINGO);

			// 기차가 도착하지 못했다면
			FailToArrive();
		}
		// 레일이 마지막 레일과 연결되었다면
		else
			// 도착했다!
			Arrive();
	}





	// 현재 레일이 안정권 내에 있는 레일이라면
	if (m_iRailNumber < RailInfo.size()) {
		// 레일의 중점에 도착했는지 확인한다.
		_float3 vTrainPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vRailPos =
			dynamic_cast<CTransform*>(RailInfo.at(m_iRailNumber)->Get_Component(Component_Transform))->Get_State(CTransform::STATE_POSITION);

		// 두 중점의 차이가 각 x,y값으로 0.01 이하면 도착한것이다.
		if ((abs(vTrainPos.x - vRailPos.x) < 0.03125) &&
			(abs(vTrainPos.z - vRailPos.z) < 0.03125)) {

			// 도착했다면 다음 레일 정보를 가져온다.
			if (m_iRailNumber < RailInfo.size()) {
				++m_iRailNumber;


				if (m_iRailNumber < RailInfo.size()) {
					CRail::RAIL_TYPE eRailType = dynamic_cast<CRail*>(RailInfo.at(m_iRailNumber))->Get_RailType();

					// 원하는 각도를 넣어주고
					switch (eRailType)
					{
					case Client::CRail::RAIL_ROTATE_RIGHT_DOWN:
						m_fAngle = 90;
						break;
					case Client::CRail::RAIL_ROTATE_UP_LEFT:
						m_fAngle = -180;
						break;
					case Client::CRail::RAIL_ROTATE_RIGHT_UP:
						m_fAngle = -90;
						break;
					case Client::CRail::RAIL_ROTATE_DOWN_LEFT:
						m_fAngle = 180;
						break;
					case Client::CRail::RAIL_ROTATE_DOWN_RIGHT:
						m_fAngle = 0;
						break;
					case Client::CRail::RAIL_ROTATE_LEFT_UP:
						m_fAngle = -90;
						break;
					case Client::CRail::RAIL_ROTATE_LEFT_DOWN:
						m_fAngle = 90;
						break;
					case Client::CRail::RAIL_ROTATE_UP_RIGHT:
						m_fAngle = 0;
						break;
					default:
						break;
					}
				}
			}
		}

		// 레일을 따라서 움직인다.
		m_pTransformCom->Chase(vRailPos, _fDeltaTime, 0.01);

		// 돌린만큼 반대 방향으로 돌리고(== 원본방향으로 만들고)
		m_pTransformCom->Turned(_float3(0.f, 1.f, 0.f), -m_fTurnAngle);


		if (m_fTurnAngle < m_fAngle)
		{
			m_fTurnAngle += 0.8f*_fDeltaTime*10.f;
			if (m_fTurnAngle >= m_fAngle)
			{
				m_fTurnAngle = m_fAngle;
			}
		}

		else if (m_fTurnAngle > m_fAngle)
		{
			m_fTurnAngle -= 0.8f*_fDeltaTime*10.f;
			if (m_fTurnAngle <= m_fAngle)
			{
				m_fTurnAngle = m_fAngle;
			}
		}
		// 원하는 각도만큼 다시 돌린다.
		m_pTransformCom->Turned(_float3(0.f, 1.f, 0.f), m_fTurnAngle);
	}
}

void CTrain::Arrive()
{
	// 도착했다는 신호를 준다.
	m_bArrive = true;
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::GAMEFINISH);
	CSoundMgr::Get_Instance()->PlaySound(TEXT("GameFinish.mp3"), CSoundMgr::CHANNELID::GAMEFINISH, 0.6f);
	// 추가적인 상호작용을 작성한다.
}

void CTrain::FailToArrive()
{
	// 도착하지 못했경우, 관련 코드를 작성한다.

	// 터진다.
	Bomb();
}

void CTrain::Bomb()
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::TRAINEXPLO);
	CSoundMgr::Get_Instance()->PlaySound(TEXT("explo22.mp3"), CSoundMgr::CHANNELID::TRAINEXPLO, 0.6f);
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	CEffect_Explo::EXPLODESC	ExploDesc;
	ExploDesc.iTextureNumber = rand() % 2 + 15;
	ExploDesc.TransDesc.m_WorldMatrix.m[3][0] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).x;
	ExploDesc.TransDesc.m_WorldMatrix.m[3][1] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
	ExploDesc.TransDesc.m_WorldMatrix.m[3][2] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).z;
	for (_uint i = 0; i < 30; i++)
	{
		pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Effect_Explo"), TEXT("Prototype_GameObject_Effect_Explo"), &ExploDesc.TransDesc);
		m_bDead = true;
	}
}

void CTrain::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
