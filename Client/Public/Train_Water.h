#pragma once

#include "Train.h"

BEGIN(Client)

class CTrain_Water final : public CTrain {
private:
	explicit CTrain_Water(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTrain_Water(const CTrain_Water& rhs);
	virtual ~CTrain_Water() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
private:
	_float time = 0.f;
public:
	void FireGone(_float fTimeDelta);	//ºÒ²ô´Â ÇÔ¼ö.
	void FireOn();
	bool isFire = false;

private:
	HRESULT SetUp_TrainComponents(void* pArg) override;

public:
	static CTrain_Water* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END