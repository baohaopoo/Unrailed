#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Object;
class CCollider;
END

BEGIN(Client)

class CPlayer_RightHand final : public CGameObject
{
private:
	explicit CPlayer_RightHand(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayer_RightHand(const CPlayer_RightHand& rhs);
	virtual ~CPlayer_RightHand() = default;

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
	CCollider*			m_pColliderCom = nullptr;
private:
	HRESULT SetUp_Components(void* pArg);
	HRESULT SetUp_OnTerrain();

public:
	HRESULT Change_Hand(int num);
	HRESULT HandAnimation(int num);
	HRESULT PlayerCol(bool col);
	static CPlayer_RightHand* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

private:
	DIRECTION	m_eDir;
	bool m_isCol = false;
	LPD3DXMESH mesh;
	float ObjectPlayerDis = 1.f;
	float timecontrol = 0.f;
	float ObjectTurn = 0.f;
	bool ObjectRotate = false;
	_float3 vTempPosition = {};
	int iHandAnimation = 0;

	//bool
};

END