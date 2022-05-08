#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Transform.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Object;
class CCollider;
END

BEGIN(Client)

class CGiant final : public CGameObject {
private:
	explicit CGiant(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGiant(const CGiant& rhs);
	virtual ~CGiant() = default;

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

private:
	bool m_isCol = false;
	float fTime = 0.f;
	bool m_updown = false;
	float fup = 0.f;
	float fdown = 0.f;
public:
	static CGiant* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END