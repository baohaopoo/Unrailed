#pragma once

#include "Base.h"

BEGIN(Engine)
class CGameInstance;
class CLayer;
class CGameObject;
class CVIBuffer;
class CGameObject;
END

BEGIN(Engine)

class CFrustum_Manager final : public CBase {

DECLARE_SINGLETON(CFrustum_Manager)

private:
	explicit CFrustum_Manager();
	explicit CFrustum_Manager(LPDIRECT3DDEVICE9 _pGraphic_Device);
	virtual ~CFrustum_Manager() = default;

public:
	static CFrustum_Manager* Create(LPDIRECT3DDEVICE9 _pGraphic_Device);

public:
	// ���������� ���忡���� ��ǥ�� �����ϴ� �Լ�
	void Tick(void);
	
	// ���ڷ� ���� ����� ���� ������Ʈ�� �������Ҿȿ� �����ϴ��� �Ǻ��Ͽ� 
	// �����ϸ� true �ƴ϶�� false���� ��ȯ�ϴ� �Լ�
	bool Check_InOut(const _float4x4& Object_WorldMatrix);

private:
	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;

public:
	virtual void Free() override;

private:
	_float3 m_vfLocalFrustumPoints[8];
	_float3 m_vfWorldFrustumPoints[8];
};

END

