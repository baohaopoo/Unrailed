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

class CPlayer_LeftHand final : public CGameObject
{
private:
	explicit CPlayer_LeftHand(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayer_LeftHand(const CPlayer_LeftHand& rhs);
	virtual ~CPlayer_LeftHand() = default;

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

public:
	static CPlayer_LeftHand* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

public:
	HRESULT Change_Hand(bool lift);
	HRESULT PlayerLift(int num);

private:
	DIRECTION	m_eDir;
	bool m_isCol = false;
	LPD3DXMESH mesh;
	float ObjectPlayerDis = 1.f;
	float timecontrol = 0.f;
	float ObjectTurn = 14.f;
	bool ObjectRotate = false;
	_float3 vTempPosition = {};
	bool liftsomething = false;
};

END