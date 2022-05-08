#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Object;
END

BEGIN(Client)

class CPlayer_Feet final : public CGameObject
{
private:
	explicit CPlayer_Feet(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayer_Feet(const CPlayer_Feet& rhs);
	virtual ~CPlayer_Feet() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CTransform*			m_pTransformCom = nullptr;
	CVIBuffer_Object*	m_pVIBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;

private:
	HRESULT SetUp_Components(void* pArg);
	HRESULT SetUp_OnTerrain();
	bool CheckIntersect(_float3 center1, float radius1, _float3 center2, float radius2);

public:
	static CPlayer_Feet* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

private:
	DIRECTION	m_eDir;
	bool m_isCol = false;
	LPD3DXMESH mesh;
	int GrabObject = 0;
	float ObjectPlayerDis = 1.f;
	float timecontrol = 0.f;
	float ObjectTurn = -30.f;
	bool ObjectRotate = false;
	_float3 vTempPosition = {};
};

END