#include "stdafx.h"
#include "..\Public\Rail_Manager.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "Train.h"

IMPLEMENT_SINGLETON(CRail_Manager)

CRail_Manager::CRail_Manager()
{
}

CRail_Manager::CRail_Manager(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: m_pGraphic_Device(_pGraphic_Device)
{
}

CRail_Manager::~CRail_Manager()
{
}

CRail_Manager * CRail_Manager::Create(LPDIRECT3DDEVICE9 _pGraphic_Device)
{
	m_pInstance = new CRail_Manager(_pGraphic_Device);

	return m_pInstance;
}


void CRail_Manager::Push_RailObject(CGameObject * pGameObject)
{
	m_vRail.push_back(pGameObject);

	// 마지막에 넣은 레일의 정보가 엔딩레일과 연결되었는지 판별한다.
	Check_ConnectEndingRail(pGameObject);

	if (m_vRail.size() > 2) {
		// 마지막 레일로 부터 이전이전 레일의 정보를 검색한다.
		CTransform* pTransformPre =
			dynamic_cast<CTransform*>(m_vRail.at(m_vRail.size() - 3)->Get_Component(Component_Transform));

		if (nullptr == pTransformPre)
			return;

		_float3 vPosPre = pTransformPre->Get_State(CTransform::STATE_POSITION);

		// 해당 레일과 현재 추가된 레일의 중점 정보를 비교한다.
		CTransform* pTransformNext =
			dynamic_cast<CTransform*>(pGameObject->Get_Component(Component_Transform));

		_float3 vPosNext = pTransformNext->Get_State(CTransform::STATE_POSITION);

		// 같은 축에 있는지 다른 축에 있는지 검사
		// 중점 사이의 길이가 루트2보다 크다면 같은축, 루트2와 같다면 다른축에 있다.
		// 이는 큐브들의 가로세로 길이가 1이기 때문에 적용할 수 있는 방법이다.

		// 비교를 위해 2가 아닌 조금 큰, 4보단 작은 수를 넣는다.

		_float fGapX, fGapZ;
		// 설치한 레일로부터 이전 레일의 정보를 뺀다. 
		fGapX = vPosNext.x - vPosPre.x;
		fGapZ = vPosNext.z - vPosPre.z;

		// 만약 중심점의 차이의 제곱끼리 더한값이 2보다 작다면, 이는 같은 축에 있다.
		if (pow(fGapX, 2.f) + pow(fGapZ, 2.f) <= 1.f) {
			// 이전 레일의 방향을 현재 레일의 방향으로 한다.

			// 이전 레일의 정보를 가져온다.
			CGameObject* pRailObjectPre = m_vRail.at(m_vRail.size() - 2);
			CRail::RAIL_TYPE eTypePre = dynamic_cast<CRail*>(pRailObjectPre)->Get_RailType();

			// 현재 레일의 정보를 가져온다.
			CGameObject* pRailObjectLast = m_vRail.at(m_vRail.size() - 1);

			// 현재 레일의 정보를 이전 레일의 방향으로 설정한다.
			if (pRailObjectLast->Get_Type() != OBJECT_RAIL_END)
				dynamic_cast<CRail*>(pRailObjectLast)->Change_RailType(eTypePre);
		}
		// 만약 중심점의 차이의 제곱의 합이 2보다 크다면, 이는 다른 축에 있다. 
		else {
			// 플레이어의 위치에 따라 설치할 레일의 방향을 설정하고, 마지막에 설치한 레일 이전레일의 정보를 바꿔준다.

			// 이전이전 레일의 정보를 가져온다.
			CGameObject* pRailObjectPrePre = m_vRail.at(m_vRail.size() - 3);
			CRail::RAIL_TYPE eTypePrePre = dynamic_cast<CRail*>(pRailObjectPrePre)->Get_RailType();

			// 이전 레일의 정보를 가져온다.
			CGameObject* pRailObjectPre = m_vRail.at(m_vRail.size() - 2);
			CRail::RAIL_TYPE eTypePre = dynamic_cast<CRail*>(pRailObjectPre)->Get_RailType();

			// 현재 레일의 정보를 가져온다.
			CGameObject* pRailObjectLast = m_vRail.at(m_vRail.size() - 1);
			CRail::RAIL_TYPE eTypeLast = CRail::RAIL_END;
			if (!m_bConnect)
				eTypeLast = dynamic_cast<CRail*>(pRailObjectLast)->Get_RailType();
			else
				eTypeLast = CRail::RAIL_ENDING;

			// 설치한 오브젝트가 이전꺼의 좌하단에 위치
			// x의 차이 < 0, z의 차이 < 0
			if ((fGapX < 0) && (fGapZ < 0)) {
				// 이전이전 레일의 방향이 왼쪽이라면
				if (eTypePrePre == CRail::RAIL_STRAIGHT_LEFT) {
					// 이전 레일의 정보를 왼쪽 아래로 바꾼다.
					ChangeObjectType(pRailObjectPre, CRail::RAIL_ROTATE_LEFT_DOWN);
					// 현재 레일의 정보를 아래로 바꾼다.
					ChangeObjectType(pRailObjectLast, CRail::RAIL_STRAIGHT_DOWN);
					m_eStraightRailState = CRail::RAIL_STRAIGHT_DOWN;
				}
				// 이전이전 레일의 방향이 아래라면
				else if (m_eStraightRailState == CRail::RAIL_STRAIGHT_DOWN) {
					// 이전 레일의 정보를 아래 왼쪽으로 바꾼다.
					ChangeObjectType(pRailObjectPre, CRail::RAIL_ROTATE_DOWN_LEFT);
					// 현재 레일의 정보를 왼쪽으로 바꾼다.
					ChangeObjectType(pRailObjectLast, CRail::RAIL_STRAIGHT_LEFT);
					m_eStraightRailState = CRail::RAIL_STRAIGHT_LEFT;
				}
			}


			// 설치한 오브젝트가 이전꺼의 좌상단에 위치
			// x의 차이 < 0, z의 차이 > 0
			else if ((fGapX < 0) && (fGapZ > 0)) {
				// 이전이전 레일의 방향이 왼쪽이라면
				if (m_eStraightRailState == CRail::RAIL_TYPE::RAIL_STRAIGHT_LEFT) {
					// 이전 레일의 정보를 왼쪽 위로 바꾼다.
					ChangeObjectType(pRailObjectPre, CRail::RAIL_ROTATE_LEFT_UP);
					// 현재 레일의 정보를 위로 바꾼다.
					ChangeObjectType(pRailObjectLast, CRail::RAIL_STRAIGHT_UP);
					m_eStraightRailState = CRail::RAIL_STRAIGHT_UP;
				}
				// 이전이전 레일의 방향이 위쪽이라면
				else if (m_eStraightRailState == CRail::RAIL_TYPE::RAIL_STRAIGHT_UP) {
					// 이전 레일의 정보를 위 왼쪽으로 바꾼다.
					ChangeObjectType(pRailObjectPre, CRail::RAIL_ROTATE_UP_LEFT);
					// 현재 레일의 정보를 왼쪽으로 바꾼다.
					ChangeObjectType(pRailObjectLast, CRail::RAIL_STRAIGHT_LEFT);
					m_eStraightRailState = CRail::RAIL_STRAIGHT_LEFT;
				}
			}


			// 설치한 오브젝트가 이전꺼의 우하단에 위치
			// x의 차이 > 0, z의 차이 < 0
			else if ((fGapX > 0) && (fGapZ < 0)) {
				// 이전이전 레일의 방향이 오른쪽이라면
				if (m_eStraightRailState == CRail::RAIL_TYPE::RAIL_STRAIGHT_RIGHT) {
					// 이전 레일의 정보를 오른쪽 아래로 바꾼다.
					ChangeObjectType(pRailObjectPre, CRail::RAIL_ROTATE_RIGHT_DOWN);
					// 현재 레일의 정보를 아래로 바꾼다.
					ChangeObjectType(pRailObjectLast, CRail::RAIL_STRAIGHT_DOWN);
					m_eStraightRailState = CRail::RAIL_STRAIGHT_DOWN;
				}
				// 이전이전 레일의 방향이 아래쪽이라면
				else if (m_eStraightRailState == CRail::RAIL_TYPE::RAIL_STRAIGHT_DOWN) {
					// 이전 레일의 정보를 아래 오른쪽으로 바꾼다.
					ChangeObjectType(pRailObjectPre, CRail::RAIL_ROTATE_DOWN_RIGHT);
					// 현재 레일의 정보를 오른쪽으로 바꾼다.
					ChangeObjectType(pRailObjectLast, CRail::RAIL_STRAIGHT_RIGHT);
					m_eStraightRailState = CRail::RAIL_STRAIGHT_RIGHT;
				}
			}


			// 설치한 오브젝트가 이전꺼의 우상단에 위치
			// x의 차이 > 0, z의 차이 > 0
			else if ((fGapX > 0) && (fGapZ > 0)) {
				// 이전이전 레일의 방향이 위쪽이라면
				if (m_eStraightRailState == CRail::RAIL_TYPE::RAIL_STRAIGHT_UP) {
					// 이전 레일의 정보를 위 오른쪽으로 바꾼다.
					ChangeObjectType(pRailObjectPre, CRail::RAIL_ROTATE_UP_RIGHT);
					// 현재 레일의 정보를 오른쪽으로 바꾼다.
					ChangeObjectType(pRailObjectLast, CRail::RAIL_STRAIGHT_RIGHT);
					m_eStraightRailState = CRail::RAIL_STRAIGHT_RIGHT;
				}
				// 이전이전 레일의 방향이 오른쪽이라면
				else if (m_eStraightRailState == CRail::RAIL_TYPE::RAIL_STRAIGHT_RIGHT) {
					// 이전 레일의 정보를 오른쪽 위로 바꾼다.
					ChangeObjectType(pRailObjectPre, CRail::RAIL_ROTATE_RIGHT_UP);
					// 현재 레일의 정보를 위쪽으로 바꾼다.
					ChangeObjectType(pRailObjectLast, CRail::RAIL_STRAIGHT_UP);
					m_eStraightRailState = CRail::RAIL_STRAIGHT_UP;
				}
			}
		}

	}
}

void CRail_Manager::Pop_RailObject(void)
{
	if (!m_vRail.empty())
		m_vRail.pop_back();
}

void CRail_Manager::Check_ConnectEndingRail(CGameObject * pGameObject)
{
	if (nullptr != pGameObject) {
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		CLayer* pEndingRailLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Rail_End"));

		if (nullptr != pEndingRailLayer) {
			// 인자로 받은 레일의 위치가 엔딩 레일과 중점의 차이가 1이하(약1.1 정도로 잡는다.)라면
			_float3 vRailPos = dynamic_cast<CTransform*>(pGameObject->Get_Component(Component_Transform))->Get_State(CTransform::STATE_POSITION);

			for (auto& EndingRail : pEndingRailLayer->Get_ObjectList()) {
				_float3 vEndingRailPos = dynamic_cast<CTransform*>(EndingRail->Get_Component(Component_Transform))->Get_State(CTransform::STATE_POSITION);
				if (
					(abs(vRailPos.x - vEndingRailPos.x) + abs(vRailPos.z - vEndingRailPos.z)) < 1.1f
					) {
					m_bConnect = true;

					// 마지막 레일엔딩의 정보를 레일 리스트에 넣어준다. 
					m_vRail.push_back(EndingRail);
					break;
				}

				// 레일이 연결됬다는 의미이므로, 모든 기차에게 길이 연결되었음을 알리고, 스피드를 올린다.
				// 여기서는 기차의 스피드만 올린다.
				if (m_bConnect) {
					CLayer* pTrainLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Train"));
					if (nullptr != pTrainLayer) {
						for (auto& Train : pTrainLayer->Get_ObjectList()) {
							dynamic_cast<CTrain*>(Train)->Arrive();


							// 기존 열차의 스피드를 1로 올린다. 원래 0.2로 설정되어있다.
							CTransform* pTrainTransform = dynamic_cast<CTransform*>(Train->Get_Component(Component_Transform));
							CTransform::TRANSFORMDESC TransDesc = pTrainTransform->Get_TransformState();
							TransDesc.fSpeedPerSec = 1.f;
							pTrainTransform->Set_TransformState(TransDesc);
						}
					}
				}
			}
		}


	}
}

void CRail_Manager::ChangeObjectType(CGameObject * pRailObject, CRail::RAIL_TYPE eType)
{
	if (nullptr == pRailObject)
		return;

	dynamic_cast<CRail*>(pRailObject)->Change_RailType(eType);
}

void CRail_Manager::Free()
{

}