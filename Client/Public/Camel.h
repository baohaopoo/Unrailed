#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Transform.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Object;
END

BEGIN(Client)

class CCamel final : public CGameObject {
private:
	explicit CCamel(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCamel(const CCamel& rhs);
	virtual ~CCamel() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	/* 움직인다.(상태를 표현한다.) */
	CTransform*			m_pTransformCom = nullptr;
	
	/* 어떤모양으로 그릴건지?! (모델) */
	CVIBuffer_Object*	m_pVIBufferCom = nullptr;	

	/* 저 모델위에 어떤 텍스쳐 올릴건지?! */
	CTexture*			m_pTextureCom = nullptr;

	/* 그린다!!! */
	CRenderer*			m_pRendererCom = nullptr;

private:
	HRESULT SetUp_Components(void* pArg);

private:
	int			m_iLife;
	int			m_imove = 2;
	_float		fTime = 0.f;
	_float		fTempTime = 0.f;

public:
	// 라이프가 1 줄어들고, VIBuffer가 교체된다.
	void Get_Damage(void);

public:
	static CCamel* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END