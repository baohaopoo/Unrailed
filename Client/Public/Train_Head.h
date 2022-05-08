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
	// �߰� ���� �� �Լ�
	// ===================================================================
	_float		fEffectTime = 0.f;
	int			m_iStart = 0;		// ������ ����ϱ� ���̸� 0, ����ϸ� 1�� ����. ������ ���������Ͽ� ������ ������ �� �� �����̰� �Ҹ����°��� �������� �������.
	_float		m_fStartTime = 0.f;
private:
	void Create_Smoke(_float _fDeltaTime);	// ���� ���� ���⸦ �����ϴ� �Լ�
};

END