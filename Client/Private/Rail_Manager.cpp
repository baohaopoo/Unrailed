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

	// �������� ���� ������ ������ �������ϰ� ����Ǿ����� �Ǻ��Ѵ�.
	Check_ConnectEndingRail(pGameObject);

	if (m_vRail.size() > 2) {
		// ������ ���Ϸ� ���� �������� ������ ������ �˻��Ѵ�.
		CTransform* pTransformPre =
			dynamic_cast<CTransform*>(m_vRail.at(m_vRail.size() - 3)->Get_Component(Component_Transform));

		if (nullptr == pTransformPre)
			return;

		_float3 vPosPre = pTransformPre->Get_State(CTransform::STATE_POSITION);

		// �ش� ���ϰ� ���� �߰��� ������ ���� ������ ���Ѵ�.
		CTransform* pTransformNext =
			dynamic_cast<CTransform*>(pGameObject->Get_Component(Component_Transform));

		_float3 vPosNext = pTransformNext->Get_State(CTransform::STATE_POSITION);

		// ���� �࿡ �ִ��� �ٸ� �࿡ �ִ��� �˻�
		// ���� ������ ���̰� ��Ʈ2���� ũ�ٸ� ������, ��Ʈ2�� ���ٸ� �ٸ��࿡ �ִ�.
		// �̴� ť����� ���μ��� ���̰� 1�̱� ������ ������ �� �ִ� ����̴�.

		// �񱳸� ���� 2�� �ƴ� ���� ū, 4���� ���� ���� �ִ´�.

		_float fGapX, fGapZ;
		// ��ġ�� ���Ϸκ��� ���� ������ ������ ����. 
		fGapX = vPosNext.x - vPosPre.x;
		fGapZ = vPosNext.z - vPosPre.z;

		// ���� �߽����� ������ �������� ���Ѱ��� 2���� �۴ٸ�, �̴� ���� �࿡ �ִ�.
		if (pow(fGapX, 2.f) + pow(fGapZ, 2.f) <= 1.f) {
			// ���� ������ ������ ���� ������ �������� �Ѵ�.

			// ���� ������ ������ �����´�.
			CGameObject* pRailObjectPre = m_vRail.at(m_vRail.size() - 2);
			CRail::RAIL_TYPE eTypePre = dynamic_cast<CRail*>(pRailObjectPre)->Get_RailType();

			// ���� ������ ������ �����´�.
			CGameObject* pRailObjectLast = m_vRail.at(m_vRail.size() - 1);

			// ���� ������ ������ ���� ������ �������� �����Ѵ�.
			if (pRailObjectLast->Get_Type() != OBJECT_RAIL_END)
				dynamic_cast<CRail*>(pRailObjectLast)->Change_RailType(eTypePre);
		}
		// ���� �߽����� ������ ������ ���� 2���� ũ�ٸ�, �̴� �ٸ� �࿡ �ִ�. 
		else {
			// �÷��̾��� ��ġ�� ���� ��ġ�� ������ ������ �����ϰ�, �������� ��ġ�� ���� ���������� ������ �ٲ��ش�.

			// �������� ������ ������ �����´�.
			CGameObject* pRailObjectPrePre = m_vRail.at(m_vRail.size() - 3);
			CRail::RAIL_TYPE eTypePrePre = dynamic_cast<CRail*>(pRailObjectPrePre)->Get_RailType();

			// ���� ������ ������ �����´�.
			CGameObject* pRailObjectPre = m_vRail.at(m_vRail.size() - 2);
			CRail::RAIL_TYPE eTypePre = dynamic_cast<CRail*>(pRailObjectPre)->Get_RailType();

			// ���� ������ ������ �����´�.
			CGameObject* pRailObjectLast = m_vRail.at(m_vRail.size() - 1);
			CRail::RAIL_TYPE eTypeLast = CRail::RAIL_END;
			if (!m_bConnect)
				eTypeLast = dynamic_cast<CRail*>(pRailObjectLast)->Get_RailType();
			else
				eTypeLast = CRail::RAIL_ENDING;

			// ��ġ�� ������Ʈ�� �������� ���ϴܿ� ��ġ
			// x�� ���� < 0, z�� ���� < 0
			if ((fGapX < 0) && (fGapZ < 0)) {
				// �������� ������ ������ �����̶��
				if (eTypePrePre == CRail::RAIL_STRAIGHT_LEFT) {
					// ���� ������ ������ ���� �Ʒ��� �ٲ۴�.
					ChangeObjectType(pRailObjectPre, CRail::RAIL_ROTATE_LEFT_DOWN);
					// ���� ������ ������ �Ʒ��� �ٲ۴�.
					ChangeObjectType(pRailObjectLast, CRail::RAIL_STRAIGHT_DOWN);
					m_eStraightRailState = CRail::RAIL_STRAIGHT_DOWN;
				}
				// �������� ������ ������ �Ʒ����
				else if (m_eStraightRailState == CRail::RAIL_STRAIGHT_DOWN) {
					// ���� ������ ������ �Ʒ� �������� �ٲ۴�.
					ChangeObjectType(pRailObjectPre, CRail::RAIL_ROTATE_DOWN_LEFT);
					// ���� ������ ������ �������� �ٲ۴�.
					ChangeObjectType(pRailObjectLast, CRail::RAIL_STRAIGHT_LEFT);
					m_eStraightRailState = CRail::RAIL_STRAIGHT_LEFT;
				}
			}


			// ��ġ�� ������Ʈ�� �������� �»�ܿ� ��ġ
			// x�� ���� < 0, z�� ���� > 0
			else if ((fGapX < 0) && (fGapZ > 0)) {
				// �������� ������ ������ �����̶��
				if (m_eStraightRailState == CRail::RAIL_TYPE::RAIL_STRAIGHT_LEFT) {
					// ���� ������ ������ ���� ���� �ٲ۴�.
					ChangeObjectType(pRailObjectPre, CRail::RAIL_ROTATE_LEFT_UP);
					// ���� ������ ������ ���� �ٲ۴�.
					ChangeObjectType(pRailObjectLast, CRail::RAIL_STRAIGHT_UP);
					m_eStraightRailState = CRail::RAIL_STRAIGHT_UP;
				}
				// �������� ������ ������ �����̶��
				else if (m_eStraightRailState == CRail::RAIL_TYPE::RAIL_STRAIGHT_UP) {
					// ���� ������ ������ �� �������� �ٲ۴�.
					ChangeObjectType(pRailObjectPre, CRail::RAIL_ROTATE_UP_LEFT);
					// ���� ������ ������ �������� �ٲ۴�.
					ChangeObjectType(pRailObjectLast, CRail::RAIL_STRAIGHT_LEFT);
					m_eStraightRailState = CRail::RAIL_STRAIGHT_LEFT;
				}
			}


			// ��ġ�� ������Ʈ�� �������� ���ϴܿ� ��ġ
			// x�� ���� > 0, z�� ���� < 0
			else if ((fGapX > 0) && (fGapZ < 0)) {
				// �������� ������ ������ �������̶��
				if (m_eStraightRailState == CRail::RAIL_TYPE::RAIL_STRAIGHT_RIGHT) {
					// ���� ������ ������ ������ �Ʒ��� �ٲ۴�.
					ChangeObjectType(pRailObjectPre, CRail::RAIL_ROTATE_RIGHT_DOWN);
					// ���� ������ ������ �Ʒ��� �ٲ۴�.
					ChangeObjectType(pRailObjectLast, CRail::RAIL_STRAIGHT_DOWN);
					m_eStraightRailState = CRail::RAIL_STRAIGHT_DOWN;
				}
				// �������� ������ ������ �Ʒ����̶��
				else if (m_eStraightRailState == CRail::RAIL_TYPE::RAIL_STRAIGHT_DOWN) {
					// ���� ������ ������ �Ʒ� ���������� �ٲ۴�.
					ChangeObjectType(pRailObjectPre, CRail::RAIL_ROTATE_DOWN_RIGHT);
					// ���� ������ ������ ���������� �ٲ۴�.
					ChangeObjectType(pRailObjectLast, CRail::RAIL_STRAIGHT_RIGHT);
					m_eStraightRailState = CRail::RAIL_STRAIGHT_RIGHT;
				}
			}


			// ��ġ�� ������Ʈ�� �������� ���ܿ� ��ġ
			// x�� ���� > 0, z�� ���� > 0
			else if ((fGapX > 0) && (fGapZ > 0)) {
				// �������� ������ ������ �����̶��
				if (m_eStraightRailState == CRail::RAIL_TYPE::RAIL_STRAIGHT_UP) {
					// ���� ������ ������ �� ���������� �ٲ۴�.
					ChangeObjectType(pRailObjectPre, CRail::RAIL_ROTATE_UP_RIGHT);
					// ���� ������ ������ ���������� �ٲ۴�.
					ChangeObjectType(pRailObjectLast, CRail::RAIL_STRAIGHT_RIGHT);
					m_eStraightRailState = CRail::RAIL_STRAIGHT_RIGHT;
				}
				// �������� ������ ������ �������̶��
				else if (m_eStraightRailState == CRail::RAIL_TYPE::RAIL_STRAIGHT_RIGHT) {
					// ���� ������ ������ ������ ���� �ٲ۴�.
					ChangeObjectType(pRailObjectPre, CRail::RAIL_ROTATE_RIGHT_UP);
					// ���� ������ ������ �������� �ٲ۴�.
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
			// ���ڷ� ���� ������ ��ġ�� ���� ���ϰ� ������ ���̰� 1����(��1.1 ������ ��´�.)���
			_float3 vRailPos = dynamic_cast<CTransform*>(pGameObject->Get_Component(Component_Transform))->Get_State(CTransform::STATE_POSITION);

			for (auto& EndingRail : pEndingRailLayer->Get_ObjectList()) {
				_float3 vEndingRailPos = dynamic_cast<CTransform*>(EndingRail->Get_Component(Component_Transform))->Get_State(CTransform::STATE_POSITION);
				if (
					(abs(vRailPos.x - vEndingRailPos.x) + abs(vRailPos.z - vEndingRailPos.z)) < 1.1f
					) {
					m_bConnect = true;

					// ������ ���Ͽ����� ������ ���� ����Ʈ�� �־��ش�. 
					m_vRail.push_back(EndingRail);
					break;
				}

				// ������ �����ٴ� �ǹ��̹Ƿ�, ��� �������� ���� ����Ǿ����� �˸���, ���ǵ带 �ø���.
				// ���⼭�� ������ ���ǵ常 �ø���.
				if (m_bConnect) {
					CLayer* pTrainLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Train"));
					if (nullptr != pTrainLayer) {
						for (auto& Train : pTrainLayer->Get_ObjectList()) {
							dynamic_cast<CTrain*>(Train)->Arrive();


							// ���� ������ ���ǵ带 1�� �ø���. ���� 0.2�� �����Ǿ��ִ�.
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