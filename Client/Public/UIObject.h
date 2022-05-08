#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Cube;
END

BEGIN(Client)



class UIObject  final : public CGameObject
{
private:
	explicit UIObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	UIObject(const UIObject& rhs);
	virtual ~UIObject() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CTransform*			m_pTransformCom = nullptr;
	CVIBuffer_Cube*		m_pVIBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;

private:
	DIRECTION	m_eDir;
	bool m_isCol = false;
	LPD3DXMESH mesh;
private:
	HRESULT SetUp_Components();
	bool CheckIntersect(_float3 center1, float radius1, _float3 center2, float radius2);

public:
	static UIObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END