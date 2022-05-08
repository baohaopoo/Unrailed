#pragma once

#include "Train.h"

BEGIN(Client)

class CTrain_Head final : public CTrain {
private:
	explicit CTrain_Head(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTrain_Head(const CTrain_Head& rhs);
	virtual ~CTrain_Head() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_TrainComponents(void* pArg) override;
					
public:
	static CTrain_Head* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

private:
	// ===================================================================
	// 추가 변수 및 함수
	// ===================================================================
	_float		fEffectTime = 0.f;
	int			m_iStart = 0;		// 기차가 출발하기 전이면 0, 출발하면 1로 증가. 기차가 마지막레일에 도착해 폭발할 때 더 움직이고 소리나는것을 막기위해 만들었다.
	_float		m_fStartTime = 0.f;
private:
	void Create_Smoke(_float _fDeltaTime);	// 기차 위에 연기를 생성하는 함수
};

END