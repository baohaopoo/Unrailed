#pragma once

#include "Train.h"

BEGIN(Client)

class CTrain_Dynamite final : public CTrain {
private:
	explicit CTrain_Dynamite(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTrain_Dynamite(const CTrain_Dynamite& rhs);
	virtual ~CTrain_Dynamite() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_TrainComponents(void* pArg) override;

public:
	static CTrain_Dynamite* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END