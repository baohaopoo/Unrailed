#pragma once

#include "Tool_Defines.h"
#include "Camera.h"

BEGIN(Tool)

class CCamera_Observer final : public CCamera
{
private:
	explicit CCamera_Observer(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera_Observer(const CCamera_Observer& rhs);
	virtual ~CCamera_Observer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	static CCamera_Observer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

private:
	_float	m_fRadius;
};

END