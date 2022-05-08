#pragma once

#include "Client_Defines.h"
#include "Engine_Defines.h"
#include "Base.h"
#include "GameObject.h"
#include "Cube_Object.h"
#include "Rail.h"

BEGIN(Client)

class CRail_Manager final : public CBase {

	DECLARE_SINGLETON(CRail_Manager)

private:
	explicit CRail_Manager(LPDIRECT3DDEVICE9 _pGraphic_Device);
	explicit CRail_Manager();
	~CRail_Manager();

public:
	static CRail_Manager* Create(LPDIRECT3DDEVICE9 _pGraphic_Device);

public:
	// 여기 들어오는 오브젝트는 이미 생성이 완료된 객체이다. 함수 내부에선 모양만 바꿔주면 된다.
	void Push_RailObject(CGameObject* pGameObject);
	void Pop_RailObject(void);
	void Check_ConnectEndingRail(CGameObject * pGameObject);

private:
	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;

public:
	vector<CGameObject*>& Get_vRailInfo(void) { return m_vRail; }
	CRail::RAIL_TYPE Get_State(void) { return m_eStraightRailState; }
	void Set_State(CRail::RAIL_TYPE _eType) { m_eStraightRailState = _eType; }

private:
	void ChangeObjectType(CGameObject* pRailObject, CRail::RAIL_TYPE eType);

public:
	virtual void Free() override;

public:
	bool	Get_Connected(void) { return m_bConnect; }

private:
	vector<CGameObject*> m_vRail;
	bool	m_bConnect = false;

	CRail::RAIL_TYPE m_eStraightRailState = CRail::RAIL_TYPE::RAIL_STRAIGHT_RIGHT;	// 처음 상태는 오른쪽으로 가는 레일인 상태이다.
};

END

