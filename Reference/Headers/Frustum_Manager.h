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
	// 프러스텀의 월드에서의 좌표를 갱신하는 함수
	void Tick(void);
	
	// 인자로 받은 행렬을 가진 오브젝트가 프러스텀안에 존재하는지 판별하여 
	// 존재하면 true 아니라면 false값을 반환하는 함수
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

