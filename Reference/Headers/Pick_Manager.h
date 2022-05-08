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
	CGameObject* Picking_Layer(_uint _iNumLevel, const _tchar* _tagLayer);	// Ư�� ������ �ִ� ���ڷ� ���� ���̾� ���� ������Ʈ �� ������ ���� ��ŷ�ǰ� �ִ� ������Ʈ�� ��ȯ �ϴ� �Լ�
	POLYGON* Picking(_uint _iNumLevel);	// Ư�� ������ �ִ� ��� ������Ʈ�� VIBuffer�� ���� �浹������ �ϴ� �Լ�
	CGameObject* Picking_Object(_uint _iNumLevel);	// Ư�� ������ �ִ� ��� ������Ʈ�� VIBuffer�� ���� �浹������ �Ͽ� �浹�� �Ͼ ������Ʈ�� ��ȯ�ϴ� �Լ�

	_float3 TriBevelMiddle(_float3 & p1, const _float3 & p2, const _float3 & p3);
	_float3 TriBevelMiddle(CPick_Manager::POLYGON & polygon);
	
	void MatchPrecisionFloat(_float& _fNum);	// ���ڷ� ���� float���� ���е��� ���̴� �Լ�, ������ ������ ���� 0.5������ ���̳����̴�.
	void MatchPrecisionPoint(_float3& _fPoint);	// ���ڷ� ���� 3���� ���� ������ ���е��� ���̴� �Լ�, ������ ������ ���� 0.5������ ���̳����̴�.
	void MatchPrecisionPolygon(POLYGON& _polygon);	// ���ڷ� ���� ������ ���� ���е��� ���̴� �Լ�, ������ ������ ���� 0.5������ ���̳����̴�.

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

	// �� 3���� ������ ������ �ִ� ������, ������� �������� ������ �Ÿ��� ����ִ� STL�� �ϳ� �ʿ��ϴ�.
	map<_float, pair<POLYGON, CGameObject*>> m_lstColPolygon;
};

END

