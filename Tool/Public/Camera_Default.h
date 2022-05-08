#pragma once

#include "Tool_Defines.h"
#include "Camera.h"

BEGIN(Tool)

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

public:
	static CCamera_Default* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

public:
	void SetObserving(bool _bObserve) { m_bObserving = _bObserve; }
	bool GetObserving(void) { return m_bObserving; }

private:
	// 관찰과 관련된 변수, 현재 변수가 true일 경우에만 카메라를 움직일 수 있음.
	bool m_bObserving = true;
};

END