#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Cube;
END

BEGIN(Client)

class CEffect_Rain final : public CGameObject
{
private:
	explicit CEffect_Rain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEffect_Rain(const CEffect_Rain& rhs);
	virtual ~CEffect_Rain() = default;
public:
	virtual	HRESULT			NativeConstruct_Prototype();
	virtual	HRESULT			NativeConstruct(void* pArg);
	virtual	void			Tick(_float fTimeDelta);
	virtual void			LateTick(_float fTimeDelta);
	virtual HRESULT			Render();

private:
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Cube*			m_pVIBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;

	D3DXMATRIX World{};
	_float3 fPos;
	_float fTime = 0.f;

private:
	HRESULT					SetUp_Components(void * pArg);

public:
	static	CEffect_Rain*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual	CGameObject*	Clone(void* pArg) override;
	virtual	void			Free() override;

};

END