#pragma once

#include "Train.h"

BEGIN(Client)

class CTrain_Rail final : public CTrain {
private:
	explicit CTrain_Rail(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTrain_Rail(const CTrain_Rail& rhs);
	virtual ~CTrain_Rail() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	void TrainState(bool onrail);

private:
	HRESULT SetUp_TrainComponents(void* pArg) override;

public:
	static CTrain_Rail* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


public:
	// ===================================================================
	// 추가 변수 및 함수
	// ===================================================================
	void GetMaterial(int num);
	HRESULT	Ready_Layer_Rail(const _tchar* pLayerTag);

private:
	int			materialNum = 0;
};

END