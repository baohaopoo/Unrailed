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

class CCube_Object final : public CGameObject
{
private:
	explicit CCube_Object(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCube_Object(const CCube_Object& rhs);
	virtual ~CCube_Object() = default;

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
	CVIBuffer_Cube*		m_pVIBufferCom = nullptr;

	/* 저 모델위에 어떤 텍스쳐 올릴건지?! */
	CTexture*			m_pTextureCom = nullptr;

	/* 그린다!!! */
	CRenderer*			m_pRendererCom = nullptr;

private:
	HRESULT SetUp_Components(void * pArg);

public:
	static CCube_Object* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END