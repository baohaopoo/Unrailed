#pragma once

#include "Base.h"

BEGIN(Engine)
class CGameInstance;
class CVIBuffer;
class CGameObject;
END

BEGIN(Engine)

class CPick_Manager final : public CBase {
public:
	typedef struct tagPolygon {
		_float3 p1;
		_float3 p2;
		_float3 p3;

		tagPolygon() : p1(_float3(0.f,0.f,0.f)), p2(_float3(0.f, 0.f, 0.f)), p3(_float3(0.f, 0.f, 0.f)) {}
		tagPolygon(_float3 _p1, _float3 _p2, _float3 _p3)
			: p1(_p1), p2(_p2), p3(_p3) {}

	}POLYGON;

	typedef struct tagRayDesc {
		_float3 vRayPos;
		_float3 vRayDirection;
	}RAYDESC;

DECLARE_SINGLETON(CPick_Manager)

private:
	explicit CPick_Manager();
	explicit CPick_Manager(LPDIRECT3DDEVICE9 _pGraphic_Device);
	virtual ~CPick_Manager() = default;

public:
	static CPick_Manager* Create(LPDIRECT3DDEVICE9 _pGraphic_Device);

public:
	HRESULT InitDevice(HWND hWnd, _uint _iWinCX, _uint _iWinCY, _uint _iStartWinPosX = 0, _uint _iStartWinPosY = 0);
	void Tick(void);
	void LateTick(void);

public:
	bool CheckObjectPicked(CGameObject* pObject);
	CGameObject* Picking_Layer(_uint _iNumLevel, const _tchar* _tagLayer);	// 특정 레벨에 있는 인자로 받은 레이어 안의 오브젝트 중 선택한 현재 피킹되고 있는 오브젝트를 반환 하는 함수
	POLYGON* Picking(_uint _iNumLevel);	// 특정 레벨에 있는 모든 오브젝트의 VIBuffer에 대해 충돌연산을 하는 함수
	CGameObject* Picking_Object(_uint _iNumLevel);	// 특정 레벨에 있는 모든 오브젝트의 VIBuffer에 대해 충돌연산을 하여 충돌이 일어난 오브젝트를 반환하는 함수

	_float3 TriBevelMiddle(_float3 & p1, const _float3 & p2, const _float3 & p3);
	_float3 TriBevelMiddle(CPick_Manager::POLYGON & polygon);
	
	void MatchPrecisionFloat(_float& _fNum);	// 인자로 받은 float값의 정밀도를 높이는 함수, 조건은 무조껀 값이 0.5단위가 차이날것이다.
	void MatchPrecisionPoint(_float3& _fPoint);	// 인자로 받은 3차원 상의 한점의 정밀도를 높이는 함수, 조건은 무조껀 값이 0.5단위가 차이날것이다.
	void MatchPrecisionPolygon(POLYGON& _polygon);	// 인자로 받은 폴리곤 값의 정밀도를 높이는 함수, 조건은 무조껀 값이 0.5단위가 차이날것이다.

	float Pick(D3DXVECTOR3& _p1, D3DXVECTOR3& _p2, D3DXVECTOR3& _p3);
	float Pick(POLYGON& polygon);

	const RAYDESC&	GetRayDesc(void) const { return m_RayDecs; }
	const POINT&	GetMousePos(void) const { return m_ptMouse; }

public:
	virtual void Free() override;

private:
	LPDIRECT3DDEVICE9	m_pGraphic_Device;

	RAYDESC		m_RayDecs;
	POINT		m_ptMouse;

	HWND		m_hWnd;
	POINT		m_ptResolution;
	POINT		m_ptWindowStartPos;

	// 점 3개의 정보를 가지고 있는 폴리곤, 폴리곤과 레이포스 사이의 거리를 담고있는 STL이 하나 필요하다.
	map<_float, pair<POLYGON, CGameObject*>> m_lstColPolygon;
};

END

