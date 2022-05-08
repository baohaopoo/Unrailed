#pragma once

#include "Base.h"

BEGIN(Engine)

class CPicking final : public CBase {
public:
	typedef struct tagRayDesc {
		_float3 vRayPos;
		_float3 vRayDirection;
	}RAYDESC;

DECLARE_SINGLETON(CPicking)

private:
	CPicking();
	virtual ~CPicking() = default;

public:
	void InitPickingInfo(LPDIRECT3DDEVICE9 _pGraphic_Device, HWND hWnd, _uint _iWinCX, _uint _iWinCY, _uint _iStartWinPosX = 0, _uint _iStartWinPosY = 0);
	void Tick(void);
	void LateTick(_float fTimeDelta);

public:
	float Pick(D3DXVECTOR3& _p1, D3DXVECTOR3& _p2, D3DXVECTOR3& _p3);
	const RAYDESC&	GetRayDesc(void) const { return m_RayDecs; }
	const POINT& GetMousePos(void) const { return m_ptMouse; }

public:
	virtual void Free() override;

private:
	RAYDESC		m_RayDecs;
	POINT		m_ptMouse;

	HWND		m_hWnd;
	POINT		m_ptResolution;
	POINT		m_ptWindowStartPos;

	LPDIRECT3DDEVICE9	m_pGraphic_Device;
};

END