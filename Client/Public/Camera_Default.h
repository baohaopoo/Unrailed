#pragma once

#include "Client_Defines.h"
#include "Camera.h"
#include "Player.h"
BEGIN(Client)

class CCamera_Default final : public CCamera
{
private:
	explicit CCamera_Default(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera_Default(const CCamera_Default& rhs);
	virtual ~CCamera_Default() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	bool CheckIntersect(_float3 center1, float radius1, _float3 center2, float radius2);
	void Camera_Base(_float fTimeDelta);
	void Camera_Player(_float fTimeDelta);
public:
	static CCamera_Default* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
	bool Get_ChangeCamera() { return ChangeCamera; }

private:
	bool m_isCol = false;
	bool ChangeCamera = false;
public:
	static D3DXVECTOR3		Get_Mouse(void)
	{
		POINT	pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		return D3DXVECTOR3((float)pt.x, (float)pt.y, 0.f);
	}

	void	Set_Mouse(_long ptX = (g_iWinCX >> 1),
		_long ptY = (g_iWinCY >> 1))
	{
		POINT	pt = { ptX, ptY };

		m_vPreMouse.x = pt.x;
		m_vPreMouse.y = pt.y;

		ClientToScreen(g_hWnd, &pt);
		SetCursorPos(pt.x, pt.y);
	}

private:
	_float3	m_vMouse;
	_float3 m_vPreMouse;
	_float f1pTime = 0.f;		// 1인칭 카메라에서 사용하는 시간
	_float f3pTime = 0.f;		// 3인칭 카메라에서 사용하는 시간

};

END